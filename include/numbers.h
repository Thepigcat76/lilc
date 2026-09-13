#pragma once

#ifndef __SSIZE_T_TYPE
#define __SSIZE_T_TYPE signed long
#endif

typedef unsigned char uchar;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef float f32;
typedef double f64;

typedef __SIZE_TYPE__ usz;
typedef __SSIZE_T_TYPE isz;

#define align_up(x, a) (x + (a - 1)) & ~(a - 1)

#define min(a, b) ((a) > (b) ? (b) : (a))

#define max(a, b) ((a) > (b) ? (a) : (b))

#define clamp(a, min, max) ((a) < (min) ? (min) : ((a) > (max) ? (max) : (a)))
