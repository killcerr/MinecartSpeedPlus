#include "mod.h"

#include <ll/api/mod/NativeMod.h>

extern void ModInit();

namespace mod {

Mod::~Mod() = default;

static std::unique_ptr<Mod> mod{};

Mod& Mod::getInstance() { return *mod; }

Mod::Mod(ll::mod::NativeMod& self) : mSelf(self) {
    // mSelf.getLogger().info("loading...");

    // Code for loading the mod goes here.
}

ll::mod::NativeMod& Mod::getSelf() const { return mSelf; }

bool Mod::enable() {
    // mSelf.getLogger().info("enabling...");
    ModInit();
    // Code for enabling the mod goes here.

    return true;
}

bool Mod::disable() {
    // mSelf.getLogger().info("disabling...");

    // Code for disabling the mod goes here.

    return true;
}

extern "C" {
_declspec(dllexport) bool ll_mod_load(ll::mod::NativeMod& self) {
    mod = std::make_unique<mod::Mod>(self);
    return true;
}

/// @warning Unloading the mod may cause a crash if the mod has not released all of its
/// resources. If you are unsure, keep this function commented out.
// _declspec(dllexport) bool ll_mod_unload(ll::mod::Mod&) {
//     mod.reset();
//
//     return true;
// }

_declspec(dllexport) bool ll_mod_enable(ll::mod::NativeMod&) { return mod->enable(); }

_declspec(dllexport) bool ll_mod_disable(ll::mod::NativeMod&) { return mod->disable(); }
}

} // namespace mod
