#pragma once
#include <cstdint>
#include <vector>

#include "renderer2d/inc/utils/gl_commons.hpp"
#include "renderer2d/inc/objects/texture.hpp"

namespace R2D
{
	class TextureData
	{
	public:
		bool isValid() const;

		/*
		 * If passing in depth texture that are using 24bit components,
		 * it must be done from a framebuffer as 24bit depth components need
		 * to be read from framebuffers.
		 */
		void readIn(Texture const& textureIn);

		uint8_t const readPixel(int x, int y) const;

		uint8_t const* getPtr(int x = 0, int y = 0) const;

		template <typename T>
		T const readPixel(int x, int y) const;

		template <typename T>
		T const* getPtr(int x = 0, int y = 0) const;

		uint8_t const* getDataPtr() const;

	private:
		std::vector<uint8_t> m_data;

		uint32_t       m_width;
		uint32_t       m_height;
		uint32_t       m_pixelStride;
		uint32_t       m_pixelCompSize;
		TEXTURE_FORMAT m_format;
		ATTRIBUTE_TYPE m_pixelType;

		bool m_valid{false};
	};

	template <typename T>
	T const TextureData::readPixel(int x, int y) const
	{
		if (!m_valid || m_data.empty())
		{
			return T{};
		}

		return *getPtr<T>(x, y);
	}

	template <typename T>
	T const* TextureData::getPtr(int x, int y) const
	{
		if (!m_valid || m_data.empty())
		{
			return nullptr;
		}

		return reinterpret_cast<T const*>(getPtr(x, y));
	}
}
