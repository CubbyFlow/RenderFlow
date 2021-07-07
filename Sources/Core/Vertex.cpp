#include <Core/Vertex.hpp>

namespace Core
{
std::size_t VertexHelper::GetNumberOfFloats(VertexFormat format)
{
    std::size_t size = 0;

    if (static_cast<int>(format & VertexFormat::Position3))
        size += 3;

    if (static_cast<int>(format & VertexFormat::Normal3))
        size += 3;

    if (static_cast<int>(format & VertexFormat::TexCoord2))
        size += 2;

    if (static_cast<int>(format & VertexFormat::TexCoord3))
        size += 3;

    if (static_cast<int>(format & VertexFormat::Color4))
        size += 4;

    if (static_cast<int>(format & VertexFormat::Tangent4))
        size += 4;

    return size;
}

std::size_t VertexHelper::GetSizeInBytes(VertexFormat format)
{
    return sizeof(float) * GetNumberOfFloats(format);
}

}  // namespace Core