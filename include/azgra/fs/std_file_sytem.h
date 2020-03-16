#pragma once

#include <azgra/azgra.h>

#ifdef AZGRA_CPP17

#include <filesystem>

#else

#include <experimental/filesystem>

#endif


#ifdef AZGRA_CPP17
namespace sfs = std::filesystem;
#else
namespace sfs = std::experimental::filesystem;
#endif