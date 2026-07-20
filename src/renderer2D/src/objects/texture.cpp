#include "renderer2d/inc/objects/texture.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"


namespace R2D
{
  void Texture::alloc(
    std::size_t    width,
    std::size_t    height,
    TEXTURE_FORMAT format,
    uint16_t       numLayers,
    const void*    data
  )
  {
    if (m_valid)
    {
      LOG_ERROR("TEXTURE HAS ALREADY BEEN ALLOCATED");
      return;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
    glTextureStorage2D(m_texture, numLayers, static_cast<GLenum>(format), width, height);
    LOG_GL_ERRORS("TEXTURE DATA STORE ERROR");

    GLenum pixelFormat;
    GLenum pixelType;

    switch (format)
    {
    case TEXTURE_FORMAT::R16F:
      pixelType = GL_HALF_FLOAT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::RGB8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RGB;
      break;
    case TEXTURE_FORMAT::RGBA8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGBA16F:
      pixelType = GL_HALF_FLOAT;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGBA32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGB32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RGB;
      break;
    case TEXTURE_FORMAT::DEPTH32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_DEPTH_COMPONENT;
      break;
    case TEXTURE_FORMAT::R32U:
      pixelType = GL_UNSIGNED_INT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R32I:
      pixelType = GL_INT;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::R8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R8I:
      pixelType = GL_BYTE;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::R8UI:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::DEPTH24:
      pixelType = GL_UNSIGNED_INT;
      pixelFormat = GL_DEPTH_COMPONENT;
      break;
    }

    if (data)
    {
      glTextureSubImage2D(m_texture, 0, 0, 0, width, height, pixelFormat, pixelType, data);
      LOG_GL_ERRORS("TEXTURE ALLOC");
    }

    m_width       = width;
    m_height      = height;
    m_format      = format;
    m_pixelType   = pixelType;
    m_pixelFormat = pixelFormat;
    m_valid       = true;
  }

  void Texture::update(int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height, void const* data)
  {
    if (!m_valid)
    {
      LOG_ERROR("TEXTURE NOT YET ALLOCATED BEFORE UPDATE WAS CALLED");
      return;
    }
    glTextureSubImage2D(
      m_texture,
      0,
      xOffset,
      yOffset,
      width,
      height,
      m_pixelFormat,
      m_pixelType,
      data
    );
    LOG_GL_ERRORS("TEXTURE SUB DATA");
  }

  void Texture::destroy(void)
  {
    if (!m_valid)
    {
      return;
    }
    glDeleteTextures(1, &m_texture);
    m_valid = false;
  }

  void Texture::bind(int32_t textureUnit) const
  {
    glBindTextureUnit(textureUnit, m_texture);
    LOG_GL_ERRORS("TEXTURE BIND ERROR");
  }

	void Texture::bindAsImage(int32_t imageUnit, IMAGE_ACCESS imageAccess)
	{
		glBindImageTexture(imageUnit, m_texture, 0, GL_FALSE, 0, static_cast<GLenum>(imageAccess), static_cast<GLenum>(m_format));
	}

	void Texture::setTextureParams(TEX_PARAM_NAMES name, TEX_PARAM_VALUES value)
  {
    // bind the texture to GL_TEXTURE_2D so that subsequent texture modification calls
    // will modify the bound texture
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(name), static_cast<GLenum>(value));
    LOG_GL_ERRORS("TEXTURE PARAM ERROR");
  }

  uint32_t Texture::getWidth(void) const noexcept
  {
    return m_width;
  }

  uint32_t Texture::getHeight(void) const noexcept
  {
    return m_height;
  }

  TEXTURE_FORMAT Texture::getFormat(void) const noexcept
  {
    return m_format;
  }

  GLuint Texture::getTextureID(void) const noexcept
  {
    return m_texture;
  }

  bool Texture::isValid() const
  {
    return m_valid;
  }

  void Texture2DArray::alloc(
    std::size_t     width,
    std::size_t     height,
    std::size_t     layers,
    TEXTURE_FORMAT  format,
    uint16_t        numLayers
  )
  {
    if (m_valid)
    {
      LOG_ERROR("TEXTURE2D_ARRAY HAS ALREADY BEEN ALLOCATED");
      return;
    }

    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_texture);
    glTextureStorage3D(m_texture, numLayers, static_cast<GLenum>(format), static_cast<GLsizei>(width), static_cast<GLsizei>(height), static_cast<GLsizei>(layers));
    LOG_GL_ERRORS("TEXTURE2D_ARRAY DATA STORE ERROR");

    GLenum pixelFormat;
    GLenum pixelType;

    switch (format)
    {
    case TEXTURE_FORMAT::R16F:
      pixelType = GL_HALF_FLOAT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::RGB8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RGB;
      break;
    case TEXTURE_FORMAT::RGBA8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGBA16F:
      pixelType = GL_HALF_FLOAT;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGBA32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RGBA;
      break;
    case TEXTURE_FORMAT::RGB32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_RGB;
      break;
    case TEXTURE_FORMAT::DEPTH32F:
      pixelType = GL_FLOAT;
      pixelFormat = GL_DEPTH_COMPONENT;
      break;
    case TEXTURE_FORMAT::R32U:
      pixelType = GL_UNSIGNED_INT;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R32I:
      pixelType = GL_INT;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::R8:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RED;
      break;
    case TEXTURE_FORMAT::R8I:
      pixelType = GL_BYTE;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::R8UI:
      pixelType = GL_UNSIGNED_BYTE;
      pixelFormat = GL_RED_INTEGER;
      break;
    case TEXTURE_FORMAT::DEPTH24:
      pixelType = GL_UNSIGNED_INT;
      pixelFormat = GL_DEPTH_COMPONENT;
      break;
    }

    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    m_layers = static_cast<uint32_t>(layers);
    m_format = format;
    m_pixelType = pixelType;
    m_pixelFormat = pixelFormat;
    m_valid = true;
  }

  void Texture2DArray::update(int32_t xOffset, int32_t yOffset, int32_t layer, uint32_t width, uint32_t height, void const* data)
  {
    if (!m_valid)
    {
      LOG_ERROR("TEXTURE NOT YET ALLOCATED BEFORE UPDATE WAS CALLED");
      return;
    }
    glTextureSubImage3D(
      m_texture,
      0,
      xOffset,
      yOffset,
      layer,
      width,
      height,
      1,
      m_pixelFormat,
      m_pixelType,
      data
    );
    LOG_GL_ERRORS("TEXTURE2D_ARRAY SUB DATA");
  }

  void Texture2DArray::destroy(void)
  {
    if (!m_valid)
    {
      return;
    }
    glDeleteTextures(1, &m_texture);
    m_valid = false;
  }

  void Texture2DArray::bind(int32_t textureUnit) const
  {
    glBindTextureUnit(textureUnit, m_texture);
    LOG_GL_ERRORS("TEXTURE2D_ARRAY BIND ERROR");
  }

  void Texture2DArray::setTextureParams(TEX_PARAM_NAMES name, TEX_PARAM_VALUES value)
  {
    // bind the texture to GL_TEXTURE_2D so that subsequent texture modification calls
    // will modify the bound texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, static_cast<GLenum>(name), static_cast<GLenum>(value));
    LOG_GL_ERRORS("TEXTURE2D_ARRAY PARAM ERROR");
  }

  uint32_t Texture2DArray::getWidth(void) const noexcept
  {
    return m_width;
  }

  uint32_t Texture2DArray::getHeight(void) const noexcept
  {
    return m_height;
  }

  uint32_t Texture2DArray::getLayers(void) const noexcept
  {
    return m_layers;
  }

  TEXTURE_FORMAT Texture2DArray::getFormat(void) const noexcept
  {
    return m_format;
  }

  GLuint Texture2DArray::getTextureID(void) const noexcept
  {
    return m_texture;
  }

  bool Texture2DArray::isValid() const
  {
    return m_valid;
  }
}
