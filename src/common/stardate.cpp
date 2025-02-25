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
#include "common/stardate.h"

#include <fmt/format.h>

#include "date/date.h"

namespace cqsp::common::components {
namespace {
auto GetDateObject(int start_date, int day) {
    auto date = date::year(start_date) / 1 / 1;
    // Add hours to the date and then output
    // Add days to the date
    date = date::sys_days{date} + date::days{day};
    return date;
}
}  // namespace
std::string StarDate::ToString() {
    // Parse the date
    auto date = GetDateObject(start_date, (int)ToDay());
    return fmt::format("{}-{}-{}", (int) date.year(), (unsigned int) date.month(), (unsigned int) date.day());
}

int StarDate::GetYear() {
    auto date = GetDateObject(start_date, (int)ToDay());
    return (int) date.year();
}

int StarDate::GetMonth() {
    auto date = GetDateObject(start_date, (int)ToDay());
    return (unsigned int) date.month();
}

int StarDate::GetDay() {
    auto date = GetDateObject(start_date, (int)ToDay());
    return (unsigned int) date.day();
}
}  // namespace cqsp::common::components
