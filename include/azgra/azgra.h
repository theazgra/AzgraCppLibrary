#pragma once

#include <cstring>
#include <vector>
#include <azgra/always_on_assert.h>
#include <memory>
#include <limits>

#include <azgra/utilities/print_helpers.h>
#include <azgra/azgra_math.h>

#ifndef NDEBUG
#define DEBUG 1
#endif

#if (__cplusplus >= 201703L)
#define AZGRA_CPP17
#endif

#ifdef AZGRA_CPP17

#include <string_view>
#include <optional>

#else

#include <experimental/string_view>
#include <experimental/optional>

#endif

#include "azgra_types.h"

#define AZGRA_NAME_OF(var) (#var)