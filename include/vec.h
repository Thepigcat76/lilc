#pragma once

#include "numbers.h"
#include <stddef.h>

#define MAKE_VEC2(type)                                                        \
  typedef struct {                                                             \
    type x;                                                                    \
    type y;                                                                    \
  } vec2##type;

#define MAKE_VEC3(type)                                                        \
  typedef struct {                                                             \
    type x;                                                                    \
    type y;                                                                    \
    type z;                                                                    \
  } vec3##type;

/* Vector structs*/

MAKE_VEC2(i8)
MAKE_VEC2(i16)
MAKE_VEC2(i32)
MAKE_VEC2(i64)
MAKE_VEC2(f32)
MAKE_VEC2(f64)

MAKE_VEC3(i8)
MAKE_VEC3(i16)
MAKE_VEC3(i32)
MAKE_VEC3(i64)
MAKE_VEC3(f32)
MAKE_VEC3(f64)

#undef MAKE_VEC2
#undef MAKE_VEC3

typedef vec2f32 vec2f;
typedef vec2i32 vec2i;

typedef vec3f32 vec3f;
typedef vec3i32 vec3i;

/* Vector making */

#define vec2(_x, _y)                                                           \
  _Generic((_x),                                                               \
      f32: (vec2f32){.x = _x, .y = _y},                                        \
      f64: (vec2f32){.x = _x, .y = _y},                                        \
      i8: (vec2i8){.x = _x, .y = _y},                                          \
      i16: (vec2i16){.x = _x, .y = _y},                                        \
      i32: (vec2i32){.x = _x, .y = _y},                                        \
      i64: (vec2i64){.x = _x, .y = _y},                                        \
      size_t: (vec2i64){.x = _x, .y = _y})

#define vec3(_x, _y, _z)                                                       \
  _Generic((_x),                                                               \
      f32: (vec3f32){.x = _x, .y = _y, .z = _z},                               \
      f64: (vec3f32){.x = _x, .y = _y, .z = _z},                               \
      i8: (vec3i8){.x = _x, .y = _y, .z = _z},                                 \
      i16: (vec3i16){.x = _x, .y = _y, .z = _z},                               \
      i32: (vec3i32){.x = _x, .y = _y, .z = _z},                               \
      i64: (vec3i64){.x = _x, .y = _y, .z = _z},                               \
      size_t: (vec2i64){.x = _x, .y = _y})

/* Vector casting */

#define vec2_cast(_type, ...)                                                  \
  _Generic((__VA_ARGS__),                                                      \
      _type: __VA_ARGS__,                                                      \
      default: (_type){.x = __VA_ARGS__.x, .y = __VA_ARGS__.y})

#define vec3_cast(_type, ...)                                                  \
  _Generic((__VA_ARGS__),                                                      \
      _type: __VA_ARGS__,                                                      \
      default: (_type){                                                        \
          .x = __VA_ARGS__.x, .y = __VA_ARGS__.y, .z = __VA_ARGS__.z})

/* Vector add functions */

#define VEC2_ADD(_type)                                                        \
  vec2##_type _vec2##_type##_add(vec2##_type a, vec2##_type b)
#define VEC3_ADD(_type)                                                        \
  vec3##_type _vec3##_type##_add(vec3##_type a, vec3##_type b)

VEC2_ADD(i8);
VEC2_ADD(i16);
VEC2_ADD(i32);
VEC2_ADD(i64);
VEC2_ADD(f32);
VEC2_ADD(f64);

VEC3_ADD(i8);
VEC3_ADD(i16);
VEC3_ADD(i32);
VEC3_ADD(i64);
VEC3_ADD(f32);
VEC3_ADD(f64);

#undef VEC2_ADD
#undef VEC3_ADD

#define vec2_add(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec2i8: _vec2i8_add(vec2_cast(vec2i8, _a), vec2_cast(vec2i8, _b)),       \
      vec2i16: _vec2i16_add(vec2_cast(vec2i16, _a), vec2_cast(vec2i16, _b)),   \
      vec2i32: _vec2i32_add(vec2_cast(vec2i32, _a), vec2_cast(vec2i32, _b)),   \
      vec2i64: _vec2i64_add(vec2_cast(vec2i64, _a), vec2_cast(vec2i64, _b)),   \
      vec2f32: _vec2f32_add(vec2_cast(vec2f32, _a), vec2_cast(vec2f32, _b)),   \
      vec2f64: _vec2f64_add(vec2_cast(vec2f64, _a), vec2_cast(vec2f64, _b)))

#define vec3_add(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec3i8: _vec3i8_add(vec3_cast(vec3i8, _a), vec3_cast(vec3i8, _b)),       \
      vec3i16: _vec3i16_add(vec3_cast(vec3i16, _a), vec3_cast(vec3i16, _b)),   \
      vec3i32: _vec3i32_add(vec3_cast(vec3i32, _a), vec3_cast(vec3i32, _b)),   \
      vec3i64: _vec3i64_add(vec3_cast(vec3i64, _a), vec3_cast(vec3i64, _b)),   \
      vec3f32: _vec3f32_add(vec3_cast(vec3f32, _a), vec3_cast(vec3f32, _b)),   \
      vec3f64: _vec3f64_add(vec3_cast(vec3f64, _a), vec3_cast(vec3f64, _b)))

/* Vector sub functions */

#define VEC2_SUB(_type)                                                        \
  vec2##_type _vec2##_type##_sub(vec2##_type a, vec2##_type b)
#define VEC3_SUB(_type)                                                        \
  vec3##_type _vec3##_type##_sub(vec3##_type a, vec3##_type b)

VEC2_SUB(i8);
VEC2_SUB(i16);
VEC2_SUB(i32);
VEC2_SUB(i64);
VEC2_SUB(f32);
VEC2_SUB(f64);

VEC3_SUB(i8);
VEC3_SUB(i16);
VEC3_SUB(i32);
VEC3_SUB(i64);
VEC3_SUB(f32);
VEC3_SUB(f64);

#undef VEC2_SUB
#undef VEC3_SUB

#define vec2_sub(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec2i8: _vec2i8_sub(vec2_cast(vec2i8, _a), vec2_cast(vec2i8, _b)),       \
      vec2i16: _vec2i16_sub(vec2_cast(vec2i16, _a), vec2_cast(vec2i16, _b)),   \
      vec2i32: _vec2i32_sub(vec2_cast(vec2i32, _a), vec2_cast(vec2i32, _b)),   \
      vec2i64: _vec2i64_sub(vec2_cast(vec2i64, _a), vec2_cast(vec2i64, _b)),   \
      vec2f32: _vec2f32_sub(vec2_cast(vec2f32, _a), vec2_cast(vec2f32, _b)),   \
      vec2f64: _vec2f64_sub(vec2_cast(vec2f64, _a), vec2_cast(vec2f64, _b)))

#define vec3_sub(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec3i8: _vec3i8_sub(vec3_cast(vec3i8, _a), vec3_cast(vec3i8, _b)),       \
      vec3i16: _vec3i16_sub(vec3_cast(vec3i16, _a), vec3_cast(vec3i16, _b)),   \
      vec3i32: _vec3i32_sub(vec3_cast(vec3i32, _a), vec3_cast(vec3i32, _b)),   \
      vec3i64: _vec3i64_sub(vec3_cast(vec3i64, _a), vec3_cast(vec3i64, _b)),   \
      vec3f32: _vec3f32_sub(vec3_cast(vec3f32, _a), vec3_cast(vec3f32, _b)),   \
      vec3f64: _vec3f64_sub(vec3_cast(vec3f64, _a), vec3_cast(vec3f64, _b)))

/* Vector mul functions */

#define VEC2_MUL(_type)                                                        \
  vec2##_type _vec2##_type##_mul(vec2##_type a, vec2##_type b)
#define VEC3_MUL(_type)                                                        \
  vec3##_type _vec3##_type##_mul(vec3##_type a, vec3##_type b)

VEC2_MUL(i8);
VEC2_MUL(i16);
VEC2_MUL(i32);
VEC2_MUL(i64);
VEC2_MUL(f32);
VEC2_MUL(f64);

VEC3_MUL(i8);
VEC3_MUL(i16);
VEC3_MUL(i32);
VEC3_MUL(i64);
VEC3_MUL(f32);
VEC3_MUL(f64);

#undef VEC2_MUL
#undef VEC3_MUL

#define vec2_mul(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec2i8: _vec2i8_mul(vec2_cast(vec2i8, _a), vec2_cast(vec2i8, _b)),       \
      vec2i16: _vec2i16_mul(vec2_cast(vec2i16, _a), vec2_cast(vec2i16, _b)),   \
      vec2i32: _vec2i32_mul(vec2_cast(vec2i32, _a), vec2_cast(vec2i32, _b)),   \
      vec2i64: _vec2i64_mul(vec2_cast(vec2i64, _a), vec2_cast(vec2i64, _b)),   \
      vec2f32: _vec2f32_mul(vec2_cast(vec2f32, _a), vec2_cast(vec2f32, _b)),   \
      vec2f64: _vec2f64_mul(vec2_cast(vec2f64, _a), vec2_cast(vec2f64, _b)))

#define vec3_mul(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec3i8: _vec3i8_mul(vec3_cast(vec3i8, _a), vec3_cast(vec3i8, _b)),       \
      vec3i16: _vec3i16_mul(vec3_cast(vec3i16, _a), vec3_cast(vec3i16, _b)),   \
      vec3i32: _vec3i32_mul(vec3_cast(vec3i32, _a), vec3_cast(vec3i32, _b)),   \
      vec3i64: _vec3i64_mul(vec3_cast(vec3i64, _a), vec3_cast(vec3i64, _b)),   \
      vec3f32: _vec3f32_mul(vec3_cast(vec3f32, _a), vec3_cast(vec3f32, _b)),   \
      vec3f64: _vec3f64_mul(vec3_cast(vec3f64, _a), vec3_cast(vec3f64, _b)))

/* Vector div functions */

#define VEC2_DIV(_type)                                                        \
  vec2##_type _vec2##_type##_div(vec2##_type a, vec2##_type b)
#define VEC3_DIV(_type)                                                        \
  vec3##_type _vec3##_type##_div(vec3##_type a, vec3##_type b)

VEC2_DIV(i8);
VEC2_DIV(i16);
VEC2_DIV(i32);
VEC2_DIV(i64);
VEC2_DIV(f32);
VEC2_DIV(f64);

VEC3_DIV(i8);
VEC3_DIV(i16);
VEC3_DIV(i32);
VEC3_DIV(i64);
VEC3_DIV(f32);
VEC3_DIV(f64);

#undef VEC2_DIV
#undef VEC3_DIV

#define vec2_div(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec2i8: _vec2i8_div(vec2_cast(vec2i8, _a), vec2_cast(vec2i8, _b)),       \
      vec2i16: _vec2i16_div(vec2_cast(vec2i16, _a), vec2_cast(vec2i16, _b)),   \
      vec2i32: _vec2i32_div(vec2_cast(vec2i32, _a), vec2_cast(vec2i32, _b)),   \
      vec2i64: _vec2i64_div(vec2_cast(vec2i64, _a), vec2_cast(vec2i64, _b)),   \
      vec2f32: _vec2f32_div(vec2_cast(vec2f32, _a), vec2_cast(vec2f32, _b)),   \
      vec2f64: _vec2f64_div(vec2_cast(vec2f64, _a), vec2_cast(vec2f64, _b)))

#define vec3_div(_a, _b)                                                       \
  _Generic((_a),                                                               \
      vec3i8: _vec3i8_div(vec3_cast(vec3i8, _a), vec3_cast(vec3i8, _b)),       \
      vec3i16: _vec3i16_div(vec3_cast(vec3i16, _a), vec3_cast(vec3i16, _b)),   \
      vec3i32: _vec3i32_div(vec3_cast(vec3i32, _a), vec3_cast(vec3i32, _b)),   \
      vec3i64: _vec3i64_div(vec3_cast(vec3i64, _a), vec3_cast(vec3i64, _b)),   \
      vec3f32: _vec3f32_div(vec3_cast(vec3f32, _a), vec3_cast(vec3f32, _b)),   \
      vec3f64: _vec3f64_div(vec3_cast(vec3f64, _a), vec3_cast(vec3f64, _b)))
