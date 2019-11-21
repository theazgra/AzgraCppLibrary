#pragma once

#include <azgra/azgra.h>

#ifdef GCC_8

#include <filesystem>

#else

#include <experimental/filesystem>

#endif


#ifdef GCC_8
namespace sfs = std::filesystem;
#else
namespace sfs = std::experimental::filesystem;
#endif