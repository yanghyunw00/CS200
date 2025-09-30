/**
 * \file
 * \author Rudy Castan
 * \author Hyunwwoo Yang
 * \date 2025 Fall
 * \par CS200 Computer Graphics I
 * \copyright DigiPen Institute of Technology
 */
#include "ImmediateRenderer2D.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Matrix.hpp"
#include "Engine/Texture.hpp"
#include "OpenGL/Buffer.hpp"
#include "OpenGL/GL.hpp"
#include "Renderer2DUtils.hpp"
#include <utility>

namespace CS200
{
    ImmediateRenderer2D::ImmediateRenderer2D(ImmediateRenderer2D&& other) noexcept
        : vao(other.vao), ibo(other.ibo), vbo(other.vbo), indicesCount(other.indicesCount), uniformBlock(other.uniformBlock), sdfVao(other.sdfVao), sdfVbo(other.sdfVbo), uboCamera(other.uboCamera),
          viewProjection(other.viewProjection), textureShader(std::move(other.textureShader)), sdfShader(std::move(other.sdfShader))
    {
        other.vao          = 0;
        other.vbo          = 0;
        other.ibo          = 0;
        other.indicesCount = 0;
        other.sdfVao       = 0;
        other.sdfVbo       = 0;
        other.uboCamera    = 0;
        other.uniformBlock = 0;
    }

    ImmediateRenderer2D& ImmediateRenderer2D::operator=(ImmediateRenderer2D&& other) noexcept
    {
        if (this != &other)
        {
            std::swap(vao, other.vao);
            std::swap(vbo, other.vbo);
            std::swap(ibo, other.ibo);
            std::swap(indicesCount, other.indicesCount);
            std::swap(sdfVao, other.sdfVao);
            std::swap(uniformBlock, other.uniformBlock);
            std::swap(sdfVbo, other.sdfVbo);
            std::swap(uboCamera, other.uboCamera);
            std::swap(viewProjection, other.viewProjection);
            std::swap(textureShader, other.textureShader);
            std::swap(sdfShader, other.sdfShader);
        }

        return *this;
    }

    ImmediateRenderer2D::~ImmediateRenderer2D()
    {
        Shutdown();
    }

    struct Vertex
    {
        float x;
        float y;
        float s;
        float t;
    };

    void ImmediateRenderer2D::Init()
    {
        const std::array<unsigned int, 6> indices  = { 0, 1, 2, 2, 3, 0 };
        const std::array<Vertex, 4>       vertices = {
            Vertex{ -0.5f, -0.5f, 0.0f, 0.0f },
            Vertex{  0.5f, -0.5f, 1.0f, 0.0f },
            Vertex{  0.5f,  0.5f, 1.0f, 1.0f },
            Vertex{ -0.5f,  0.5f, 0.0f, 1.0f }
        };

        vbo          = OpenGL::CreateBuffer(OpenGL::BufferType::Vertices, std::as_bytes(std::span{ vertices }));
        ibo          = OpenGL::CreateBuffer(OpenGL::BufferType::Indices, std::as_bytes(std::span{ indices }));
        indicesCount = static_cast<GLsizei>(indices.size());


        const auto layout = OpenGL::BufferLayout{
            { OpenGL::Attribute::Float2, OpenGL::Attribute::Float2 }
        };

        const auto vertex_buffer = OpenGL::VertexBuffer{ vbo, layout };

        vao = OpenGL::CreateVertexArrayObject(vertex_buffer, ibo);

        using filepath = std::filesystem::path;
        textureShader  = OpenGL::CreateShader(filepath{ "Assets/shaders/ImmediateRenderer2D/quad.vert" }, filepath{ "Assets/shaders/ImmediateRenderer2D/quad.frag" });
    }

    void ImmediateRenderer2D::Shutdown()
    {
        OpenGL::DestroyShader(textureShader);
        OpenGL::DestroyShader(sdfShader);

        GL::DeleteBuffers(1, &vbo);
        GL::DeleteBuffers(1, &ibo);
        GL::DeleteBuffers(1, &uniformBlock);
        GL::DeleteBuffers(1, &sdfVbo);

        GL::DeleteVertexArrays(1, &vao);
        GL::DeleteVertexArrays(1, &sdfVao);

        textureShader = {};
        uniformBlock  = {};
        sdfShader     = {};
        vbo           = {};
        ibo           = {};
        sdfVbo        = {};
        uboCamera     = {};
        vao           = {};
        sdfVao        = {};
        indicesCount  = 0;
    }

    void ImmediateRenderer2D::BeginScene(const Math::TransformationMatrix& view_projection)
    {
        this->viewProjection = view_projection;
    }

    void ImmediateRenderer2D::EndScene()
    {
    }

    void ImmediateRenderer2D::DrawQuad(const Math::TransformationMatrix& transform, OpenGL::TextureHandle texture, Math::vec2 texture_coord_bl, Math::vec2 texture_coord_tr, CS200::RGBA tintColor)
    {
        GL::UseProgram(textureShader.Shader);

        const Math::vec2                 tex_scale          = texture_coord_tr - texture_coord_bl;
        const Math::TransformationMatrix texcoord_transform = Math::TranslationMatrix(texture_coord_bl) * Math::ScaleMatrix(tex_scale);

        const std::array<float, 9> model      = CS200::Renderer2DUtils::to_opengl_mat3(transform);
        const std::array<float, 9> tex_mat    = CS200::Renderer2DUtils::to_opengl_mat3(texcoord_transform);
        const std::array<float, 9> view_proj  = CS200::Renderer2DUtils::to_opengl_mat3(viewProjection);
        const std::array<float, 4> tint_array = CS200::unpack_color(tintColor);

        GL::UniformMatrix3fv(textureShader.UniformLocations.at("uModel"), 1, GL_FALSE, model.data());
        GL::UniformMatrix3fv(textureShader.UniformLocations.at("uTexCoordTransform"), 1, GL_FALSE, tex_mat.data());
        GL::UniformMatrix3fv(textureShader.UniformLocations.at("uViewProjection"), 1, GL_FALSE, view_proj.data());
        GL::Uniform4fv(textureShader.UniformLocations.at("uTint"), 1, tint_array.data());
        GL::Uniform1f(textureShader.UniformLocations.at("uDepth"), 0.0f);
        GL::Uniform1i(textureShader.UniformLocations.at("uTexture"), 0);

        GL::ActiveTexture(GL_TEXTURE0);
        GL::BindTexture(GL_TEXTURE_2D, texture);
        GL::BindVertexArray(vao);
        GL::DrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);

        GL::UseProgram(0);
    }
}