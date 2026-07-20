#pragma once

#include <cstdint>

#include "renderer2d/inc/utils/gl_commons.hpp"

namespace R2D
{
  class Texture
  {
  public:
    void alloc(
      std::size_t    width,
      std::size_t    height,
      TEXTURE_FORMAT format    = TEXTURE_FORMAT::RGB8,
      uint16_t       numLayers = 1,
      void const*    data      = nullptr
    );
    void update(int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height, void const* data);
    void destroy(void);

    void bind(int32_t textureUnit) const;
    void bindAsImage(int32_t imageUnit, IMAGE_ACCESS imageAccess);
    void setTextureParams(TEX_PARAM_NAMES name, TEX_PARAM_VALUES value);

    uint32_t getWidth(void) const noexcept;
    uint32_t getHeight(void) const noexcept;

    TEXTURE_FORMAT getFormat(void) const noexcept;
    GLuint         getTextureID(void) const noexcept;

    bool isValid() const;

  private:
    friend class FrameBuffer;
    friend class TextureData;

    GLuint m_texture;

    uint32_t       m_width;
    uint32_t       m_height;
    TEXTURE_FORMAT m_format;
    GLenum         m_pixelType;
    GLenum         m_pixelFormat;

    bool m_valid {false};
  };

  class Texture2DArray
  {
  public:
    void alloc(
      std::size_t     width,
      std::size_t     height,
      std::size_t     layers,
      TEXTURE_FORMAT  format = TEXTURE_FORMAT::RGB8,
      uint16_t        numLayers = 1
    );

    void update(int32_t xOffset, int32_t yOffset, int32_t layer, uint32_t width, uint32_t height, void const* data);
    void destroy(void);

    void bind(int32_t textureUnit) const;
    void setTextureParams(TEX_PARAM_NAMES name, TEX_PARAM_VALUES value);

    uint32_t getWidth(void) const noexcept;
    uint32_t getHeight(void) const noexcept;
    uint32_t getLayers(void) const noexcept;

    TEXTURE_FORMAT getFormat(void) const noexcept;
    GLuint         getTextureID(void) const noexcept;

    bool isValid() const;

  private:
    friend class FrameBuffer;
    friend class TextureData;

    GLuint m_texture;

    uint32_t        m_width;
    uint32_t        m_height;
    uint32_t        m_layers;
    TEXTURE_FORMAT  m_format;
    GLenum          m_pixelType;
    GLenum          m_pixelFormat;

    bool m_valid{ false };
  };
}
