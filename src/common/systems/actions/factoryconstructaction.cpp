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
#include "common/systems/actions/factoryconstructaction.h"

#include <spdlog/spdlog.h>

#include "common/components/resource.h"
#include "common/components/area.h"
#include "common/components/economy.h"
#include "common/systems/economy/markethelpers.h"

using cqsp::common::Universe;
entt::entity cqsp::common::systems::actions::OrderConstructionFactory(cqsp::common::Universe& universe,
    entt::entity city, entt::entity market, entt::entity recipe, int productivity, entt::entity builder) {
    entt::entity factory = common::systems::actions::CreateFactory(
        universe, city, recipe, productivity);
    if (factory == entt::null) {
        return entt::null;
    }
    cqsp::common::systems::economy::AddParticipant(universe, market, factory);
    auto cost = common::systems::actions::GetFactoryCost(universe, city, recipe, productivity);

    // Buy the goods on the market
    common::systems::economy::PurchaseGood(universe, builder, cost);
    return factory;
}

entt::entity cqsp::common::systems::actions::CreateFactory(Universe& universe, entt::entity city,
    entt::entity recipe, int productivity) {
    namespace cqspc = cqsp::common::components;
    // Make the factory
    entt::entity factory = universe.create();
    auto& factory_converter = universe.emplace<cqspc::ResourceConverter>(factory);
    universe.emplace<cqspc::Factory>(factory);

    // Add capacity
    // Add producivity
    auto& prod = universe.emplace<cqspc::FactoryProductivity>(factory);
    prod.current_production = productivity;
    prod.max_production = static_cast<float>(productivity);

    universe.emplace<cqspc::ResourceStockpile>(factory);
    auto& employer = universe.emplace<cqspc::Employer>(factory);
    employer.population_fufilled = 1000000;
    employer.population_needed = 1000000;
    employer.segment = entt::null;

    // Add recipes and stuff
    factory_converter.recipe = recipe;
    universe.get<cqspc::Industry>(city).industries.push_back(factory);
    return factory;
}

cqsp::common::components::ResourceLedger
cqsp::common::systems::actions::GetFactoryCost(cqsp::common::Universe& universe, entt::entity city,
    entt::entity recipe, int capacity) {
    cqsp::common::components::ResourceLedger ledger;
    // Get the recipe and things
    if (universe.any_of<components::RecipeCost>(recipe)) {
        auto& cost = universe.get<components::RecipeCost>(recipe);
        ledger.MultiplyAdd(cost.scaling, capacity);
        ledger += cost.fixed;
    }
    return ledger;
}

entt::entity cqsp::common::systems::actions::CreateMine(cqsp::common::Universe& universe,
    entt::entity city, entt::entity good, int amount, float productivity) {
    namespace cqspc = cqsp::common::components;
    entt::entity mine = universe.create();
    auto& gen = universe.emplace<cqspc::ResourceGenerator>(mine);

    auto& employer = universe.emplace<cqspc::Employer>(mine);
    employer.population_fufilled = 1000000;
    employer.population_needed = 1000000;
    employer.segment = entt::null;

    gen.emplace(good, amount);
    universe.get<cqspc::Industry>(city).industries.push_back(mine);

    // Add productivity
    universe.emplace<cqspc::FactoryProductivity>(mine, productivity, productivity);

    universe.emplace<cqspc::ResourceStockpile>(mine);
    universe.emplace<cqspc::Mine>(mine);
    universe.emplace<cqspc::RawResourceGen>(mine);
    return mine;
}

cqsp::common::components::ResourceLedger
cqsp::common::systems::actions::GetMineCost(cqsp::common::Universe& universe, entt::entity city,
    entt::entity good, int amount) {
    return cqsp::common::components::ResourceLedger();
}

entt::entity
cqsp::common::systems::actions::CreateCommercialArea(cqsp::common::Universe& universe, entt::entity city) {
    namespace cqspc = cqsp::common::components;
    entt::entity commercial = universe.create();

    universe.emplace<cqspc::Employer>(commercial);
    universe.emplace<cqspc::Commercial>(commercial, city, 0);

    universe.get<cqspc::Industry>(city).industries.push_back(commercial);
    return commercial;
}

entt::entity cqsp::common::systems::actions::CreateFarm(
    cqsp::common::Universe& universe, entt::entity city, entt::entity good,
    int amount, float productivity) {
    namespace cqspc = cqsp::common::components;
    entt::entity farm = universe.create();
    auto& gen = universe.emplace<cqspc::ResourceGenerator>(farm);

    auto& employer = universe.emplace<cqspc::Employer>(farm);
    employer.population_fufilled = 1000000;
    employer.population_needed = 1000000;
    employer.segment = entt::null;

    gen.emplace(good, amount);
    universe.get<cqspc::Industry>(city).industries.push_back(farm);

    // Add productivity
    universe.emplace<cqspc::FactoryProductivity>(farm, productivity, productivity);

    universe.emplace<cqspc::ResourceStockpile>(farm);
    universe.emplace<cqspc::Farm>(farm);
    universe.emplace<cqspc::RawResourceGen>(farm);
    return farm;
}
