#include "mod.h"

#include <Nlohmann/json.hpp>
#include <filesystem>
#include <ll/api/Config.h>
#include <ll/api/io/Logger.h>


struct {
    float GoldenRailMul;
    float CommonRailMul;
    float ClimbRailMul;
} cfg{1.0, 1.0, 1.0};

void ModInit() {
    const auto configDir = mod::Mod::getInstance().getSelf().getConfigDir();
    std::filesystem::create_directories(configDir);
    auto json = ll::reflection::serialize<nlohmann::json>(cfg);
    if (std::filesystem::exists(configDir / "config.json")) {
        json->patch_inplace(
            nlohmann::json::parse(std::fstream{configDir / "config.json", std::ios::in}, nullptr, true, true)
        );
    } else {
        std::fstream{configDir / "config.json", std::ios::out} << *json;
    }
    ll::reflection::deserialize(cfg, *json).value();
    mod::Mod::getInstance().getSelf().getLogger().info("MinecartSpeedPlus by killcerr loaded");
}

#include <ll/api/memory/Hook.h>
#include <ll/api/memory/Symbol.h>
#include <mc/deps/core/math/Vec3.h>
#include <mc/entity/utilities/RailMovementUtility.h>


bool flag = false;
using namespace ll::literals;

LL_AUTO_TYPE_STATIC_HOOK /*NOLINT*/ (
    calculateGoldenRailSpeedIncreaseHook,
    HookPriority::Normal,
    RailMovementUtility,
    "?calculateGoldenRailSpeedIncrease@RailMovementUtility@@SA?AVVec3@@AEBVIConstBlockSource@@AEBVBlockPos@@HV2@@Z"_sym,
    Vec3,
    class IConstBlockSource const& a,
    class BlockPos const&          b,
    int                            c,
    class Vec3                     d
) {
    auto res = origin(a, b, c, d);
    flag     = true;
    return res;
}

LL_AUTO_TYPE_STATIC_HOOK /*NOLINT*/ (
    calculateMoveVelocityHook,
    HookPriority::Normal,
    RailMovementUtility,
    "?calculateMoveVelocity@RailMovementUtility@@SA?AVVec3@@AEBVBlock@@HM_NAEAV2@AEA_N3AEBV?$function@$$A6A_NAEAVVec3@@@Z@std@@@Z"_sym,
    Vec3,
    class Block const&                            a,
    int                                           b,
    float                                         c,
    bool                                          d,
    class Vec3&                                   e,
    bool&                                         f,
    bool&                                         g,
    class std::function<bool(class Vec3&)> const& h
) {
    auto res = origin(a, b, c, d, e, f, g, h);
    if (!flag) res *= cfg.CommonRailMul;
    else {
        flag  = false;
        res  *= cfg.GoldenRailMul;
    }
    if (res.y > 0) res.y *= cfg.ClimbRailMul;
    return res;
}


void ModDeinit() {
    calculateGoldenRailSpeedIncreaseHook::unhook();
    calculateMoveVelocityHook::unhook();
    mod::Mod::getInstance().getSelf().getLogger().info("MinecartSpeedPlus by killcerr unloaded");
}