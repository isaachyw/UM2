function write_xdmf_2d(filename::String, 
                       HRPM::HierarchicalRectangularlyPartitionedMesh)
    @info "Writing $filename" 
    # Check valid filename
    if !occursin(".xdmf", filename)
        @error "Invalid filename. '.xdmf' does not occur in $filename"
    end

    # If there are materials, map all material names to an integer
    material_map = make_material_name_to_id_map(HRPM)

    # h5 filename
    h5_filename = replace(filename, ("xdmf" => "h5"))
    h5_file = h5open(h5_filename, "w")
    # XML
    xdoc = XMLDocument()
    # Xdmf
    xroot = create_root(xdoc, "Xdmf")
    set_attribute(xroot, "Version", "3.0")
    # Domain
    xdomain = new_child(xroot, "Domain")
    # Material names
    if material_map != Dict{String, Int64}()
        xmaterials = new_child(xdomain, "Information")
        set_attribute(xmaterials, "Name", "MaterialNames")
        add_text(xmaterials, join(keys(material_map), " ")) 
    end
    add_HRPM_xdmf!(xdomain, h5_filename, h5_file, HRPM, material_map)
    save_file(xdoc, filename)
    close(h5_file)
end

function write_xdmf_2d(filename::String, mesh::UnstructuredMesh_2D)
    @info "Writing $filename" 
    # Check valid filename
    if !occursin(".xdmf", filename)
        @error "Invalid filename. '.xdmf' does not occur in $filename"
    end

    # If there are materials, map all material names to an integer
    material_map = Dict{String, Int64}()
    if mesh.face_sets != Dict{String, Set{UInt32}}()
        material_map = make_material_name_to_id_map(mesh)
    end

    # h5 filename
    h5_filename = replace(filename, ("xdmf" => "h5"))
    h5_file = h5open(h5_filename, "w")
    # XML
    xdoc = XMLDocument()
    # Xdmf
    xroot = create_root(xdoc, "Xdmf")
    set_attribute(xroot, "Version", "3.0")
    # Domain
    xdomain = new_child(xroot, "Domain")
    # Material names
    if material_map != Dict{String, Int64}()
        xmaterials = new_child(xdomain, "Information")
        set_attribute(xmaterials, "Name", "MaterialNames")
        add_text(xmaterials, join(keys(material_map), " ")) 
    end

    # Add uniform grid
    add_uniform_grid_xdmf!(xdomain, h5_filename, h5_file, mesh, material_map)

    save_file(xdoc, filename)
    close(h5_file)
end

# Helper functions for write_xdmf
# -------------------------------------------------------------------------------------------------
function add_uniform_grid_xdmf!(xml::XMLElement,
                                h5_filename::String,
                                h5_mesh::Union{HDF5.Group, HDF5.File},
                                mesh::UnstructuredMesh_2D,
                                material_map::Dict{String, Int64})
    @debug "Adding uniform grid for $(mesh.name)"
    # Grid                                                                       
    xgrid = new_child(xml, "Grid")
    set_attribute(xgrid, "Name", mesh.name)
    set_attribute(xgrid, "GridType", "Uniform")

    # h5
    h5_group = create_group(h5_mesh, mesh.name)
 
    # Geometry
    write_xdmf_geometry!(xgrid, h5_filename, h5_group, mesh)
 
    # Topology
    write_xdmf_topology!(xgrid, h5_filename, h5_group, mesh)
 
    # Non-material face sets
    if mesh.face_sets != Dict{String, Set{UInt64}}()
        write_xdmf_face_sets!(xgrid, h5_filename, h5_group, mesh)
    end
 
    # Materials
    if material_map != Dict{String, Int64}()
        write_xdmf_materials!(xgrid, h5_filename, h5_group, mesh, material_map)
    end
    return nothing
end

function write_xdmf_geometry!(xml::XMLElement, 
                              h5_filename::String, 
                              h5_mesh::HDF5.Group, 
                              mesh::UnstructuredMesh_2D)
    @debug "Writing XDMF geometry"
    # Geometry
    xgeom = new_child(xml, "Geometry")
    set_attribute(xgeom, "GeometryType", "XYZ")
    # DataItem
    xdataitem = new_child(xgeom, "DataItem")
    set_attribute(xdataitem, "DataType", "Float")
    npoints = length(mesh.points)
    set_attribute(xdataitem, "Dimensions", "$npoints 3")
    set_attribute(xdataitem, "Format", "HDF")
    float_precision = 8
    set_attribute(xdataitem, "Precision", "$float_precision")
    h5_text_item = split(string(h5_mesh))[2]
    add_text(xdataitem, string(h5_filename, ":", h5_text_item, "/points"))
    # Convert the points into an array
    point_array = zeros(3, npoints)
    for i = 1:npoints
        point_array[1, i] = mesh.points[i][1]
        point_array[2, i] = mesh.points[i][2]
    end
    # Write the h5
    h5_mesh["points"] = point_array
    return nothing
end

function write_xdmf_topology!(xml::XMLElement, 
                              h5_filename::String, 
                              h5_mesh::HDF5.Group, 
                              mesh::UnstructuredMesh_2D)
    @debug "Writing XDMF topology"
    # Topology
    xtopo = new_child(xml, "Topology")
    set_attribute(xtopo, "TopologyType", "Mixed")
    nelements = length(mesh.faces)
    set_attribute(xtopo, "NumberOfElements", "$nelements")
    # DataItem
    xdataitem = new_child(xtopo, "DataItem")
    set_attribute(xdataitem, "DataType", "Int")
    topo_length = mapreduce(x->length(x), +, mesh.faces) + nelements
    topo_array = Vector{UInt64}(undef, topo_length)
    convert_xdmf_faces_to_array!(topo_array, mesh.faces)
    ndimensions = length(topo_array)
    set_attribute(xdataitem, "Dimensions", "$ndimensions")
    set_attribute(xdataitem, "Format", "HDF")
    set_attribute(xdataitem, "Precision", "8")
    h5_text_item = split(string(h5_mesh))[2]
    add_text(xdataitem, string(h5_filename, ":", h5_text_item, "/cells"))
    # Write the h5
    h5_mesh["cells"] = topo_array
    return nothing
end

function convert_xdmf_faces_to_array!(topo_array::Vector{UInt64}, 
                                      faces::Vector{<:SArray{S, UInt32, 1, L} where {S<:Tuple, L}})
#    length_to_xdmf_type = Dict{UInt32, UInt32}(
#        # triangle
#        3  => 4,
#        # triangle6
#        6 => 36, 
#        # quadrilateral
#        4 => 5,
#        # quad8
#        8 => 37
#       )  
    topo_ctr = 1
    for face in faces
        # convert face to vector for mutability
        # adjust 1-based to 0-based indexing
        face_xdmf = collect(face) .- 1
        # add xdmf type
        face_length = length(face)
        if face_length === 3
            pushfirst!(face_xdmf, 4)
        elseif face_length === 4
            pushfirst!(face_xdmf, 5)
        elseif face_length === 6
            pushfirst!(face_xdmf, 36)
        elseif face_length === 8
            pushfirst!(face_xdmf, 37)
        else
            @error "Unknown face type"
        end
        topo_array[topo_ctr:topo_ctr + face_length] = face_xdmf
        topo_ctr += face_length + 1
    end
    return nothing
end

function make_material_name_to_id_map(mesh::UnstructuredMesh_2D)
    material_map = Dict{String, Int64}()
    mat_names = String[]
    max_length = 0
    for set_name in keys(mesh.face_sets)
        if occursin("MATERIAL", uppercase(set_name))
            material_map[set_name] = 0
            push!(mat_names, set_name)
            if length(set_name) > max_length
                max_length = length(set_name)
            end
        end
    end
    # Adjust material IDs so that IDs are assigned alphabetically
    sort!(mat_names)
    for (i, set_name) in enumerate(mat_names)
        material_map[set_name] = i - 1 
    end 
    if max_length < 13
        max_length = 13
    end
    println(string(rpad("Material Name", max_length, ' '), " : XDMF Material ID"))
    println(rpad("=", max_length + 19, '='))
    for set_name in mat_names
        if occursin("MATERIAL", uppercase(set_name))
            id = material_map[set_name]
            println(string(rpad(set_name, max_length, ' '), " : $id"))  
        end 
    end 
    return material_map
end

function make_material_name_to_id_map(HRPM::HierarchicalRectangularlyPartitionedMesh)
    mesh_children = [HRPM]
    next_mesh_children = HierarchicalRectangularlyPartitionedMesh[]
    leaves_reached = false
    while !leaves_reached
        for child_mesh in mesh_children
            if length(child_mesh.children) > 0
                for child_ref in child_mesh.children
                    push!(next_mesh_children, child_ref[])
                end
            else
                leaves_reached = true
            end
        end
        if !leaves_reached
            mesh_children = next_mesh_children
            next_mesh_children = HierarchicalRectangularlyPartitionedMesh[]
        end
    end
    material_map = Dict{String, Int64}()
    mat_names = String[]
    max_length = 0
    for leaf_mesh in mesh_children 
        UM = leaf_mesh.mesh[]
        for set_name in keys(UM.face_sets)
            if occursin("MATERIAL", uppercase(set_name))
                if set_name ∉  keys(material_map)
                    material_map[set_name] = 0 
                    push!(mat_names, set_name)
                    if length(set_name) > max_length
                        max_length = length(set_name)
                    end
                end
            end
        end
    end
    # Adjust material IDs so that IDs are assigned alphabetically
    sort!(mat_names)
    for (i, set_name) in enumerate(mat_names)
        material_map[set_name] = i - 1
    end
    if max_length < 13
        max_length = 13
    end
    println(string(rpad("Material Name", max_length, ' '), " : XDMF Material ID"))
    println(rpad("=", max_length + 19, '='))
    for set_name in mat_names
        if occursin("MATERIAL", uppercase(set_name))
            id = material_map[set_name]
            println(string(rpad(set_name, max_length, ' '), " : $id"))  
        end
    end
    return material_map
end

function write_xdmf_materials!(xml::XMLElement, 
                               h5_filename::String, 
                               h5_mesh::HDF5.Group, 
                               mesh::UnstructuredMesh_2D,
                               material_map::Dict{String, Int64})
    @debug "Writing XDMF materials"
    # MaterialID
    xmaterial = new_child(xml, "Attribute")
    set_attribute(xmaterial, "Center", "Cell")
    set_attribute(xmaterial, "Name", "MaterialID")
    nelements = length(mesh.faces)
    mat_ID_array = fill(-1, nelements)
    for material_name in keys(material_map)
        if material_name ∈  keys(mesh.face_sets)
            material_ID = material_map[material_name]
            for cell in mesh.face_sets[material_name]
                if mat_ID_array[cell] === -1
                    mat_ID_array[cell] = material_ID
                else
                    @error "Mesh cell $cell has multiple materials assigned to it."
                end
            end
        end
    end
    if any(x->x === -1, mat_ID_array)
        @error "Some mesh cells do not have a material."
    end
    # DataItem
    xdataitem = new_child(xmaterial, "DataItem")
    set_attribute(xdataitem, "DataType", "Int")
    set_attribute(xdataitem, "Dimensions", "$nelements")
    set_attribute(xdataitem, "Format", "HDF")
    set_attribute(xdataitem, "Precision", "8")
    h5_text_item = split(string(h5_mesh))[2]
    add_text(xdataitem, string(h5_filename, ":", h5_text_item, "/material_id"))
    # Write the h5
    h5_mesh["material_id"] = mat_ID_array
    return nothing
end

function write_xdmf_face_sets!(xml::XMLElement, 
                               h5_filename::String, 
                               h5_mesh::HDF5.Group, 
                               mesh::UnstructuredMesh_2D)
    @debug "Writing XDMF face_sets"
    for set_name in keys(mesh.face_sets)
        if occursin("MATERIAL", uppercase(set_name))
            continue
        end
        # Set
        xset = new_child(xml, "Set")
        set_attribute(xset, "Name", set_name)
        set_attribute(xset, "SetType", "Cell")
        # DataItem
        xdataitem = new_child(xset, "DataItem")
        set_attribute(xdataitem, "DataType", "Int")
        nelements = length(mesh.face_sets[set_name])
        set_attribute(xdataitem, "Dimensions", "$nelements")
        set_attribute(xdataitem, "Format", "HDF")
        set_attribute(xdataitem, "Precision", "8")
        h5_text_item = split(string(h5_mesh))[2]
        add_text(xdataitem, string(h5_filename, ":", h5_text_item, "/", set_name))
        # Write the h5
        ID_array = collect(mesh.face_sets[set_name]) .- 1 
        h5_mesh[set_name] = ID_array
    end
    return nothing
end

function add_HRPM_xdmf!(xml::XMLElement,
                        h5_filename::String,
                        h5_mesh::Union{HDF5.Group, HDF5.File},
                        HRPM::HierarchicalRectangularlyPartitionedMesh,
                        material_map::Dict{String, Int64})
    @debug "Adding HRPM for $(HRPM.name)"
    if length(HRPM.children) > 0 
        # Grid
        xgrid = new_child(xml, "Grid")
        set_attribute(xgrid, "Name", HRPM.name)
        set_attribute(xgrid, "GridType", "Tree")
        # h5_group
        h5_group = create_group(h5_mesh, HRPM.name)
        for child in HRPM.children
            add_HRPM_xdmf!(xgrid, h5_filename, h5_group, child[], material_map)
        end
    else
        add_uniform_grid_xdmf!(xml, h5_filename, h5_mesh, HRPM.mesh[], material_map)         
    end
end
