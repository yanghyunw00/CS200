﻿/**
 * \file
 * \author Rudy Castan
 * \author Jonathan Holmes
 * \author Hyunwoo Yang
 * \date 2025 Fall
 * \par CS200 Computer Graphics I
 * \copyright DigiPen Institute of Technology
 */

#include "Texture.hpp"

#include "CS200/IRenderer2D.hpp"
#include "CS200/Image.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Window.hpp"
#include "Logger.hpp"
#include "OpenGL/GL.hpp"
#include <utility>

namespace CS230
{

    static bool IsIdentityMatrix(const Math::TransformationMatrix& mat)
    {
        return mat[0][0] == 1.0 && mat[0][1] == 0.0 && mat[0][2] == 0.0 && mat[1][0] == 0.0 && mat[1][1] == 1.0 && mat[1][2] == 0.0 && mat[2][0] == 0.0 && mat[2][1] == 0.0 && mat[2][2] == 1.0;
    }

    void CS230::Texture::Draw(const Math::TransformationMatrix& display_matrix, unsigned int color)
    {
        Draw(display_matrix, { 0, 0 }, GetSize(), color);
    }

    void CS230::Texture::Draw(const Math::TransformationMatrix& display_matrix, Math::ivec2 texel_position, Math::ivec2 frame_size, unsigned int color)
    {
        if (textureHandle == 0)
        {
            return;
        }

        Math::TransformationMatrix transform_matrix = display_matrix;

        const Math::vec2 st_min{ static_cast<float>(texel_position.x) / static_cast<float>(size.x), 1.0f - static_cast<float>(texel_position.y + frame_size.y) / static_cast<float>(size.y) };
        const Math::vec2 st_max{ static_cast<float>(texel_position.x + frame_size.x) / static_cast<float>(size.x), 1.0f - static_cast<float>(texel_position.y) / static_cast<float>(size.y) };

        const Math::ScaleMatrix scale_matrix{
            { static_cast<double>(frame_size.x), static_cast<double>(frame_size.y) }
        };

        const CS200::RGBA tintColor = color;

        if (IsIdentityMatrix(display_matrix)) // background
        {
            if (frame_size.x == size.x && frame_size.y == size.y)
            {
                transform_matrix = Math::TranslationMatrix(Math::vec2{ static_cast<double>(frame_size.x) / 2.0, static_cast<double>(frame_size.y) / 2.0 });
            }
            const Math::TransformationMatrix final_transform = transform_matrix * scale_matrix;
            Engine::GetRenderer2D().DrawQuad(final_transform, textureHandle, st_min, st_max, tintColor);
        }
        else // cat, robot
        {
            const Math::TranslationMatrix    toCenter(Math::vec2{ 0.5, 0.5 });
            const Math::TransformationMatrix final_transform = display_matrix * scale_matrix * toCenter;
            Engine::GetRenderer2D().DrawQuad(final_transform, textureHandle, st_min, st_max, tintColor);
        }
    }

    Math::ivec2 CS230::Texture::GetSize() const
    {
        return size;
    }

    CS230::Texture::~Texture()
    {
        if (textureHandle != 0)
        {
            GL::DeleteTextures(1, &textureHandle);
        }
    }

    CS230::Texture::Texture(Texture&& temporary) noexcept : textureHandle(temporary.textureHandle), size(temporary.size)
    {
        temporary.textureHandle = 0;
        temporary.size          = { 0, 0 };
    }

    Texture& CS230::Texture::operator=(Texture&& temporary) noexcept
    {
        if (this != &temporary)
        {
            if (textureHandle != 0)
            {
                GL::DeleteTextures(1, &textureHandle);
            }

            textureHandle = temporary.textureHandle;
            size          = temporary.size;

            temporary.textureHandle = 0;
            temporary.size          = { 0, 0 };
        }
        return *this;
    }

    Texture::Texture(const std::filesystem::path& file_name) : textureHandle(0), size{ 0, 0 }
    {
        CS200::Image image(file_name, true);

        if (image.data())
        {
            textureHandle = OpenGL::CreateTextureFromImage(image);
            size          = image.GetSize();
        }
        else
        {
            Engine::GetLogger().LogError("Failed to load texture: " + file_name.string());
        }
    }

    Texture::Texture(OpenGL::TextureHandle given_texture, Math::ivec2 the_size) : textureHandle(given_texture), size(the_size)
    {
    }

}