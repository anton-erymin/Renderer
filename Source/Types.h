#pragma once

#include <cstdint>

#ifdef USE_DOUBLE
using float_t = double;
#else
using float_t = float;
#endif