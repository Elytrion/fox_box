#pragma once
#include "../../logger.hpp"
#include <glad/glad.h>


inline char const* gl_error_string(GLenum error) noexcept
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";

    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";

    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";

    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";

    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";

#ifdef GL_CONTEXT_LOST
    case GL_CONTEXT_LOST:
        return "GL_CONTEXT_LOST";
#endif

    default:
        return "GL_UNKNOWN_ERROR";
    }
}

inline void log_gl_errors(
    char const* operation,
    char const* file,
    int line) noexcept
{
    GLenum error = GL_NO_ERROR;

    while ((error = glGetError()) != GL_NO_ERROR)
    {
        LOG_ERROR(
            "{}:{} - OpenGL error after {}: {} ({})",
            file,
            line,
            operation,
            gl_error_string(error),
            static_cast<unsigned int>(error));
    }
}

#define LOG_GL_ERRORS(operation)                                      \
    do                                                                \
    {                                                                 \
        ::log_gl_errors((operation), __FILE__, __LINE__);             \
    } while (false)


[[nodiscard]] inline bool has_gl_errors(
    char const* operation,
    char const* file,
    int line) noexcept
{
    bool hasErrors = false;
    GLenum error = GL_NO_ERROR;

    while ((error = glGetError()) != GL_NO_ERROR)
    {
        hasErrors = true;

        LOG_ERROR(
            "{}:{} - OpenGL error after {}: {} ({})",
            file,
            line,
            operation,
            gl_error_string(error),
            static_cast<unsigned int>(error));
    }

    return hasErrors;
}

#define HAS_GL_ERROR(operation)                                      \
    (::has_gl_errors((operation), __FILE__, __LINE__))
