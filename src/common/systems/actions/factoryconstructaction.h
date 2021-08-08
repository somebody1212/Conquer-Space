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

#include <entt/entt.hpp>

#include "common/components/resource.h"
#include "common/universe.h"

namespace conquerspace {
namespace common {
namespace systems {
namespace actions {
/// <summary>
/// Will charge the entity builder the amount needed to construct the factory/building. You'll have to find a organization that can construct it though.
/// </summary>
/// <param name="universe"></param>
/// <param name="city"></param>
/// <param name="recipe"></param>
/// <param name="productivity"></param>
/// <param name="builder"></param>
/// <returns>The factory entity created, entt::null if it's not possible.</returns>
entt::entity OrderConstructionFactory(
    conquerspace::common::components::Universe& universe, entt::entity city,
    entt::entity recipe, int productivity, entt::entity builder);

/// <summary>
/// Creates a factory from thin air
/// </summary>
/// <param name="universe"></param>
/// <param name="city"></param>
/// <param name="recipe"></param>
/// <param name="productivity"></param>
/// <returns>The factory entity created</returns>
entt::entity CreateFactory(conquerspace::common::components::Universe& universe,
    entt::entity city, entt::entity recipe, int productivity);

conquerspace::common::components::ResourceLedger GetFactoryCost(
    conquerspace::common::components::Universe& universe, entt::entity city,
    entt::entity recipe, int productivity);

entt::entity CreateMine(conquerspace::common::components::Universe& universe,
                        entt::entity city, entt::entity good, int amount);

conquerspace::common::components::ResourceLedger GetMineCost(
    conquerspace::common::components::Universe& universe, entt::entity city,
    entt::entity good, int amount);
}  // namespace actions
}  // namespace systems
}  // namespace common
}  // namespace conquerspace