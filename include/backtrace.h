#pragma once

#include "numbers.h"
#include "dynstr.h"

// Returns the number of frames
usz backtrace_sprint(usz max_frames, dyn_string_t *buf);
