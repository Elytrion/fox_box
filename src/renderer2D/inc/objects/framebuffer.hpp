#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "renderer2D/inc/utils/gl_commons.hpp"
#include "renderer2D/inc/objects/texture.hpp"
#include "renderer2D/inc/objects/textureData.hpp"

namespace R2D
{
	class FrameBuffer
	{
	public:
		void alloc();
		void alloc(std::vector<Texture const*> textures);
		void alloc(uint32_t width, uint32_t height, TEXTURE_FORMAT format, bool depth = true);
		void bind(void) const;
		void destroy(void);

		void setClearColor(float r, float g, float b, float a) noexcept;
		void setClearDepth(float value) noexcept;

		void clear(bool withDepth = true) noexcept;
		void clear(float value, int attachment);

		void bindColourAttachmentTexture(Texture const& texIn, int attachmentID = 0);

		void blitIn(
			FrameBuffer const& frameBufferIn,
			GLbitfield         mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
			GLenum             filter = GL_NEAREST
		);

		bool checkComplete();

		Texture& getTextureRef();
		Texture const* getTexture() const noexcept;
		GLuint   getID() const noexcept;

		TextureData readTexture() const;
		TextureData readDepthTexture() const;

		uint32_t getWidth() const;
		uint32_t getHeight() const;

	private:
		GLuint m_frameBuffer;

		Texture m_depth;
		// Only used for frame buffers created from texture details
		Texture m_texture;

		uint32_t m_width{};
		uint32_t m_height{};

		bool m_valid{ false };

		glm::vec4 m_clearColour{ 0.f };
		float m_clearDepth{ 1.f };
	};

	using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
}
