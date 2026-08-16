#include "ArmorResistance.h"
#include "APIManager.h"

#include <filesystem>

#include <toml++/toml.hpp>
#include "Utility.h"

namespace ARRConfig
{
    float scalingFactor = 1.0f;
    bool disableHidden = false;
    int overrideArmorCap = 0;

    bool loaded = false;

    void Load()
    {
        if (loaded)
        {
            return;
        }

        const auto path =
            std::filesystem::path("Data") /
            "SKSE" /
            "Plugins" /
            "ArmorRatingRescaledRemake.toml";

        try
        {
            const auto config = toml::parse_file(path.string());

            const auto *general = config["General"].as_table();

            if (!general)
            {
                return;
            }

            if (auto value = (*general)["ArmorScalingFactor"].value<double>())
            {
                scalingFactor = static_cast<float>(*value);
            }

            if (auto value = (*general)["DisableHiddenArmorRating"].value<bool>())
            {
                disableHidden = *value;
            }

            if (auto value = (*general)["OverrideArmorCap"].value<int64_t>())
            {
                overrideArmorCap = static_cast<int>(*value);
            }

            loaded = true;
        }
        catch (const toml::parse_error &)
        {
            // Keep defaults.
        }
    }
}

namespace ArmorResistance
{
    namespace
    {
        float g_armorBaseFactor = 0.03f;
        float g_armorScalingFactor = 0.12f;

        float GetGMSTFloat(const char *name, float fallback)
        {
            auto *settings = RE::GameSettingCollection::GetSingleton();

            if (!settings)
            {
                return fallback;
            }

            if (auto *setting = settings->GetSetting(name))
            {
                return setting->GetFloat();
            }

            return fallback;
        }

        int32_t GetArmorPiecesWorn(RE::Actor *actor)
        {
            if (!actor)
            {
                return 0;
            }

            auto inventory = actor->GetInventory(
                [](const RE::TESBoundObject &object)
                {
                    return object.IsArmor();
                });

            int32_t pieces = 0;

            for (auto &[item, inventoryData] : inventory)
            {
                auto &[count, entry] = inventoryData;

                if (count <= 0 || !entry || !entry->IsWorn())
                {
                    continue;
                }

                auto *armor = item->As<RE::TESObjectARMO>();

                if (!armor)
                {
                    continue;
                }

                if (armor->IsLightArmor() ||
                    armor->IsHeavyArmor() ||
                    armor->IsShield())
                {
                    ++pieces;
                }
            }

            return pieces;
        }

        float CalculateVanilla(RE::Actor *actor)
        {
            if (!actor)
            {
                return 0.0f;
            }

            const float armorRating =
                actor->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kDamageResist);

            const int32_t armorPieces =
                GetArmorPiecesWorn(actor);

            /*
             * Vanilla Skyrim armor formula:
             *
             * Physical Resistance =
             *     (Displayed Armor * fArmorScalingFactor / 100)
             *     + (Hidden Armor Pieces * fArmorBaseFactor)
             *
             * Vanilla values:
             *     fArmorScalingFactor = 0.12
             *     fArmorBaseFactor    = 0.03
             *
             * Each worn armor piece contributes:
             *     0.03 = 3% resistance
             *
             * Four pieces:
             *     4 * 0.03 = 12% hidden resistance
             *
             * Armor has an 80% physical damage reduction cap.
             */

            float resistance =
                (armorRating * g_armorScalingFactor / 100.0f) +
                (armorPieces * g_armorBaseFactor);

            return std::min(resistance, 0.80f);
        }

        float CalculateArmorRatingRescaled(RE::Actor *actor)
        {
            if (!actor)
            {
                return 0.0f;
            }

            const float armorRating =
                actor->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kDamageResist);

            const int32_t armorPieces =
                GetArmorPiecesWorn(actor);

            const float vanillaResistance =
                (armorRating / 100.0f) * g_armorScalingFactor;

            const float scalingFactor =
                ARRConfig::scalingFactor * 5.0f;

            float resistance =
                vanillaResistance * scalingFactor;

            resistance =
                resistance / (1.0f + resistance);

            if (!ARRConfig::disableHidden)
            {
                const float hiddenResistance =
                    armorPieces * g_armorBaseFactor;

                resistance +=
                    hiddenResistance * (1.0f - resistance);
            }

            return resistance;
        }

        float CalculateBladeAndBlunt(RE::Actor *actor)
        {
            if (!actor)
            {
                return 0.0f;
            }

            const float armorRating =
                actor->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kDamageResist);

            /*
             * Blade & Blunt armor formula:
             *
             * 0 - 500 Armor Rating:
             *     0.15% resistance per point
             *
             * 500 - 1000 Armor Rating:
             *     0.03% resistance per point
             *
             * 1000+ Armor Rating:
             *     90% maximum resistance
             *
             * Blade & Blunt also removes Skyrim's hidden armor
             * rating from worn armor pieces.
             */

            if (armorRating <= 500.0f)
            {
                return armorRating * 0.0015f;
            }

            if (armorRating <= 1000.0f)
            {
                return 0.75f +
                       (armorRating - 500.0f) * 0.0003f;
            }

            return 0.90f;
        }
    }

    void Initialize()
    {
        g_armorBaseFactor =
            GetGMSTFloat("fArmorBaseFactor", 0.03f);

        g_armorScalingFactor =
            GetGMSTFloat("fArmorScalingFactor", 0.12f);

        SKSE::log::info(
            "ArmorResistance: Skyrim GMSTs | fArmorBaseFactor={} | fArmorScalingFactor={}",
            g_armorBaseFactor,
            g_armorScalingFactor);

        if (APIs::ArmorRatingRescaled)
        {
            ARRConfig::Load();

            SKSE::log::info(
                "ArmorResistance: Using Armor Rating Rescaled Remake.");

            SKSE::log::info(
                "ArmorResistance: ARR ScalingFactor={}, DisableHiddenArmorRating={}, OverrideArmorCap={}",
                ARRConfig::scalingFactor,
                ARRConfig::disableHidden,
                ARRConfig::overrideArmorCap);
        }
        else if (APIs::BladeAndBlunt)
        {
            SKSE::log::info(
                "ArmorResistance: Using Blade and Blunt.");
        }
        else
        {
            SKSE::log::info(
                "ArmorResistance: Using vanilla Skyrim armor formula.");
        }
    }

    bool IsArmorRatingRescaledInstalled()
    {
        return APIs::ArmorRatingRescaled;
    }

    bool IsBladeAndBluntInstalled()
    {
        return APIs::BladeAndBlunt;
    }

    float Get(RE::Actor *actor)
    {
        if (!actor)
        {
            return 0.0f;
        }

        float resistance;

        if (APIs::ArmorRatingRescaled)
        {
            resistance = CalculateArmorRatingRescaled(actor);
        }
        else if (APIs::BladeAndBlunt)
        {
            resistance = CalculateBladeAndBlunt(actor);
        }
        else
        {
            resistance = CalculateVanilla(actor);
        }

        if (debugMode)
        {
            SKSE::log::info(
                "ArmorResistance: formID={:08X} | resistance={}%",
                actor->formID,
                resistance * 100.0f);
        }

        return resistance;
    }

    float GetBladeAndBluntSpellResistance(float armorRating)
    {
        armorRating = std::max(armorRating, 0.0f);

        float resistance;

        if (armorRating <= 500.0f)
        {
            resistance = armorRating * 0.0005f;
        }
        else
        {
            resistance =
                0.25f +
                (armorRating - 500.0f) * 0.0003f;
        }

        return std::min(resistance, 0.25f);
    }
}