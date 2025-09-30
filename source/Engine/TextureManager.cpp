/**
 * \file
 * \author Rudy Castan
 * \author Jonathan Holmes
 * \author Hyunwoo Yang
 * \date 2025 Fall
 * \par CS200 Computer Graphics I
 * \copyright DigiPen Institute of Technology
 */

#include "TextureManager.hpp"
#include "CS200/IRenderer2D.hpp"
#include "CS200/Image.hpp"
#include "CS200/NDC.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "OpenGL/GL.hpp"
#include <iostream>

namespace CS230
{

    Texture* TextureManager::Load(const std::filesystem::path& file_name)
    {
        const std::string key = file_name.string();

        auto it = texture_cache.find(key);
        if (it != texture_cache.end())
        {
            return it->second.get();
        }

        try
        {
            CS200::Image image(file_name, true);
            const OpenGL::TextureHandle handle = OpenGL::CreateTextureFromImage(image, OpenGL::Filtering::NearestPixel, OpenGL::Wrapping::ClampToEdge);

            Texture*                 raw_ptr = new Texture(handle, image.GetSize());
            std::unique_ptr<Texture> new_texture(raw_ptr);
            Texture*                 texture_ptr = new_texture.get();
            texture_cache[key]                   = std::move(new_texture);

            return texture_ptr;
        }
        catch (const std::runtime_error& e)
        {
            Engine::GetLogger().LogError("Failed to load texture: " + std::string(e.what()));
            return nullptr;
        }
    }

    void TextureManager::Unload()
    {
        texture_cache.clear();
    }

}
