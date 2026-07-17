#pragma once

#include "renderer2d/inc/utils/gl_commons.hpp"
#include "renderer2d/inc/utils/bufferLayout.hpp"
#include "renderer2d/inc/objects/buffer.hpp"

#include <vector>

namespace R2D
{
	class VertexArray
	{
	public:
		using InstanceDivisor = std::pair<Buffer const*, uint32_t>;

		// Automatic building
		VertexArray& setVertexBuffers(std::initializer_list<Buffer const*> buffers);
		VertexArray& setIndexBuffer(Buffer const* buffer);
		VertexArray& setInstanceBuffers(std::initializer_list<Buffer const*> buffers);
		VertexArray& setInstanceBuffers(std::initializer_list<InstanceDivisor> buffers);
		bool         build();

		bool bind() const;

		bool reset();

		// Does the same thing as reset except it doesn't clear the buffers. This is so that we can store mesh vertex/index buffers
		// without having to reset instance buffers.
		bool deleteVertexArray();

		INDEX_TYPE GetIndexType(void) const noexcept;

		bool isValid() const;
	private:
		struct Buffers
		{
			std::vector<Buffer const*>   vertexBuffers;
			Buffer const* indexBuffer{ nullptr };
			std::vector<InstanceDivisor> instanceBuffers;
		};

		GLuint  m_vertexArray;
		bool    m_initialised{ false };
		Buffers m_buffers;
	};
}