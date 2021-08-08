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

#include <vector>

#include "common/scripting/scripting.h"
#include "common/universe.h"

namespace conquerspace {
namespace common {
namespace systems {
class SysEventScriptRunner {
 public:
    SysEventScriptRunner(conquerspace::common::components::Universe& _universe,
                         scripting::ScriptInterface& interface);
    void ScriptEngine();
    ~SysEventScriptRunner();
 private:
    scripting::ScriptInterface &m_script_interface;
    conquerspace::common::components::Universe& universe;
    std::vector<sol::table> events;
};
}  // namespace systems
}  // namespace common
}  // namespace conquerspace