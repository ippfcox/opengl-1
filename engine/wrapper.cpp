#include <unordered_map>
#include "assert.h"
#include "wrapper.hpp"

struct error_message
{
    std::string string;
};

const std::unordered_map<GLenum, error_message> error_map{
    {GL_INVALID_ENUM, {"invalid enum"}},
    {GL_INVALID_VALUE, {"invalid value"}},
    {GL_INVALID_OPERATION, {"invalid operation"}},
    {GL_OUT_OF_MEMORY, {"out of memory"}},
};

void gl_check_error()
{
    auto code = glGetError();
    if (code == GL_NO_ERROR)
        return;

    if (error_map.contains(code))
        SPDLOG_ERROR("gl call failed: {}({:#x})", error_map.at(code).string, code);
    else
        SPDLOG_ERROR("gl call failed, unknown error {:#x}", code);
    assert(code == GL_NO_ERROR);
}