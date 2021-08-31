/// @file utils.hpp
#pragma once

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
    // ! This code is from Internet, feasibility to be verified
    /// make directory if not exists, 0 for directory successfully created or already exists
    int makedir(char const* path)
    {
        if (access(path, 0) == -1) {
            return mkdir(path
#ifndef WIN32
                , S_IRWXU
#endif
            );
        }
        return 0;
    }


    struct DirectoryNode
    {
        DirectoryNode * _parent;
        DirectoryNode * _subordinate;
        char const* _name;
    };

} // namespace nyas

