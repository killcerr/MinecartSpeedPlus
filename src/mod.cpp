#include "mod.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"


extern void ModInit();
extern void ModDeinit();
namespace mod {

Mod& Mod::getInstance() {
    static Mod instance;
    return instance;
}

bool Mod::load() /*NOLINT*/ {
    getSelf().getLogger().debug("Loading...");
    return true;
}

bool Mod::enable() /*NOLINT*/ {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    ModInit();
    return true;
}

bool Mod::disable() /*NOLINT*/ {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    ModDeinit();
    return true;
}

bool Mod::unload() /*NOLINT*/ {
    getSelf().getLogger().debug("Unloading...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace mod

LL_REGISTER_MOD(mod::Mod, mod::Mod::getInstance());