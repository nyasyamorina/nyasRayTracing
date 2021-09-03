/// @file utils.hpp
#pragma once

#include <string>
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

    // ! This code is from Internet, feasibility to be verified
    /// make directory if not exists, true for directory successfully created or already exists
    bool makedir(char const* path)
    {
        if (access(path, 0) == -1) {
            return mkdir(path
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


    struct DirectoryNode
    {
        DirectoryNode * _parent;
        DirectoryNode * _subordinate;
        char const* _name;
    };

} // namespace nyas

