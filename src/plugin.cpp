/**
 * @file plugin.cpp
 * @brief The main file of the plugin
 */

#include <llapi/LoggerAPI.h>

#include "version.h"

// We recommend using the global logger.
extern Logger logger;

/**
 * @brief The entrypoint of the plugin. DO NOT remove or rename this function.
 *
 */
void PluginInit() { logger.info("MinecartSpeeedPlus by killcerr loaded"); }

#include <llapi/mc/RailMovementUtility.hpp>
#include <llapi/mc/Vec3.hpp>

bool flag = false;

TStaticHook(Vec3,
            "?calculateGoldenRailSpeedIncrease@RailMovementUtility@@SA?AVVec3@@"
            "AEBVIConstBlockSource@@AEBVBlockPos@@HV2@@Z",
            RailMovementUtility, class IConstBlockSource const &a,
            class BlockPos const &b, int c, class Vec3 d) {
  auto res = original(a, b, c, d);
  logger.info("calculateGoldenRailSpeedIncrease::speed:{},{},{}", res.x, res.y,
              res.z);
  flag = true;
  return res;
}

TStaticHook(Vec3,
            "?calculateMoveVelocity@RailMovementUtility@@SA?"
            "AVVec3@@AEBVBlock@@HM_NAEAV2@AEA_N3AEBV?$function@$$A6A_"
            "NAEAVVec3@@@Z@std@@@Z",
            RailMovementUtility, class Block const &a, int b, float c, bool d,
            class Vec3 &e, bool &f, bool &g,
            class std::function<bool(class Vec3 &)> const &h) {
  auto res = original(a, b, c, d, e, f, g, h);
  if (!flag)
    res *= 5;
  else {
    flag = false;
    res *= 15;
  }
  logger.info("calculateMoveVelocity::speed:{},{},{}", res.x, res.y, res.z);
  return res;
}
