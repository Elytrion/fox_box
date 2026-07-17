#include "renderer2d/inc/objects/shaderProgram.hpp"
#include "renderer2d/inc/utils/shaderManager.hpp"
#include "renderer2d/inc/utils/gl_logger.hpp"

namespace R2D
{
	void ShaderProgram::setProgram(SHADER_PROGRAM_VALUE program)
	{
		m_programHandle = ShaderManager::getShaderProgram(program);
		m_uniformLocations.clear();
		m_initialised = true;
	}

	void ShaderProgram::setUniform(
		const std::string& uniformName, UNIFORM_TYPE type, const void* data)
	{
		if (!m_initialised)
		{
			LOG_ERROR("SHADER PROGRAM NOT SET BEFORE SET UNIFORM");
			return;
		}
		GLint location;
		// Find for cached uniform location
		try
		{
			location = m_uniformLocations.at(uniformName);
		}
		catch (std::exception const& e)
		{
			location = glGetUniformLocation(m_programHandle, uniformName.c_str());
			LOG_GL_ERRORS("SHADER GET UNIFORM LOCATION ERROR");
			if (location == -1)
			{
				LOG_ERROR("UNIFORM NOT FOUND: {}", uniformName);
			}
			m_uniformLocations.insert({ uniformName, location });
		}

		// Set uniform
		updateUniform(location, type, data);
	}

	void ShaderProgram::use()
	{
		if (!m_initialised)
		{
			LOG_ERROR("SHADER PROGRAM NOT SET BEFORE USE");
			return;
		}
		// set shader program
		glUseProgram(m_programHandle);
		LOG_GL_ERRORS("SHADER USE ERROR");
	}

	void ShaderProgram::destroy()
	{
		m_programHandle = 0;
		m_initialised = false;
	}

	bool ShaderProgram::isSet() const
	{
		return m_initialised;
	}

	GLuint ShaderProgram::getHandle() const
	{
		return m_programHandle;
	}

	void ShaderProgram::updateUniform(GLint location, UNIFORM_TYPE type, const void* data)
	{
		switch (type)
		{
		case UNIFORM_TYPE::FLOAT:
			glProgramUniform1f(m_programHandle, location, *reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::INT:
		case UNIFORM_TYPE::SAMPLER:
			glProgramUniform1i(m_programHandle, location, *reinterpret_cast<GLint const*>(data));
			break;
		case UNIFORM_TYPE::UINT:
			glProgramUniform1ui(m_programHandle, location, *reinterpret_cast<GLuint const*>(data));
			break;
		case UNIFORM_TYPE::VEC2:
			glProgramUniform2fv(m_programHandle, location, 1, reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::VEC3:
			glProgramUniform3fv(m_programHandle, location, 1, reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::VEC4:
			glProgramUniform4fv(m_programHandle, location, 1, reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::IVEC2:
			glProgramUniform2iv(m_programHandle, location, 1, reinterpret_cast<GLint const*>(data));
			break;
		case UNIFORM_TYPE::IVEC3:
			glProgramUniform3iv(m_programHandle, location, 1, reinterpret_cast<GLint const*>(data));
			break;
		case UNIFORM_TYPE::IVEC4:
			glProgramUniform4iv(m_programHandle, location, 1, reinterpret_cast<GLint const*>(data));
			break;
		case UNIFORM_TYPE::UVEC2:
			glProgramUniform2uiv(m_programHandle, location, 1, reinterpret_cast<GLuint const*>(data));
			break;
		case UNIFORM_TYPE::UVEC3:
			glProgramUniform3uiv(m_programHandle, location, 1, reinterpret_cast<GLuint const*>(data));
			break;
		case UNIFORM_TYPE::UVEC4:
			glProgramUniform4uiv(m_programHandle, location, 1, reinterpret_cast<GLuint const*>(data));
			break;
		case UNIFORM_TYPE::MAT2:
			glProgramUniformMatrix2fv(m_programHandle, location, 1, GL_FALSE, reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::MAT3:
			glProgramUniformMatrix3fv(m_programHandle, location, 1, GL_FALSE, reinterpret_cast<GLfloat const*>(data));
			break;
		case UNIFORM_TYPE::MAT4:
			glProgramUniformMatrix4fv(m_programHandle, location, 1, GL_FALSE, reinterpret_cast<GLfloat const*>(data));
			break;
		}

		LOG_GL_ERRORS("SHADER SET UNIFORM ERROR");
	}
}