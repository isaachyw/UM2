Base.@kwdef mutable struct HierarchicalRectangularlyPartitionedMesh
    mesh::Union{Nothing, UnstructuredMesh_2D} = nothing
    parent::Union{
                  Nothing,
                  Ref{HierarchicalRectangularlyPartitionedMesh}
                 } = nothing
    children::Vector{Ref{HierarchicalRectangularlyPartitionedMesh}
                    } = Ref{HierarchicalRectangularlyPartitionedMesh}[]
end

function partition_rectangularly(mesh::UnstructuredMesh_2D)
    @info "Converting UnstructuredMesh_2D into HierarchicalRectangularlyPartitionedMesh"
    # Extract set names, grid names, and max level
    set_names, grid_names, max_level = _process_partition_rectangularly_input(mesh)

    # Create a tree to store grid relationships.
    root = _create_HRPM_tree(mesh, grid_names, max_level)
end

# Extract set names, grid names, and max level
function _process_partition_rectangularly_input(mesh::UnstructuredMesh_2D)
    set_names = [ key for key in keys(mesh.face_sets) ]
    grid_names = copy(set_names)
    for set_name in set_names
        if !occursin("GRID_", uppercase(set_name))
            filter!(x->x ≠ set_name, grid_names)
        end
    end

    if length(grid_names) === 0
        error("No grid face sets in mesh.")
    end

    # Get the number of grid levels
    max_level = 0
    for grid_name in grid_names
        level = parse(Int64, grid_name[7])
        if max_level < level
            max_level = level
        end
    end

    return set_names, grid_names, max_level
end

# Create a tree to store grid relationships.
function _create_HRPM_tree(mesh::UnstructuredMesh_2D, grid_names::Vector{String}, max_level::Int64)
    root = Tree( data = mesh.name )
    current_nodes = Tree[]
    next_nodes = Tree[]
    old_grid_names = copy(grid_names)
    # Do first level
    for grid_name in old_grid_names
        grid_level = parse(Int64, grid_name[7])
        if grid_level === 1
            # Add to appropriate node (root)
            push!(next_nodes, Tree(data = grid_name; parent=Ref(root)))
            filter!(x->x ≠ grid_name, grid_names)
        end
    end
    # Do all other levels:
    for level in 2:max_level
        old_grid_names = copy(grid_names)
        current_nodes = next_nodes
        next_nodes = []
        for grid_name in old_grid_names
            grid_level = parse(Int64, grid_name[7])
            if grid_level == level
                # find the parent for this grid
                grid_faces = Set(mesh.face_sets[grid_name])
                for node in current_nodes
                    node_faces = Set(mesh.face_sets[node.data])
                    if grid_faces ⊆ node_faces
                        push!(next_nodes, Tree(data = grid_name, parent=Ref(node)))
                        filter!(x->x ≠ grid_name, grid_names)
                        break
                    end
                end
            end
        end
    end
    return root
end
