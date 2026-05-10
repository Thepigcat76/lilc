#include "../include/vec.h"

#define VEC2_ADD_IMPL(_type)                                                   \
  vec2##_type _vec2##_type##_add(vec2##_type a, vec2##_type b) {               \
    return (vec2##_type){.x = (a).x + (b).x, .y = (a).y + (b).y};                       \
  }

#define VEC2_SUB_IMPL(_type)                                                   \
  vec2##_type _vec2##_type##_sub(vec2##_type a, vec2##_type b) {               \
    return (vec2##_type){.x = (a).x - (b).x, .y = (a).y - (b).y};                       \
  }

#define VEC2_MUL_IMPL(_type)                                                   \
  vec2##_type _vec2##_type##_mul(vec2##_type a, vec2##_type b) {               \
    return (vec2##_type){.x = (a).x * (b).x, .y = (a).y * (b).y};                       \
  }

#define VEC2_DIV_IMPL(_type)                                                   \
  vec2##_type _vec2##_type##_div(vec2##_type a, vec2##_type b) {               \
    return (vec2##_type){.x = (a).x / (b).x, .y = (a).y / (b).y};                       \
  }

#define VEC3_ADD_IMPL(_type)                                                   \
  vec3##_type _vec3##_type##_add(vec3##_type a, vec3##_type b) {               \
    return (vec3##_type){.x = (a).x + (b).x, .y = (a).y + (b).y, .z = (a).z + (b).z};                       \
  }

#define VEC3_SUB_IMPL(_type)                                                   \
  vec3##_type _vec3##_type##_sub(vec3##_type a, vec3##_type b) {               \
    return (vec3##_type){.x = (a).x - (b).x, .y = (a).y - (b).y, .z = (a).z - (b).z};                       \
  }

#define VEC3_MUL_IMPL(_type)                                                   \
  vec3##_type _vec3##_type##_mul(vec3##_type a, vec3##_type b) {               \
    return (vec3##_type){.x = (a).x * (b).x, .y = (a).y * (b).y, .z = (a).z * (b).z};                       \
  }

#define VEC3_DIV_IMPL(_type)                                                   \
  vec3##_type _vec3##_type##_div(vec3##_type a, vec3##_type b) {               \
    return (vec3##_type){.x = (a).x / (b).x, .y = (a).y / (b).y, .z = (a).z / (b).z};                       \
  }

VEC2_ADD_IMPL(i8)
VEC2_ADD_IMPL(i16)
VEC2_ADD_IMPL(i32)
VEC2_ADD_IMPL(i64)
VEC2_ADD_IMPL(f32)
VEC2_ADD_IMPL(f64)

VEC2_SUB_IMPL(i8)
VEC2_SUB_IMPL(i16)
VEC2_SUB_IMPL(i32)
VEC2_SUB_IMPL(i64)
VEC2_SUB_IMPL(f32)
VEC2_SUB_IMPL(f64)

VEC2_MUL_IMPL(i8)
VEC2_MUL_IMPL(i16)
VEC2_MUL_IMPL(i32)
VEC2_MUL_IMPL(i64)
VEC2_MUL_IMPL(f32)
VEC2_MUL_IMPL(f64)

VEC2_DIV_IMPL(i8)
VEC2_DIV_IMPL(i16)
VEC2_DIV_IMPL(i32)
VEC2_DIV_IMPL(i64)
VEC2_DIV_IMPL(f32)
VEC2_DIV_IMPL(f64)

VEC3_ADD_IMPL(i8)
VEC3_ADD_IMPL(i16)
VEC3_ADD_IMPL(i32)
VEC3_ADD_IMPL(i64)
VEC3_ADD_IMPL(f32)
VEC3_ADD_IMPL(f64)

VEC3_SUB_IMPL(i8)
VEC3_SUB_IMPL(i16)
VEC3_SUB_IMPL(i32)
VEC3_SUB_IMPL(i64)
VEC3_SUB_IMPL(f32)
VEC3_SUB_IMPL(f64)

VEC3_MUL_IMPL(i8)
VEC3_MUL_IMPL(i16)
VEC3_MUL_IMPL(i32)
VEC3_MUL_IMPL(i64)
VEC3_MUL_IMPL(f32)
VEC3_MUL_IMPL(f64)

VEC3_DIV_IMPL(i8)
VEC3_DIV_IMPL(i16)
VEC3_DIV_IMPL(i32)
VEC3_DIV_IMPL(i64)
VEC3_DIV_IMPL(f32)
VEC3_DIV_IMPL(f64)
