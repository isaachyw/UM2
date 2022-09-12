export encode_morton, decode_morton

const MORTON_INDEX_TYPE = UInt32 # 32-bit or 64-bit
const MORTON_MAX_INDEX = typemax(MORTON_INDEX_TYPE)
const MORTON_MAX_SIDE  = typemax(MORTON_INDEX_TYPE) >> (4 * sizeof(MORTON_INDEX_TYPE))
const MORTON_MAX_LEVELS = 4 * sizeof(MORTON_INDEX_TYPE)

const morton_masks_32 = (
    0x0000ffff, 
    0x00ff00ff,
    0x0f0f0f0f,
    0x33333333,
    0x55555555,
)

const morton_masks_64 = (
    0x00000000ffffffff,
    0x0000ffff0000ffff,
    0x00ff00ff00ff00ff,
    0x0f0f0f0f0f0f0f0f,
    0x3333333333333333,
    0x5555555555555555,
)     

function pdep_0x55555555(x::UInt32)
    x &= morton_masks_32[1]
    x = (x | (x << 8)) & morton_masks_32[2]
    x = (x | (x << 4)) & morton_masks_32[3]
    x = (x | (x << 2)) & morton_masks_32[4]
    x = (x | (x << 1)) & morton_masks_32[5]
    return x
end

function pdep_0x5555555555555555(x::UInt64)
    x &= morton_masks_64[1]
    x = (x | (x << 16)) & morton_masks_64[2]
    x = (x | (x <<  8)) & morton_masks_64[3]
    x = (x | (x <<  4)) & morton_masks_64[4]
    x = (x | (x <<  2)) & morton_masks_64[5]
    x = (x | (x <<  1)) & morton_masks_64[6]
    return x
end

function pext_0x55555555(x::UInt32)
    x &= morton_masks_32[5]
    x = (x ^ (x >> 1)) & morton_masks_32[4]
    x = (x ^ (x >> 2)) & morton_masks_32[3]
    x = (x ^ (x >> 4)) & morton_masks_32[2]
    x = (x ^ (x >> 8)) & morton_masks_32[1]
    return x
end

function pext_0x5555555555555555(x::UInt64)
    x &= morton_masks_64[6]
    x = (x ^ (x >>  1)) & morton_masks_64[5]
    x = (x ^ (x >>  2)) & morton_masks_64[4]
    x = (x ^ (x >>  4)) & morton_masks_64[3]
    x = (x ^ (x >>  8)) & morton_masks_64[2]
    x = (x ^ (x >> 16)) & morton_masks_64[1]
    return x
end

# For UInt32, assumes x, y are in the range [0, 2^16]
# For UInt64, assumes x, y are in the range [0, 2^32]
if UM2_HAS_BMI2
    function encode_morton(x::UInt32, y::UInt32)
        return pdep(x, 0x55555555) | pdep(y, 0xaaaaaaaa)
    end

    function encode_morton(x::UInt64, y::UInt64)
        return pdep(x, 0x5555555555555555) | pdep(y, 0xaaaaaaaaaaaaaaaa)
    end

    decode_morton(z::UInt32) = pext(z, 0x55555555), pext(z, 0xaaaaaaaa)
    decode_morton(z::UInt64) = pext(z, 0x5555555555555555), pext(z, 0xaaaaaaaaaaaaaaaa)
else
    function encode_morton(x::UInt32, y::UInt32)
        x = pdep_0x55555555(x)
        y = pdep_0x55555555(y)
        return x | (y << 1)
    end

    function encode_morton(x::UInt64, y::UInt64)
        x = pdep_0x5555555555555555(x)
        y = pdep_0x5555555555555555(y)
        return x | (y << 1)
    end

    function decode_morton(z::UInt32)
        x = pext_0x55555555(z)
        y = pext_0x55555555(z >> 1)
        return x, y
    end

    function decode_morton(z::UInt64)
        x = pext_0x5555555555555555(z)
        y = pext_0x5555555555555555(z >> 1)
        return x, y
    end
end

# Normalizes the coordinate to the range [0, MORTON_MAX_SIDE]
function encode_morton(x::T, y::T, scale_inv::T) where {T <: Union{Float32, Float64}}
    x_u = floor(MORTON_INDEX_TYPE, x * scale_inv * MORTON_MAX_SIDE )
    y_u = floor(MORTON_INDEX_TYPE, y * scale_inv * MORTON_MAX_SIDE )
    return encode_morton(x_u, y_u)
end

function decode_morton(z::MORTON_INDEX_TYPE, scale::T
    ) where {T <: Union{Float32, Float64}}
    x, y = decode_morton(z)
    return x * scale / MORTON_MAX_SIDE, y * scale / MORTON_MAX_SIDE
end
