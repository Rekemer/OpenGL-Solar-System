#pragma once
#include <iostream>

#include "glew.h"
// debugging macros
#define ASSERT(x) if (!x) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x,__FILE__,__LINE__))
bool GLLogCall(const char* function, const char* file, int line);
void GLClearError();
