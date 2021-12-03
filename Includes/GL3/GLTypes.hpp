#ifndef GL_TYPES_HPP
#define GL_TYPES_HPP

#include <KHR/khrplatform.h>
using GLenum = unsigned int;
using GLboolean = unsigned char;
using GLbitfield = unsigned int;
using GLvoid = void;
using GLbyte = khronos_int8_t;
using GLubyte = khronos_uint8_t;
using GLshort = khronos_int16_t;
using GLushort = khronos_uint16_t;
using GLint = int;
using GLuint = unsigned int;
using GLclampx = khronos_int32_t;
using GLsizei = int;
using GLfloat = khronos_float_t;
using GLclampf = khronos_float_t;
using GLdouble = double;
using GLclampd = double;
using GLeglClientBufferEXT = void*;
using GLeglImageOES = void*;
using GLchar = char;
using GLcharARB = char;
#ifdef __APPLE__
using GLhandleARB = void*;
#else
using GLhandleARB = unsigned int;
#endif
using GLhalf = khronos_uint16_t;
using GLhalfARB = khronos_uint16_t;
using GLfixed = khronos_int32_t;
using GLintptr = khronos_intptr_t;
using GLintptrARB = khronos_intptr_t;
using GLsizeiptr = khronos_ssize_t;
using GLsizeiptrARB = khronos_ssize_t;
using GLint64 = khronos_int64_t;
using GLint64EXT = khronos_int64_t;
using GLuint64 = khronos_uint64_t;
using GLuint64EXT = khronos_uint64_t;
using GLsync = struct __GLsync*;
using GLhalfNV = unsigned short;
using GLvdpauSurfaceNV = GLintptr;

#endif  //! end of GLTypes.hpp