#include "APIManager.h"
#include "Utility.h"
#include "ArmorResistance.h"
#include "PoiseAPI.h"

SmoothCamAPI::IVSmoothCam2 *g_SmoothCam = nullptr;

Poise_GetArmorReducedStagger_t g_Poise_GetArmorReducedStagger = nullptr;
Poise_GetEffectiveMagicResistance_t g_Poise_GetEffectiveMagicResistance = nullptr;
Poise_GetHandDamage_t g_Poise_GetHandDamage = nullptr;

bool InitializePoiseAPI()
{
    HMODULE module = GetModuleHandleW(L"ChocolatePoise.dll");

    if (!module)
    {
        logger::info("ChocolatePoise.dll is not loaded.");
        return false;
    }

    g_Poise_GetArmorReducedStagger =
        reinterpret_cast<Poise_GetArmorReducedStagger_t>(
            GetProcAddress(module, "Poise_GetArmorReducedStagger"));

    g_Poise_GetEffectiveMagicResistance =
        reinterpret_cast<Poise_GetEffectiveMagicResistance_t>(
            GetProcAddress(module, "Poise_GetEffectiveMagicResistance"));

    g_Poise_GetHandDamage =
        reinterpret_cast<Poise_GetHandDamage_t>(
            GetProcAddress(module, "Poise_GetHandDamage"));

    if (!g_Poise_GetArmorReducedStagger ||
        !g_Poise_GetEffectiveMagicResistance ||
        !g_Poise_GetHandDamage)
    {
        logger::error("Failed to resolve ChocolatePoise API.");
        return false;
    }

    logger::info("ChocolatePoise API resolved successfully.");
    return true;
}

void APIs::RequestAPIs()
{
    // SmoothCam
    if (!SmoothCam)
    {
        if (!SmoothCamAPI::RegisterInterfaceLoaderCallback(
                SKSE::GetMessagingInterface(),
                [](void *interfaceInstance,
                   SmoothCamAPI::InterfaceVersion interfaceVersion)
                {
                    if (interfaceVersion ==
                        SmoothCamAPI::InterfaceVersion::V2)
                    {
                        SmoothCam =
                            reinterpret_cast<SmoothCamAPI::IVSmoothCam2 *>(
                                interfaceInstance);

                        g_SmoothCam = SmoothCam;

                        logger::info("Obtained SmoothCamAPI");
                    }
                }))
        {
            logger::warn(
                "SmoothCamAPI::RegisterInterfaceLoaderCallback reported an error");
        }

        if (!SmoothCamAPI::RequestInterface(
                SKSE::GetMessagingInterface(),
                SmoothCamAPI::InterfaceVersion::V2))
        {
            logger::warn(
                "SmoothCamAPI::RequestInterface reported an error");
        }
    }

    // Perk Entry Point Extender
    if (!PEPE)
    {
        PEPE =
            PerkEntryPointExtenderAPI::RequestInterface<
                PerkEntryPointExtenderAPI::InterfaceVersion2>();

        if (PEPE)
        {
            logger::info("Obtained PerkEntryPointExtender API V2");
        }
        else
        {
            logger::info("PerkEntryPointExtender API V2 unavailable.");
        }
    }

    // Optional mods
    ArmorRatingRescaled =
        GetModuleHandleW(L"ArmorRatingRescaledRemake.dll") != nullptr;

    BladeAndBlunt =
        GetModuleHandleW(L"BladeAndBlunt.dll") != nullptr;

    logger::info(
        "ArmorRatingRescaled: {}",
        ArmorRatingRescaled);

    logger::info(
        "BladeAndBlunt: {}",
        BladeAndBlunt);

    // ChocolatePoise
    InitializePoiseAPI();

    // Armor resistance setup
    ArmorResistance::Initialize();
}