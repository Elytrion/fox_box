#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "renderer2d/inc/utils/gl_commons.hpp"

namespace R2D
{
	class ProgramReflected;

	class ShaderProgram
	{
	public:
		// Function that takes in desired program and gets program handle
		// if program is linked, if not will create and link program
		// Makes calls to global shader linker
		void setProgram(SHADER_PROGRAM_VALUE program);

		void setUniform(std::string const& uniformName, UNIFORM_TYPE type, void const* data);

		void use();
		void destroy();

		bool isSet() const;

		GLuint getHandle() const;

	private:
		void updateUniform(GLint location, UNIFORM_TYPE type, void const* data);

	private:
		friend class Renderer;

		struct UpdateOrder
		{
			std::string  name;
			UNIFORM_TYPE type;
			void const* data;
		};

		GLuint m_programHandle;

		std::vector<UpdateOrder> m_orderQueue;
		std::unordered_map
			<std::string, GLint> m_uniformLocations;

		bool m_initialised{ false };
	};


	using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
}
