#ifndef APIMANAGER_H
#define APIMANAGER_H

#pragma once

#include <Windows.h>

#define SMOOTHCAM_API_COMMONLIB
#include "SmoothCamAPI.h"

namespace ChocolatePoiseAPI
{
    using GetArmorReducedStagger_t = float (*)(uint32_t formID, float stagger);

    inline bool Loaded = false;
    inline GetArmorReducedStagger_t GetArmorReducedStagger = nullptr;
}

struct APIs
{
    static inline SmoothCamAPI::IVSmoothCam2 *SmoothCam = nullptr;

    static void RequestAPIs();
};

extern SmoothCamAPI::IVSmoothCam2 *g_SmoothCam;

#endif // APIMANAGER_H