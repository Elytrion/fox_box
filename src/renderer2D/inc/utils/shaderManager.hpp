#pragma once

#include "renderer2d/inc/utils/gl_commons.hpp"

#include <vector>
#include <unordered_map>

// NOTE: When initialising renderer system, it is best to create shader programs
// 				based on user defined enum class inherited from SHADER_PROGRAM_VALUE
namespace R2D
{
	class ShaderManager
	{
	public:
		static SHADER_PROGRAM_VALUE createShaderProgram(
			char const** vsSource,
			char const** fsSource,
			int          vsCount = 1,
			int          fsCount = 1
		);

		static GLint getShaderProgram(SHADER_PROGRAM_VALUE program);

		static void cleanup();

	private:
		static GLuint getCompiledShader(char const** source, GLenum shaderType, int count);
		static GLuint getLinkedProgram(GLuint vs, GLuint fs);
		static GLuint getLinkedProgram(GLuint cs);

	private:
		// Shader Programs
		static std::unordered_map<SHADER_PROGRAM_VALUE, GLint> m_programMap;
		// store compiled shaders, so that reused shaders are not compiled again 
		static std::unordered_map<const char**, GLuint> m_compiledShaders;

		static SHADER_PROGRAM_VALUE m_counter;
	};
}
