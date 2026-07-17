#pragma once

#include <cstdint>
#include <string>

#include "renderer2d/inc/utils/gl_commons.hpp"
#include "renderer2d/inc/utils/bufferLayout.hpp"

namespace R2D
{
	class Buffer
	{
	public:
		Buffer() = default;

		// Ctors that call corresponding alloc function
		Buffer(BUFFER_USAGE usage, uint64_t bytes, void const* data = nullptr);
		Buffer(BUFFER_USAGE usage, uint64_t count, INDEX_TYPE indexType, void const* data = nullptr);
		Buffer(BUFFER_USAGE usage, uint64_t count, uint64_t stride, void const* data = nullptr);
		Buffer(BUFFER_USAGE usage, uint64_t count, BufferLayout const& layout, void const* data = nullptr);

		// Base function to allocate buffer memory
		bool alloc(BUFFER_USAGE usage, uint64_t bytes, void const* data = nullptr);

		// Allocate index buffer based on index type
		bool alloc(BUFFER_USAGE usage, uint64_t count, INDEX_TYPE indexType, void const* data = nullptr);

		// Allocate generic buffer based on custom data with stride and count
		bool alloc(BUFFER_USAGE usage, uint64_t count, uint64_t stride, void const* data = nullptr);

		// Allocate buffer with layout (vertex, instance, uniform, ...)
		bool alloc(BUFFER_USAGE usage, uint64_t count, BufferLayout const& layout, void const* data = nullptr);

		template<typename T>
		bool alloc(BUFFER_USAGE usage, std::vector<T> const& dataVec)
		{
			return alloc(usage, dataVec.size(), sizeof(T), dataVec.data());
		}

		// Base function to write data to buffer
		bool update(void const* data, uint64_t bytes, uint64_t offset = 0);

		// Clears entire buffer with 0
		bool clear() const;

		bool clear(
			TEXTURE_FORMAT internalFormat,
			ATTRIBUTE_TYPE typeIn,
			void const* data = nullptr
		) const;

		// Write data to buffer based on count and offset (if any). REQUIRES STRIDE FROM LAYOUT/INDEX
		bool updateElement(void const* data, uint32_t count, uint32_t offsetCount = 0);

		// Write data to element in buffer based on element index and attrib index in layout
		// REQUIRES LAYOUT, ONLY ABLE TO UPDATE ENTIRE ATTRIBUTES
		// (e.g. Entire VEC4 and not a member of VEC4)
		bool updateElementField(void const* data, uint32_t index, uint32_t attribIndex);

		// Read from buffer
		bool read(void* dstPtr, size_t size) const;

		// Set buffer label (useful in renderdoc)
		bool setLabel(char const* label);

		// Bind buffer to render target
		bool bindAsIndirect() const;
		bool bindAsUniform(uint32_t index = 0) const;
		bool bindAsSSB(uint32_t index = 0);
		bool bindAsSSB(uint32_t offset, std::size_t size, uint32_t index = 0);
		bool bindAsPixelPackBuffer(void) noexcept;

		// Unbind buffer
		bool unbindAsSSB() const;

		// Free memory
		bool destroy();

		bool       isValid(void) const noexcept;
		INDEX_TYPE getIndexType(void) const noexcept;

		uint32_t size() const noexcept;
		uint64_t sizeBytes() const noexcept;
		uint32_t stride() const noexcept;

	private:
		friend class DashGL;
		friend class VertexArray;

		//OpenGL Handle
		GLuint      m_buffer{};
		std::string m_label{};

		// Used by all buffer "types"
		uint64_t     m_size{};
		uint32_t     m_stride{};
		uint32_t     m_count{};
		BUFFER_USAGE m_usage;

		// Used by index buffer only
		INDEX_TYPE m_indexType;

		// Used by vertex/instance buffer
		BufferLayout m_layout;

		// To avoid accidentally reallocating memory to a new buffer handle
		bool m_initialised{ false };

		// Handle for internal use with VAO
		BufferHandle m_handle{};

		uint32_t m_boundSSBIndex;
	};
}
