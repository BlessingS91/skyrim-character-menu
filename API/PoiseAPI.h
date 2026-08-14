#pragma once

#include <cstdint>

#include "RE/Skyrim.h"

using Poise_GetArmorReducedStagger_t = float (*)(uint32_t, float);
using Poise_GetEffectiveMagicResistance_t = float (*)(RE::Actor *);
using Poise_GetHandDamage_t = float (*)(uint32_t, bool);

bool InitializePoiseAPI();