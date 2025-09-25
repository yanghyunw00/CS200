/**
 * \file
 * \author Rudy Castan
 * \author TODO Your Name
 * \date 2025 Fall
 * \par CS200 Computer Graphics I
 * \copyright DigiPen Institute of Technology
 */
#include "Image.hpp"
#include "Engine/Error.hpp"
#include "Engine/Path.hpp"
#include "OpenGL/GL.hpp"
#include <stb_image.h>


namespace CS200
{

    /**
     * \brief Load an image from file and store its pixel data
     * \param image_path Path to the image file (relative to Assets folder, like "Assets/ship.png")
     * \param flip_vertical Whether to flip the image vertically when loading (default: false)
     *
     * Implementation notes:
     * - Use assets::locate_asset() to find the full file path
     * - Use stb_image library functions to load the image data
     * - Always load as 4-channel RGBA regardless of source format
     * - Set stbi_set_flip_vertically_on_load() before loading
     * - Throw an error if loading fails
     * - Store the loaded pixel data and image dimensions
     */
    Image::Image(const std::filesystem::path& image_path, bool flip_vertical)
    {
        const std::filesystem::path vertex_file   = assets::locate_asset("image_path");
        const std::filesystem::path fragment_file = assets::locate_asset();
        gShader                                   = OpenGL::CreateShader(vertex_file, fragment_file);

        data = stbi_load(image_path.string().c_str(), &width, &height, &file_num_channels, &num_channels);

        // GL::GenTexture(1,) & gDuckTexture
        // glBindTexture(GL_TEXTURE_2D, gDuckTexture);


        // flitering


        // Wrapping
    }

    /**
     * \brief Move constructor - transfer ownership of image data
     * \param temporary The temporary Image object to move from
     *
     * Implementation notes:
     * - Transfer ownership of pixel data pointer and size
     * - Set the source object's data to nullptr and size to {0,0}
     * - Use noexcept to enable move optimizations
     */
    Image::Image(Image&& temporary) noexcept
    {
    }

    /**
     * \brief Move assignment - transfer ownership of image data
     * \param temporary The temporary Image object to move from
     * \return Reference to this object
     *
     * Implementation notes:
     * - Use std::swap to exchange data between objects
     * - This safely handles self-assignment and cleanup
     */
    Image& Image::operator=(Image&& temporary) noexcept
    {
        // TODO: insert return statement here
    }

    /**
     * \brief Destructor - free the allocated image data
     *
     * Implementation notes:
     * - Check if pixel data pointer is not nullptr
     * - Use stbi_image_free() to properly release memory allocated by stb_image
     */
    Image::~Image()
    {
    }

    /**
     * \brief Get read-only access to the pixel data array
     * \return Const pointer to the first RGBA pixel
     *
     * Implementation notes:
     * - Return the stored pixel data pointer as const
     * - Pixels are stored in row-major order (left-to-right, top-to-bottom)
     */
    const RGBA* Image::data() const noexcept
    {
        return nullptr;
    }

    /**
     * \brief Get read-write access to the pixel data array
     * \return Pointer to the first RGBA pixel for modification
     *
     * Implementation notes:
     * - Return the stored pixel data pointer for modification
     * - Pixels are stored in row-major order (left-to-right, top-to-bottom)
     */
    RGBA* Image::data() noexcept
    {
        return nullptr;
    }

    /**
     * \brief Get the dimensions of the loaded image
     * \return Vector containing width (x) and height (y) in pixels
     *
     * Implementation notes:
     * - Return the stored image dimensions
     * - Width and height are set during image loading
     */
    Math::ivec2 Image::GetSize() const noexcept
    {
        return Math::ivec2();
    }

}
