#include "mod.h"

#include <Nlohmann/json.hpp>
#include <filesystem>
#include <ll/api/Config.h>
#include <ll/api/io/Logger.h>


struct {
    float GoldenRailMul;
    float CommonRailMul;
    float ClimbRailMul;
    bool  EnableMinecartAABBHook;
} cfg{
    .GoldenRailMul          = 1.0,
    .CommonRailMul          = 1.0,
    .ClimbRailMul           = 1.0,
    .EnableMinecartAABBHook = false,
};

void ModInit() {
    const auto configDir = mod::Mod::getInstance().getSelf().getConfigDir();
    std::filesystem::create_directories(configDir);
    auto json = ll::reflection::serialize<nlohmann::json>(cfg);
    if (std::filesystem::exists(configDir / "config.json")) {
        json->merge_patch(
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

#include <mc/world/actor/Actor.h>
#include <mc/world/actor/ActorType.h>
#include <mc/world/phys/AABB.h>
LL_AUTO_TYPE_INSTANCE_HOOK(minecartAABBHook, HookPriority::Normal, Actor, &Actor::getAABB, AABB const&) /*NOLINT*/ {
    static AABB box{Vec3::ZERO(), Vec3::ZERO()};
    if (cfg.EnableMinecartAABBHook
        && (static_cast<int>(getEntityTypeId()) & static_cast<int>(ActorType::Minecart)) == 0x80000)
        return box;
    return origin();
}

void ModDeinit() {
    calculateGoldenRailSpeedIncreaseHook::unhook();
    calculateMoveVelocityHook::unhook();
    minecartAABBHook::unhook();
    mod::Mod::getInstance().getSelf().getLogger().info("MinecartSpeedPlus by killcerr unloaded");
}
