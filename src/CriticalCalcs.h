#pragma once

#include "RE/Skyrim.h"
#include <Windows.h>

#include "CriticalCalcs.h"

namespace CriticalCalcs
{
    void Initialize();

    float GetCriticalChance(RE::Actor *actor);
    float GetCriticalDamage(RE::Actor *actor);

    float ReadCriticalDamageMultiplier();
}