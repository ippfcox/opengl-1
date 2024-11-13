#pragma once

#include "common.hpp"

void gl_check_error();

// template <typename Func, typename... Args>
// void gl_call(Func &&func, Args &&...args)
// {
//     func(std::forward<Args>(args)...);
//     gl_check_error();
// }

#ifdef DEBUG
#define GL_CALL(call) \
    call;             \
    gl_check_error()
#else
#define GL_CALL(call) call
#endif
