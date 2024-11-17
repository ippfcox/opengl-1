#pragma once

#include "common.hpp"

void gl_check_error(const char *file, const char *func, int line);

// template <typename Func, typename... Args>
// void gl_call(Func &&func, Args &&...args)
// {
//     func(std::forward<Args>(args)...);
//     gl_check_error();
// }

#ifdef DEBUG
#define GL_CALL(call) \
    call;             \
    gl_check_error(__FILE__, __FUNCTION__, __LINE__)
#else
#define GL_CALL(call) call
#endif
