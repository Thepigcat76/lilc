#pragma once

#include "numbers.h"

typedef i32 color_t;

/* Color functions */

#define color_make(red, green, blue, alpha)                              \
  (color_t)((red) << 24) | ((green) << 16) | ((blue) << 8) | (alpha)

#define color_red(color) (color >> 24) & 0xFF

#define color_green(color) (color >> 16) & 0xFF

#define color_blue(color) (color >> 8) & 0xFF

#define color_alpha(color) color & 0xFF