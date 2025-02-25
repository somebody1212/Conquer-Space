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
#pragma once

#include <map>
#include <string>

#include <chrono>

extern std::map<std::string, int> profiler_information_map;
#define BEGIN_TIMED_BLOCK(NAME) std::chrono::high_resolution_clock::time_point \
                                block_start_##NAME = std::chrono::high_resolution_clock::now();

#define END_TIMED_BLOCK(NAME) std::chrono::high_resolution_clock::time_point block_end_##NAME = \
                                                std::chrono::high_resolution_clock::now(); \
                                profiler_information_map[#NAME] =\
                                std::chrono::duration_cast<std::chrono::microseconds> \
                                        (block_end_##NAME - block_start_##NAME).count();
