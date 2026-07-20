#include "renderer2D/inc/renderer2D.h"
#include "renderer2D/inc/objects/framebuffer.hpp"
#include "renderer2D/inc/utils/gl_logger.hpp"

namespace R2D
{
	void FrameBuffer::alloc()
	{
		glCreateFramebuffers(1, &m_frameBuffer);

		m_valid = !HAS_GL_ERROR("FRAME BUFFER CREATE EMPTY");
	}

	void FrameBuffer::alloc(std::vector<Texture const*> textures)
	{
		// Check with width and height for all textures. If not all same, cannot create framebuffer
		uint32_t w {0}, h {0};
		for (auto const& texture : textures)
		{
			if (w == 0u)
			{
				w = texture->getWidth();
				h = texture->getHeight();
			}
			else
			{
				if (w != texture->getWidth() || h != texture->getHeight())
				{
					LOG_ERROR("FrameBuffer::alloc: Not all textures are the same size.");
					return;
				}
			}
		}

		alloc();
		GLenum              colourCounter = GL_COLOR_ATTACHMENT0;
		std::vector<GLenum> colorAttachments;
		for (auto const& texture : textures)
		{
			GLenum attachmentTarget;
			if (texture->m_format == TEXTURE_FORMAT::DEPTH32F || texture->m_format == TEXTURE_FORMAT::DEPTH24)
			{
				attachmentTarget = GL_DEPTH_ATTACHMENT;
				//m_depth = *texture;
			}
			else
			{
				attachmentTarget = colourCounter++;
				colorAttachments.push_back(attachmentTarget);
			}

			glNamedFramebufferTexture(m_frameBuffer, attachmentTarget, texture->m_texture, 0);
		}

		//Attach color attachments as draw buffers for the framebuffer
		glNamedFramebufferDrawBuffers(m_frameBuffer, colorAttachments.size(), colorAttachments.data());

		m_width  = w;
		m_height = h;

		m_valid = true;
	}

	void FrameBuffer::alloc(uint32_t width, uint32_t height, TEXTURE_FORMAT format, bool depth)
	{
		m_texture.alloc(width, height, format);
		if (depth)
		{
			m_depth.alloc(width, height, TEXTURE_FORMAT::DEPTH24);
			alloc({&m_texture, &m_depth});
		}
		else
		{
			alloc({&m_texture});
		}
	}

	void FrameBuffer::bind(void) const
	{
		// bind the framebuffer 
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		LOG_GL_ERRORS("FrameBuffer::bind():: glBindFramebuffer");

		// set viewport for rendering
		glViewport(0, 0, m_width, m_height);
	}

	void FrameBuffer::destroy(void)
	{
		if (m_texture.isValid())
		{
			m_texture.destroy();
		}

		if (m_depth.isValid())
		{
			m_depth.destroy();
		}

		glDeleteFramebuffers(1, &m_frameBuffer);
		LOG_GL_ERRORS("FrameBuffer::destroy(): glDeleteFramebuffers");
	}

	void FrameBuffer::setClearColor(float r, float g, float b, float a) noexcept
	{
		m_clearColour = glm::vec4(r, g, b, a);
	}

	void FrameBuffer::setClearDepth(float value) noexcept
	{
		m_clearDepth = value;
	}

	void FrameBuffer::clear(bool withDepth) noexcept
	{
		bind();

		glClearColor(m_clearColour.x, m_clearColour.y, m_clearColour.z, m_clearColour.w);
		glClearDepthf(m_clearDepth);

		auto clearMask = GL_COLOR_BUFFER_BIT;
		if (withDepth)
		{
			clearMask |= GL_DEPTH_BUFFER_BIT;
		}
		glClear(clearMask);
	}

	void FrameBuffer::clear(float value, int attachment)
	{
		glClearNamedFramebufferfv(m_frameBuffer, GL_COLOR, attachment, &value);
		LOG_GL_ERRORS("FrameBuffer::setClearColor():: glClearNamedFramebufferfv");
	}

	void FrameBuffer::bindColourAttachmentTexture(Texture const& texIn, int attachmentID)
	{
		glNamedFramebufferTexture(m_frameBuffer, GL_COLOR_ATTACHMENT0 + attachmentID, texIn.getTextureID(), 0);
		LOG_GL_ERRORS("FRAME BUFFER BIND COLOUR ATTACHMENT TEXTURE");
	}

	void FrameBuffer::blitIn(
		FrameBuffer const& frameBufferIn,
		GLbitfield         mask,
		GLenum             filter
	)
	{
		glBlitNamedFramebuffer(
			frameBufferIn.m_frameBuffer,
			m_frameBuffer,
			0,
			0,
			frameBufferIn.m_width,
			frameBufferIn.m_height,
			0,
			0,
			m_width,
			m_height,
			mask,
			filter
		);
		HAS_GL_ERROR("FRAME BUFFER BLIT");
	}

	bool FrameBuffer::checkComplete()
	{
		if (glCheckNamedFramebufferStatus(m_frameBuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		return true;
	}

	Texture& FrameBuffer::getTextureRef()
	{
		return m_texture;
	}

	Texture const* FrameBuffer::getTexture() const noexcept
	{
		return &m_texture;
	}

	GLuint FrameBuffer::getID() const noexcept
	{
		return m_frameBuffer;
	}

	TextureData FrameBuffer::readTexture() const
	{
		TextureData result;

		result.readIn(m_texture);

		return result;
	}

	TextureData FrameBuffer::readDepthTexture() const
	{
		TextureData result;

		if (m_depth.isValid())
		{
			GLint currentFbo = 0;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);

			// Bind self as TextureData uses glReadPixels to read depth buffer
			// to support 24bit depth component
			bind();
			result.readIn(m_depth);

			glBindFramebuffer(GL_FRAMEBUFFER, currentFbo);
		}

		return result;
	}

	uint32_t FrameBuffer::getWidth() const
	{
		return m_width;
	}

	uint32_t FrameBuffer::getHeight() const
	{
		return m_height;
	}
}