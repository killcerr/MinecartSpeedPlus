#pragma once

#include <ll/api/mod/NativeMod.h>

namespace mod {

class Mod {
public:
    explicit Mod(ll::mod::NativeMod& self);

    Mod(Mod&&)                 = delete;
    Mod(const Mod&)            = delete;
    Mod& operator=(Mod&&)      = delete;
    Mod& operator=(const Mod&) = delete;

    ~Mod(); // NOLINT(performance-trivially-destructible)

    static Mod& getInstance();

    [[nodiscard]] ll::mod::NativeMod& getSelf() const;

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace mod
