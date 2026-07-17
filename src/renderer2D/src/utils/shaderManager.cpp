#include "renderer2d/inc/utils/shaderManager.hpp"
#include "../../logger.hpp"

namespace R2D
{
	// defining static variables
	std::unordered_map<SHADER_PROGRAM_VALUE, GLint> ShaderManager::m_programMap;

	std::unordered_map<const char**, GLuint> ShaderManager::m_compiledShaders;

	SHADER_PROGRAM_VALUE ShaderManager::m_counter = 0;

	GLuint ShaderManager::getCompiledShader(const char** source, GLenum shaderType, int count)
	{
		// Return shader if it already exists
		if (m_compiledShaders.count(source) > 0)
			return m_compiledShaders.at(source);

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, count, source, NULL);
		glCompileShader(shader);

		// Shader compile check
		GLint success{ 0 };
		char  log[512]{ 0 };
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, log);
			auto const shaderString{
				shaderType == GL_VERTEX_SHADER
					? "VERTEX"
					: shaderType == GL_FRAGMENT_SHADER
						  ? "FRAGMENT"
						  : "COMPUTE"
			};
			LOG_ERROR("SHADER LOG [{}]: {}", shaderString, log);
		}

		// Cache the compiled shader
		m_compiledShaders.emplace(source, shader);

		return shader;
	}

	GLuint ShaderManager::getLinkedProgram(GLuint vs, GLuint fs)
	{
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vs);
		glAttachShader(shaderProgram, fs);
		glLinkProgram(shaderProgram);

		// Shader link check
		GLint success{ 0 };
		char  log[512]{ 0 };
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, log);
			LOG_ERROR("PROGRAM LINK: {}", log);
		}
		return shaderProgram;
	}

	GLuint ShaderManager::getLinkedProgram(GLuint cs)
	{
		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, cs);
		glLinkProgram(shaderProgram);

		// Shader link check
		GLint success{ 0 };
		char  log[512]{ 0 };
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, log);
			LOG_ERROR("PROGRAM LINK: {}", log);
		}
		return shaderProgram;
	}

	SHADER_PROGRAM_VALUE ShaderManager::createShaderProgram(
		const char** vsSource,
		const char** fsSource,
		int          vsCount,
		int          fsCount
	)
	{
		auto vsProgram{ getCompiledShader(vsSource, GL_VERTEX_SHADER, vsCount) };
		auto fsProgram{ getCompiledShader(fsSource, GL_FRAGMENT_SHADER, fsCount) };

		auto shaderProgram{ getLinkedProgram(vsProgram, fsProgram) };

		m_programMap.try_emplace(m_counter, shaderProgram);
		return m_counter++;
	}

	GLint ShaderManager::getShaderProgram(SHADER_PROGRAM_VALUE program)
	{
		try
		{
			return m_programMap.at(program);
		}
		catch (std::exception const&)
		{
			LOG_ERROR("SHADER PROGRAM NOT YET CREATED: {}", program);
		}

		return 0;
	}

	void ShaderManager::cleanup()
	{
		for (auto const& program : m_programMap)
		{
			glDeleteProgram(program.second);
		}

		for (auto const& shader : m_compiledShaders)
		{
			glDeleteShader(shader.second);
		}

		m_programMap.clear();
		m_compiledShaders.clear();
	}
}
