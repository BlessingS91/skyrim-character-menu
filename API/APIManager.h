#ifndef APIMANAGER_H
#define APIMANAGER_H

#pragma once

#include <Windows.h>

#define SMOOTHCAM_API_COMMONLIB
#include "SmoothCamAPI.h"
#include "PerkEntryPointExtenderAPI.h"
#include "PoiseAPI.h"

namespace ArmorResistance
{
    void Initialize();

    bool IsArmorRatingRescaledInstalled();
    bool IsBladeAndBluntInstalled();

    float Get(RE::Actor *actor);
    float GetBladeAndBluntSpellResistance(float armorRating);
}

struct APIs
{
    static inline SmoothCamAPI::IVSmoothCam2 *SmoothCam = nullptr;
    static inline PerkEntryPointExtenderAPI::InterfaceVersion2 *PEPE = nullptr;

    static inline bool ArmorRatingRescaled = false;
    static inline bool BladeAndBlunt = false;
    static inline bool HandToHand = false;

    static void RequestAPIs();
};

extern SmoothCamAPI::IVSmoothCam2 *g_SmoothCam;

extern Poise_GetArmorReducedStagger_t g_Poise_GetArmorReducedStagger;
extern Poise_GetEffectiveMagicResistance_t g_Poise_GetEffectiveMagicResistance;
extern Poise_GetHandDamage_t g_Poise_GetHandDamage;

#endif