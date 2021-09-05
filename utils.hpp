/// @file utils.hpp
#pragma once

#include "common/setup.h"
#include "common/types.hpp"
#include <string>
#include <functional>
#ifdef WIN32    // Windows
    #include <direct.h>
    #include <io.h>
#else           // Linux
    #include <sys/io.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
#endif


namespace nyas
{
    using ::std::string;
    using ::std::function;


    // ! This code is from Internet, feasibility to be verified
    /// make directory if not exists, true for directory successfully created or already exists
    bool makedir(char const* path)
    {
        if (_access(path, 0) == -1) {
            return _mkdir(path
#ifndef WIN32
                , S_IRWXU
#endif
            ) == 0;
        }
        return true;
    }

    bool inline makedir(string const& str)
    {
        return makedir(str.c_str());
    }


    /* mapping operator */

    template<typename FROM, typename TO>
    void mapping_data(function<TO(FROM const&)> const& func, FROM const* from_data, TO * to_data, length_t const& total)
    {
        for (length_t i = 0; i < total; ++i) {
            *(to_data++) = func(*(from_data++));
        }
    }

} // namespace nyas

