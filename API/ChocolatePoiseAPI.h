#pragma once

#include <cstdint>

namespace ChocolatePoiseAPI
{
    using GetArmorReducedStagger_t = float (*)(uint32_t formID, float stagger);

    inline bool Loaded = false;
    inline GetArmorReducedStagger_t GetArmorReducedStagger = nullptr;
}