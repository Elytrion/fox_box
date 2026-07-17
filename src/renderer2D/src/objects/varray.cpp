#include "renderer2d/inc/objects/varray.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"

namespace R2D
{
	VertexArray& VertexArray::setVertexBuffers(std::initializer_list<Buffer const*> buffers)
	{
		m_buffers.vertexBuffers.clear();
		m_buffers.vertexBuffers.insert(m_buffers.vertexBuffers.end(), buffers.begin(), buffers.end());
		return *this;
	}

	VertexArray& VertexArray::setInstanceBuffers(std::initializer_list<Buffer const*> buffers)
	{
		m_buffers.instanceBuffers.clear();
		for (auto const& buffer : buffers)
		{
			m_buffers.instanceBuffers.emplace_back(buffer, 1);
		}
		return *this;
	}

	VertexArray& VertexArray::setInstanceBuffers(std::initializer_list<InstanceDivisor> buffers)
	{
		m_buffers.instanceBuffers.clear();
		m_buffers.instanceBuffers.insert(m_buffers.instanceBuffers.end(), buffers.begin(), buffers.end());
		return *this;
	}

	VertexArray& VertexArray::setIndexBuffer(Buffer const* buffer)
	{
		m_buffers.indexBuffer = buffer;
		return *this;
	}

	bool VertexArray::build()
	{
		if (m_initialised)
		{
			LOG_ERROR("Vertex array not initialized");
			return false;
		}

		auto const* indexBuffer{ m_buffers.indexBuffer };

		glCreateVertexArrays(1, &m_vertexArray);
		if (HAS_GL_ERROR("VAO GEN ERROR"))
			return false;

		// Used for vertex attribute attribIndex
		int bufferBindingIndex{ 0 };
		GLuint attribIndex = 0;

		// Bind VBOs
		if (!m_buffers.vertexBuffers.empty())
		{
			for (auto& buffer : m_buffers.vertexBuffers)
			{
				glVertexArrayVertexBuffer(m_vertexArray, bufferBindingIndex, buffer->m_buffer, 0, buffer->m_layout.byteStride);
				if (HAS_GL_ERROR("VAO BIND VBO ERROR"))
					return false;

				uint32_t offset{ 0 };

				auto const& vertexLayout{ buffer->m_layout };
				for (auto const& vertexAttrib : vertexLayout.attributes)
				{
					// Loop through buffer vertexLayout and set stride and types for each vertex attribute
					glVertexArrayAttribFormat(
						m_vertexArray,
						attribIndex,
						vertexAttrib.count,
						static_cast<GLenum>(vertexAttrib.type),
						vertexAttrib.normalised,
						offset
					);
					glVertexArrayAttribBinding(m_vertexArray, attribIndex, bufferBindingIndex);
					if (HAS_GL_ERROR("VAO VERTEX ATTRIB POINTER ERROR"))
						return false;
					glEnableVertexArrayAttrib(m_vertexArray, attribIndex);
					if (HAS_GL_ERROR("VAO VERTEX ATTRIB ENABLE ERROR"))
						return false;
					++attribIndex;
					offset += vertexAttrib.stride;
				}
				++bufferBindingIndex;
			}
		}

		// Bind EBO
		if (indexBuffer)
		{
			glVertexArrayElementBuffer(m_vertexArray, indexBuffer->m_buffer);
			if (HAS_GL_ERROR("VAO BIND EBO ERROR"))
				return false;
		}

		// Bind IBOs
		if (!m_buffers.instanceBuffers.empty())
		{
			for (auto& [buffer, divisor] : m_buffers.instanceBuffers)
			{
				glVertexArrayVertexBuffer(m_vertexArray, bufferBindingIndex, buffer->m_buffer, 0, buffer->m_layout.byteStride);
				if (HAS_GL_ERROR("VAO BIND VBO ERROR"))
					return false;

				auto const& instanceLayout{ buffer->m_layout };
				uint32_t offset = 0;
				for (auto const& vertexAttrib : instanceLayout.attributes)
				{
					// Loop through buffer vertexLayout and set stride and types for each vertex attribute per instance
					if (vertexAttrib.type == ATTRIBUTE_TYPE::FLOAT)
					{
						glVertexArrayAttribFormat(
							m_vertexArray,
							attribIndex,
							vertexAttrib.count,
							static_cast<GLenum>(vertexAttrib.type),
							vertexAttrib.normalised,
							offset
						);
					}
					else
					{
						glVertexArrayAttribIFormat(
							m_vertexArray,
							attribIndex,
							vertexAttrib.count,
							static_cast<GLenum>(vertexAttrib.type),
							offset
						);
					}
					glVertexArrayAttribBinding(m_vertexArray, attribIndex, bufferBindingIndex);
					if (HAS_GL_ERROR("VAO INSTANCE ATTRIB POINTER ERROR"))
						return false;
					glEnableVertexArrayAttrib(m_vertexArray, attribIndex);
					if (HAS_GL_ERROR("VAO VERTEX ATTRIB ENABLE ERROR"))
						return false;
					++attribIndex;
					offset += vertexAttrib.stride;
				}
				glVertexArrayBindingDivisor(m_vertexArray, bufferBindingIndex, divisor);
				if (HAS_GL_ERROR("VAO INSTANCE ATTRIB DIVISOR ERROR"))
					return false;
				++bufferBindingIndex;
			}
		}

		glBindVertexArray(0);

		m_initialised = true;
		return true;
	}

	bool VertexArray::bind() const
	{
		if (!m_initialised)
		{
			LOG_ERROR("VAO NOT BUILT BEFORE BINDING");
			return false;
		}
		glBindVertexArray(m_vertexArray);
		if (HAS_GL_ERROR("VAO BIND ERROR"))
			return false;

		return true;
	}

	bool VertexArray::reset()
	{
		m_initialised = false;
		glDeleteVertexArrays(1, &m_vertexArray);
		if (HAS_GL_ERROR("VAO UNABLE TO DELETE"))
			return false;

		m_buffers.vertexBuffers.clear();
		m_buffers.indexBuffer = nullptr;
		m_buffers.instanceBuffers.clear();

		return true;
	}

	bool VertexArray::deleteVertexArray()
	{
		m_initialised = false;
		glDeleteVertexArrays(1, &m_vertexArray);
		if (HAS_GL_ERROR("VAO UNABLE TO DELETE"))
			return false;

		return true;
	}

	INDEX_TYPE VertexArray::GetIndexType(void) const noexcept
	{
		return m_buffers.indexBuffer->getIndexType();
	}

	bool VertexArray::isValid() const
	{
		return m_initialised;
	}
}
