/*
* Copyright 2021 Conquer Space
*/
#include "common/systems/sysuniversegenerator.h"

#include <spdlog/spdlog.h>
#include <string>

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>

#include <sol/sol.hpp>

#include "common/components/bodies.h"
#include "common/components/orbit.h"
#include "common/components/organizations.h"
#include "common/components/player.h"
#include "common/components/surface.h"
#include "common/components/name.h"
#include "common/components/population.h"
#include "common/components/area.h"
#include "common/components/resource.h"

void conquerspace::systems::universegenerator::SysGenerateUniverse(
    conquerspace::engine::Application& app) {
    namespace cqspa = conquerspace::asset;
    namespace cqspb = conquerspace::components::bodies;
    namespace cqspc = conquerspace::components;

    conquerspace::components::Universe& universe = app.GetUniverse();

    cqspa::HjsonAsset* val
        = app.GetAssetManager().GetAsset<cqspa::HjsonAsset>("defaultuniversegen");

    // Initialize lua environment
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);

    // Init civilization script
    lua.set_function("create_star_system", [&] () {
        entt::entity ent = universe.create();
        universe.emplace<cqspb::StarSystem>(ent);
        return ent;
     });

    // Set print functions
    lua.set_function("print", sol::overload(
    [] (const char * y) {
        spdlog::info("* [lua]: {}", y);
    },
    [](int y) {
        spdlog::info("* [lua]: {}", y);
    },
    [](double y) {
        spdlog::info("* [lua]: {}", y);
    }));

    // RNG
    boost::mt19937 gen;
    lua.set_function("random", [&gen] (int low, int high) {
        boost::random::uniform_int_distribution<> dist(low, high);
        return dist(gen);
    });

    lua.set_function("random_normal_int", [&gen] (int mean, int sd) {
        boost::normal_distribution<> nd(mean, sd);
        boost::variate_generator<boost::mt19937&,
                           boost::normal_distribution<> > var_nor(gen, nd);
        return static_cast<int>(round(var_nor()));
    });

    lua.set_function("add_planet", [&] (entt::entity system) {
        entt::entity planet = universe.create();
        auto& body = universe.emplace<cqspb::Body>(planet);
        body.star_system = system;
        universe.emplace<cqspb::Planet>(planet);
        universe.get<cqspb::StarSystem>(system).bodies.push_back(planet);
        return planet;
    });

    lua.set_function("add_star", [&] (entt::entity system) {
        entt::entity star = universe.create();
        universe.emplace<cqspb::Star>(star);
        auto& body = universe.emplace<cqspb::Body>(star);
        body.star_system = system;

        universe.emplace<cqspb::LightEmitter>(star);

        universe.get<cqspb::StarSystem>(system).bodies.push_back(star);
        return star;
    });

    lua.set_function("set_orbit", [&] (entt::entity body,
                                            double distance,
                                            double theta,
                                            double eccentricity,
                                            double argument) {
        cqspb::Orbit orb = universe.emplace<cqspb::Orbit>(body,
                                                                    theta,
                                                                    distance,
                                                                    eccentricity,
                                                                    argument);
    });

    lua.set_function("set_radius", [&] (entt::entity body, int radius) {
        cqspb::Body &bod = universe.get<cqspb::Body>(body);
        bod.radius = radius;
    });

    lua.set_function("add_civilization", [&] () {
        entt::entity civ = universe.create();
        universe.emplace<cqspc::Organization>(civ);
        return civ;
    });

    lua.set_function("set_civilization_planet", [&] (entt::entity civ, entt::entity planet) {
        universe.emplace<cqspc::Civilization>(civ, planet);
    });

    lua.set_function("get_civilization_planet", [&] (entt::entity civ) {
        return universe.get<cqspc::Civilization>(civ).starting_planet;
    });

    lua.set_function("is_player", [&] (entt::entity civ) {
        return static_cast<bool>(universe.all_of<cqspc::Player>(civ));
    });

    lua.set_function("add_planet_habitation", [&] (entt::entity planet) {
        universe.emplace<cqspc::Habitation>(planet);
    });

    lua.set_function("add_planet_settlement", [&] (entt::entity planet) {
        entt::entity settlement = universe.create();
        universe.emplace<cqspc::Settlement>(settlement);
        // Add to planet list
        universe.get<cqspc::Habitation>(planet).settlements.push_back(settlement);
        return settlement;
    });

    lua.set_function("add_population_segment", [&] (entt::entity settlement, uint64_t popsize) {
        entt::entity population = universe.create();
        universe.emplace<cqspc::PopulationSegment>(population, popsize);
        // Add to planet list
        universe.get<cqspc::Settlement>(settlement).population.push_back(population);

        return population;
    });

    // Configure the population
    lua.set_function("set_name", [&] (entt::entity entity, std::string name) {
        universe.emplace_or_replace<cqspc::Name>(entity, name);
    });

    lua.set_function("create_industries", [&] (entt::entity city) {
        universe.emplace<cqspc::Industry>(city);
    });

    lua.set_function("create_factory", [&](entt::entity city, entt::entity recipe,
                                                                            float productivity) {
        entt::entity factory = universe.create();
        auto& gen = universe.emplace<cqspc::ResourceConverter>(factory);
        universe.emplace<cqspc::Factory>(factory);

        // Add producivity
        auto& prod = universe.emplace<cqspc::FactoryProductivity>(factory);
        prod.productivity = productivity;

        universe.emplace<cqspc::ResourceStockpile>(factory);

        // Add recipes and stuff
        gen.recipe = recipe;
        universe.get<cqspc::Industry>(city).industries.push_back(factory);

        return factory;
     });

    lua.set_function("create_mine", [&](entt::entity city, entt::entity resource, int amount,
                                                                            float productivity) {
        entt::entity mine = universe.create();
        auto& gen = universe.emplace<cqspc::ResourceGenerator>(mine);
        universe.emplace<cqspc::Mine>(mine);

        gen.emplace(resource, amount);
        universe.get<cqspc::Industry>(city).industries.push_back(mine);

        // Add producivity
        auto& prod = universe.emplace<cqspc::FactoryProductivity>(mine);
        prod.productivity = productivity;

        universe.emplace<cqspc::ResourceStockpile>(mine);
        return mine;
    });

    lua["goods"] = universe.goods;
    lua["recipes"] = universe.recipes;

    // Load and run utility scripts
    for (int i = 0; i < val->data["utility"].size(); i++) {
        std::string utility_script = val->data[i];
        cqspa::TextAsset* civgenscriptasset =
                    app.GetAssetManager().GetAsset<cqspa::TextAsset>(utility_script);
        sol::load_result utility_result = lua.load(civgenscriptasset->data);

        sol::protected_function_result res = utility_result();
        if (!res.valid()) {
            sol::error err = res;
            std::string what = err.what();
            spdlog::info("*[lua]: {}", what);
        }
    }

    // Create player
    auto player = universe.create();
    universe.emplace<cqspc::Organization>(player);
    universe.emplace<cqspc::Player>(player);

    for (int i = 0; i < 9; i++) {
        auto civ = universe.create();
        universe.emplace<cqspc::Organization>(civ);
    }

    cqspa::TextAsset* civgenscript
            = app.GetAssetManager().GetAsset<cqspa::TextAsset>(val->data["civ-gen"]);
    sol::load_result civGenResult = lua.load(civgenscript->data);

    // Loop through each civilization
    auto civilizationView = universe.view<cqspc::Organization>();

    for (auto a : civilizationView) {
        lua.set("civ", a);
        sol::protected_function_result res = civGenResult();
        if (!res.valid()) {
            sol::error err = res;
            std::string what = err.what();
            spdlog::info("*[lua]: {}", what);
        }
    }

    lua.set("civilizations", sol::as_table(civilizationView));

    cqspa::TextAsset* as
            = app.GetAssetManager().GetAsset<cqspa::TextAsset>(val->data["universe-gen"]);
    sol::load_result script = lua.load(as->data);

    sol::protected_function_result result = script();
    if (!result.valid()) {
        sol::error err = result;
        std::string what = err.what();
        spdlog::info("*[lua]: {}", what);
    }

    // Now add civilization
    cqspa::TextAsset* civgen =
        app.GetAssetManager().GetAsset<cqspa::TextAsset>(val->data["civ-system"]);
    sol::load_result civGenScript = lua.load(civgen->data);

    for (auto ent : civilizationView.each()) {
        lua.set("civilization_id", ent);
        // Initialize, etc
        sol::protected_function_result result = civGenScript();
        if (!result.valid()) {
            // aww
            sol::error err = result;
            std::string what = err.what();
            spdlog::info("*[lua]: {}", what);
        }
    }
}
