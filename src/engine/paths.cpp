/* Conquer Space
* Copyright (C) 2021 Conquer Space
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "engine/paths.h"

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#endif
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

std::string conquerspace::engine::GetConquerSpacePath() {
    std::string directory = "";
#ifdef _WIN32
    // Set log folder
    CHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

    directory = std::string(my_documents);
#else
    // Help
    struct passwd *pw = getpwuid(getuid());

    const char *homedir = pw->pw_dir;
    directory = std::string(homedir);
#endif
    // Create folder
    auto filesystem = std::filesystem::path(directory);
    filesystem /= "ConquerSpace";
    // Create dirs, and be done with it
    if (!std::filesystem::exists(filesystem))
        std::filesystem::create_directories(filesystem);
    return filesystem.string();
}