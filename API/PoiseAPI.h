#pragma once

#include <cstdint>

#include "RE/Skyrim.h"

using PoiseDamageCallback_t = float (*)(RE::Actor *attacker, RE::Actor *target, float damage);

using Poise_GetArmorReducedStagger_t = float (*)(uint32_t, float);
using Poise_GetEffectiveMagicResistance_t = float (*)(RE::Actor *);
using Poise_GetHandDamage_t = float (*)(uint32_t, bool);

extern Poise_GetArmorReducedStagger_t g_Poise_GetArmorReducedStagger;
extern Poise_GetEffectiveMagicResistance_t g_Poise_GetEffectiveMagicResistance;
extern Poise_GetHandDamage_t g_Poise_GetHandDamage;

float ApplyExternalDamageCallbacks(RE::Actor *attacker, RE::Actor *target, float damage);