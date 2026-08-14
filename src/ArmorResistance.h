#pragma once

#include "RE/Skyrim.h"

namespace ArmorResistance
{
    void Initialize();

    bool IsArmorRatingRescaledInstalled();
    bool IsBladeAndBluntInstalled();

    float Get(RE::Actor *actor);

    float GetBladeAndBluntSpellResistance(float armorRating);
}