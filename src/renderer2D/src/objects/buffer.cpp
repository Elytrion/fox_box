#include "renderer2d/inc/objects/buffer.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"
#include "renderer2D/inc/renderer2D.h"

namespace R2D
{


	bool Buffer::destroy()
	{
		if (m_initialised)
		{
			glDeleteBuffers(1, &m_buffer);
			if (HAS_GL_ERROR("DELETE BUFFER ERROR"))
				return false;
		}

		m_size = 0;
		m_count = 0;
		m_stride = 0;
		m_initialised = false;

		return true;
	}

	bool Buffer::isValid(void) const noexcept
	{
		return m_initialised;
	}

	INDEX_TYPE Buffer::getIndexType(void) const noexcept
	{
		return m_indexType;
	}

	uint32_t Buffer::size() const noexcept
	{
		return m_count;
	}

	uint64_t Buffer::sizeBytes() const noexcept
	{
		return m_size;
	}

	uint32_t Buffer::stride() const noexcept
	{
		return m_stride;
	}

	// ReSharper disable once CppPossiblyUninitializedMember
	bool Buffer::setLabel(char const* label)
	{
		m_label = label;
		if (m_initialised)
			glObjectLabel(GL_BUFFER, m_handle, -1, m_label.c_str());
		if (HAS_GL_ERROR("BUFFER SET LABEL ERROR"))
			return false;

		return true;
	}

	Buffer::Buffer(BUFFER_USAGE usage, uint64_t bytes, void const* data)
	{
		alloc(usage, bytes, data);
	}

	Buffer::Buffer(BUFFER_USAGE usage, uint64_t count, INDEX_TYPE indexType, void const* data)
	{
		alloc(usage, count, indexType, data);
	}

	Buffer::Buffer(BUFFER_USAGE usage, uint64_t count, uint64_t stride, void const* data)
	{
		alloc(usage, count, stride, data);
	}

	Buffer::Buffer(BUFFER_USAGE usage, uint64_t count, BufferLayout const& layout, void const* data)
	{
		alloc(usage, count, layout, data);
	}

	bool Buffer::alloc(BUFFER_USAGE usage, uint64_t bytes, void const* data)
	{
		glCreateBuffers(1, &m_buffer);
		if (HAS_GL_ERROR("BUFFER CREATION ERROR"))
			return false;
		if (!m_label.empty())
			glObjectLabel(GL_BUFFER, m_handle, -1, m_label.c_str());

		switch (usage)
		{
		case BUFFER_USAGE::STREAM_DRAW:
		case BUFFER_USAGE::STREAM_READ:
		case BUFFER_USAGE::STREAM_COPY:
		case BUFFER_USAGE::STATIC_DRAW:
		case BUFFER_USAGE::STATIC_READ:
		case BUFFER_USAGE::STATIC_COPY:
		case BUFFER_USAGE::DYNAMIC_DRAW:
		case BUFFER_USAGE::DYNAMIC_READ:
		case BUFFER_USAGE::DYNAMIC_COPY:
			glNamedBufferData(m_buffer, static_cast<GLsizeiptr>(bytes), data, static_cast<GLenum>(usage));
			break;
		case BUFFER_USAGE::DYNAMIC_STORE:
		case BUFFER_USAGE::MAP_READ:
		case BUFFER_USAGE::MAP_WRITE:
		case BUFFER_USAGE::MAP_PERSISTENT:
		case BUFFER_USAGE::MAP_COHERENT:
		case BUFFER_USAGE::CLIENT_STORE:
		case BUFFER_USAGE::MAP_USAGE:
		case BUFFER_USAGE::DYNAMIC_MAP_STORE:
		case BUFFER_USAGE::NONE:
			glNamedBufferStorage(m_buffer, static_cast<GLsizeiptr>(bytes), data, static_cast<GLbitfield>(usage));
			break;
		default:;
		}

		if (HAS_GL_ERROR("BUFFER ALLOC ERROR"))
			return false;

		glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);

		m_size = bytes;
		m_usage = usage;

		m_initialised = true;

		m_handle = Renderer2D::registerBuffer();
		return true;
	}

	bool Buffer::alloc(BUFFER_USAGE usage, uint64_t count, INDEX_TYPE indexType, const void* data)
	{
		uint64_t stride;
		switch (indexType)
		{
		case INDEX_TYPE::UINT8:
			stride = 1;
			break;
		case INDEX_TYPE::UINT16:
			stride = 2;
			break;
		case INDEX_TYPE::UINT32:
			stride = 4;
			break;
		}

		if (!alloc(usage, count * stride, data))
			return false;

		m_stride = stride;
		m_count = count;
		m_indexType = indexType;

		return true;
	}

	bool Buffer::alloc(BUFFER_USAGE usage, uint64_t count, uint64_t stride, void const* data)
	{
		if (!alloc(usage, count * stride, data))
			return false;

		m_stride = stride;
		m_count = count;

		return true;
	}

	bool Buffer::alloc(BUFFER_USAGE usage, uint64_t count, BufferLayout const& layout, void const* data)
	{
		if (!alloc(usage, static_cast<uint64_t>(count) * layout.byteStride, data))
			return false;

		m_layout = layout;
		m_stride = layout.byteStride;
		m_count = count;

		return true;
	}

	bool Buffer::update(void const* data, uint64_t bytes, uint64_t offset)
	{
		if (!m_initialised)
		{
			LOG_ERROR("BUFFER IS NOT INITIALISED BEFORE UPDATE WAS CALLED");
			return false;
		}
		glNamedBufferSubData(m_buffer, offset, bytes, data);
		if (HAS_GL_ERROR("BUFFER SUB DATA ERROR"))
			return false;

		glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
		return true;
	}

	bool Buffer::clear() const
	{
		if (!m_initialised)
		{
			LOG_ERROR("BUFFER IS NOT INITIALISED BEFORE CLEAR WAS CALLED");
			return false;
		}

		glClearNamedBufferData(
			m_buffer,
			GL_R8,
			GL_RED,
			GL_BYTE,
			nullptr
		);
		if (HAS_GL_ERROR("BUFFER ClEAR DATA ERROR"))
			return false;

		return true;
	}

	bool Buffer::clear(TEXTURE_FORMAT internalFormat, ATTRIBUTE_TYPE typeIn, void const* data) const
	{
		if (!m_initialised)
		{
			LOG_ERROR("BUFFER IS NOT INITIALISED BEFORE CLEAR WAS CALLED");
			return false;
		}

		GLenum formatIn = GL_RED;

		switch (internalFormat)
		{
		case TEXTURE_FORMAT::R16F:
		case TEXTURE_FORMAT::R32F:
		case TEXTURE_FORMAT::R32U:
		case TEXTURE_FORMAT::R32I:
			formatIn = GL_RED;
			break;
		case TEXTURE_FORMAT::RGB8:
			formatIn = GL_RGB;
			break;
		case TEXTURE_FORMAT::RGBA8:
		case TEXTURE_FORMAT::RGBA16F:
		case TEXTURE_FORMAT::RGBA32F:
			formatIn = GL_RGBA;
			break;
		case TEXTURE_FORMAT::DEPTH32F:
			formatIn = GL_DEPTH_COMPONENT;
			break;
		}

		glClearNamedBufferData(
			m_buffer,
			static_cast<GLenum>(internalFormat),
			formatIn,
			static_cast<GLenum>(typeIn),
			data
		);
		if (HAS_GL_ERROR("BUFFER CLEAR DATA ERROR"))
			return false;

		return true;
	}

	bool Buffer::updateElement(const void* data, uint32_t count, uint32_t offsetCount)
	{
		return update(data, static_cast<uint64_t>(count) * m_stride, static_cast<uint64_t>(offsetCount) * m_stride);
	}

	bool Buffer::updateElementField(const void* data, uint32_t index, uint32_t attribIndex)
	{
		if (m_layout.byteStride == 0)
		{
			LOG_ERROR("BUFFER DOES NOT HAVE LAYOUT FOR updateElementField()");
			return false;
		}

		auto const& attributes{ m_layout.attributes };
		uint32_t    attribOffset{ 0 };
		for (auto i{ 0 }; i < attribIndex; ++i)
		{
			attribOffset += attributes[i].stride;
		}

		return update(data, attributes[attribIndex].stride, index * m_stride + attribOffset);
	}

	bool Buffer::read(void* dstPtr, size_t size) const
	{
		glGetNamedBufferSubData(m_buffer, 0, static_cast<khronos_ssize_t>(size), dstPtr);
		if (HAS_GL_ERROR("BUFFER READ BACK ERROR"))
			return false;
		return true;
	}

	bool Buffer::bindAsUniform(uint32_t index) const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, index, m_buffer);
		if (HAS_GL_ERROR("BUFFER BIND BUFFER ERROR"))
			return false;
		return true;
	}

	bool Buffer::bindAsSSB(uint32_t index)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_buffer);
		if (HAS_GL_ERROR("BUFFER SSBO BIND ERROR"))
			return false;

		m_boundSSBIndex = index;
		return true;
	}

	bool Buffer::bindAsSSB(uint32_t offset, std::size_t size, uint32_t index)
	{
		glBindBufferRange(
			GL_SHADER_STORAGE_BUFFER,
			index,
			m_buffer,
			offset,
			size
		);
		if (HAS_GL_ERROR("BUFFER SSBO RANGE BIND ERROR"))
			return false;

		m_boundSSBIndex = index;
		return true;
	}

	bool Buffer::bindAsPixelPackBuffer(void) noexcept
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer);
		return true;
	}

	bool Buffer::unbindAsSSB() const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_boundSSBIndex, 0);
		if (HAS_GL_ERROR("BUFFER SSBO UNBIND ERROR"))
			return false;
		return true;
	}

	bool Buffer::bindAsIndirect() const
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_buffer);
		if (HAS_GL_ERROR("INDIRECT BUFFER BIND ERROR"))
			return false;
		return true;
	}
}
