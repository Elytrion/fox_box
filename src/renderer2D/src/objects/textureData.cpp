#include "renderer2d/inc/objects/textureData.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"

namespace R2D
{
	bool TextureData::isValid() const
	{
		return m_valid;
	}

	void TextureData::readIn(Texture const& textureIn)
	{
		if (!textureIn.isValid()) {
			m_valid = false;
			return;
		}

		m_width  = textureIn.m_width;
		m_height = textureIn.m_height;
		m_format = textureIn.m_format;

		uint64_t const pixelCount = static_cast<uint64_t>(m_width) * m_height;

		uint64_t textureSizeBytes = 0;

		bool isDepth = false;

		switch (textureIn.m_format)
		{
		case TEXTURE_FORMAT::R8:
			m_pixelType = ATTRIBUTE_TYPE::BYTE;
			m_pixelCompSize  = sizeof(uint8_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R8I:
			m_pixelType = ATTRIBUTE_TYPE::BYTE;
			m_pixelCompSize = sizeof(uint8_t);
			m_pixelStride = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R8UI:
			m_pixelType = ATTRIBUTE_TYPE::UBYTE;
			m_pixelCompSize = sizeof(uint8_t);
			m_pixelStride = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R16F:
			m_pixelType = ATTRIBUTE_TYPE::HALF_FLOAT;
			m_pixelCompSize  = sizeof(uint16_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R32F:
			m_pixelType = ATTRIBUTE_TYPE::FLOAT;
			m_pixelCompSize  = sizeof(uint32_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R32U:
			m_pixelType = ATTRIBUTE_TYPE::UINT;
			m_pixelCompSize  = sizeof(uint32_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::R32I:
			m_pixelType = ATTRIBUTE_TYPE::INT;
			m_pixelCompSize  = sizeof(uint32_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::RGB8:
			m_pixelType = ATTRIBUTE_TYPE::BYTE;
			m_pixelCompSize  = sizeof(uint8_t);
			m_pixelStride    = m_pixelCompSize * 3u;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::RGBA8:
			m_pixelType = ATTRIBUTE_TYPE::BYTE;
			m_pixelCompSize  = sizeof(uint8_t);
			m_pixelStride    = m_pixelCompSize * 4u;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::RGBA16F:
			m_pixelType = ATTRIBUTE_TYPE::HALF_FLOAT;
			m_pixelCompSize  = sizeof(uint16_t);
			m_pixelStride    = m_pixelCompSize * 4u;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::RGBA32F:
			m_pixelType = ATTRIBUTE_TYPE::FLOAT;
			m_pixelCompSize  = sizeof(uint32_t);
			m_pixelStride    = m_pixelCompSize * 4u;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::RGB32F:
			m_pixelType = ATTRIBUTE_TYPE::FLOAT;
			m_pixelCompSize = sizeof(uint32_t);
			m_pixelStride = m_pixelCompSize * 3u;
			textureSizeBytes = pixelCount * m_pixelStride;
			break;
		case TEXTURE_FORMAT::DEPTH24:
			m_pixelType = ATTRIBUTE_TYPE::FLOAT;
			m_pixelCompSize  = sizeof(float);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			isDepth = true;
			break;
		case TEXTURE_FORMAT::DEPTH32F:
			m_pixelType = ATTRIBUTE_TYPE::FLOAT;
			m_pixelCompSize  = sizeof(uint32_t);
			m_pixelStride    = m_pixelCompSize;
			textureSizeBytes = pixelCount * m_pixelStride;
			isDepth = true;
			break;
		}

		m_data.resize(textureSizeBytes, 0);

		if (!isDepth)
		{
			glGetTextureSubImage(
				textureIn.m_texture,
				0,
				0,
				0,
				0,
				m_width,
				m_height,
				1,
				textureIn.m_pixelFormat,
				textureIn.m_pixelType,
				textureSizeBytes,
				m_data.data()
			);
			LOG_GL_ERRORS("FAILED TO READ TEXTURE DATA");
		}
		else
		{
			glReadPixels(
				0,
				0,
				m_width,
				m_height,
				GL_DEPTH_COMPONENT,
				static_cast<GLenum>(m_pixelType),
				m_data.data()
			);
			LOG_GL_ERRORS("FAILED TO READ PIXELS FROM DEPTH ATTACHMENT");
		}

		m_valid = true;
	}

	uint8_t const TextureData::readPixel(int x, int y) const
	{
		if (!m_valid || m_data.empty())
		{
			LOG_ERROR("Attempted to read from invalid texture data");
			return 0;
		}

		return *getPtr(x, y);
	}

	uint8_t const* TextureData::getPtr(int x, int y) const
	{
		if (!m_valid || m_data.empty())
		{
			LOG_ERROR("Attempted to read from invalid texture data");
			return nullptr;
		}

		return m_data.data() +
		(
			m_width * (m_height - y) * m_pixelStride +
			x * m_pixelStride
		);
	}
	uint8_t const* TextureData::getDataPtr() const
	{
		return m_data.data();
	}
}
