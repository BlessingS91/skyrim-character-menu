#include "PerkEntryPointExtenderAPI.h"

#include <array>
#include <span>

#include "RE/Skyrim.h"

#include <Windows.h>
#include <fstream>
#include <string>

#include <filesystem>

#include <Utility.h>

namespace CriticalCalcs
{
    namespace
    {
        PerkEntryPointExtenderAPI::InterfaceVersion2 *g_pepe = nullptr;
        HMODULE g_criticalDamageFixModule = nullptr;
        bool g_initialized = false;

        float g_criticalDamageMultiplier = 1.0f;
    }

    float ReadCriticalDamageMultiplier()
    {
        constexpr float defaultMultiplier = 0.5f;

        const std::filesystem::path path =
            std::filesystem::path("Data") /
            "SKSE" /
            "Plugins" /
            "ComprehensiveCriticalDamageFix.ini";

        std::ifstream file(path, std::ios::in | std::ios::binary);

        if (!file.is_open())
        {
            logger::warn(
                "CriticalCalcs: Could not open ComprehensiveCriticalDamageFix.ini. "
                "Using default multiplier {}.",
                defaultMultiplier);

            return defaultMultiplier;
        }

        std::string line;
        bool inSettings = false;
        bool firstLine = true;

        while (std::getline(file, line))
        {
            // Remove Windows CR.
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            // Remove UTF-8 BOM from the first line.
            if (firstLine)
            {
                firstLine = false;

                if (line.size() >= 3 &&
                    static_cast<unsigned char>(line[0]) == 0xEF &&
                    static_cast<unsigned char>(line[1]) == 0xBB &&
                    static_cast<unsigned char>(line[2]) == 0xBF)
                {
                    line.erase(0, 3);
                }
            }

            // Trim whitespace.
            const auto first = line.find_first_not_of(" \t");

            if (first == std::string::npos)
            {
                continue;
            }

            const auto last = line.find_last_not_of(" \t");

            line = line.substr(
                first,
                last - first + 1);

            // Skip comments.
            if (line[0] == ';' || line[0] == '#')
            {
                continue;
            }

            // Section.
            if (line.front() == '[' && line.back() == ']')
            {
                inSettings = (line == "[Settings]");
                continue;
            }

            if (!inSettings)
            {
                continue;
            }

            const auto equals = line.find('=');

            if (equals == std::string::npos)
            {
                continue;
            }

            std::string name = line.substr(0, equals);
            std::string value = line.substr(equals + 1);

            // Trim key.
            const auto nameFirst = name.find_first_not_of(" \t");
            const auto nameLast = name.find_last_not_of(" \t");

            if (nameFirst != std::string::npos)
            {
                name = name.substr(
                    nameFirst,
                    nameLast - nameFirst + 1);
            }

            // Trim value.
            const auto valueFirst = value.find_first_not_of(" \t");
            const auto valueLast = value.find_last_not_of(" \t");

            if (valueFirst != std::string::npos)
            {
                value = value.substr(
                    valueFirst,
                    valueLast - valueFirst + 1);
            }

            if (name != "fCriticalDamageMultiplier")
            {
                continue;
            }

            try
            {
                const float multiplier = std::stof(value);

                logger::info(
                    "CriticalCalcs: Critical damage multiplier = {}",
                    multiplier);

                return multiplier;
            }
            catch (const std::exception &)
            {
                logger::warn(
                    "CriticalCalcs: Invalid fCriticalDamageMultiplier '{}'. "
                    "Using default multiplier {}.",
                    value,
                    defaultMultiplier);

                return defaultMultiplier;
            }
        }

        logger::warn(
            "CriticalCalcs: fCriticalDamageMultiplier not found. "
            "Using default multiplier {}.",
            defaultMultiplier);

        return defaultMultiplier;
    }

    void Initialize()
    {
        if (g_initialized)
        {
            return;
        }

        g_initialized = true;

        g_pepe =
            PerkEntryPointExtenderAPI::RequestInterface<
                PerkEntryPointExtenderAPI::InterfaceVersion2>();

        if (!g_pepe)
        {
            logger::warn(
                "CriticalCalcs: PEPE V2 interface unavailable.");
        }

        g_criticalDamageFixModule =
            GetModuleHandleW(
                L"ComprehensiveCriticalDamageFix.dll");

        if (g_criticalDamageFixModule)
        {
            g_criticalDamageMultiplier =
                ReadCriticalDamageMultiplier();
        }
    }

    float GetCriticalChance(RE::Actor *actor)
    {
        if (!actor)
        {
            return 0.0f;
        }

        auto *actorValueOwner = actor->AsActorValueOwner();

        if (!actorValueOwner)
        {
            return 0.0f;
        }

        const float actorValueCritChance =
            actorValueOwner->GetActorValue(
                RE::ActorValue::kCriticalChance);

        RE::TESForm *weapon = nullptr;

        if (auto *character = actor->As<RE::Character>())
        {
            weapon = character->GetEquippedObject(true);

            if (!weapon)
            {
                weapon = character->GetEquippedObject(false);
            }
        }

        if (!g_pepe)
        {
            return actorValueCritChance;
        }

        float perkCritChance = 0.0f;

        std::array<RE::TESForm *, 2> args{
            weapon,
            nullptr};

        const auto result =
            g_pepe->ApplyPerkEntryPoint_Deprecated(
                actor,
                RE::PerkEntryPoint::kCalculateMyCriticalHitChance,
                std::span<RE::TESForm *>(args),
                &perkCritChance,
                "",
                0,
                PEPE::EntryPointFlag::None);

        if (result != PEPE::RequestResult::Success)
        {
            logger::warn(
                "CalculateMyCriticalHitChance failed: {}",
                static_cast<int>(result));
        }

        const float totalCritChance =
            actorValueCritChance + perkCritChance;

        logger::info(
            "CriticalCalcs: CritChance | ActorValue={} | Perk={} | Total={}",
            actorValueCritChance,
            perkCritChance,
            totalCritChance);

        return totalCritChance;
    }

    float GetCriticalDamage(RE::Actor *actor)
    {
        if (!actor)
        {
            return 0.0f;
        }

        RE::TESForm *weapon = nullptr;

        if (auto *character = actor->As<RE::Character>())
        {
            weapon = character->GetEquippedObject(true);

            if (!weapon)
            {
                weapon = character->GetEquippedObject(false);
            }
        }

        float perkCritDamage = 0.0f;

        if (g_pepe)
        {
            std::array<RE::TESForm *, 2> args{
                weapon,
                nullptr};

            const auto result =
                g_pepe->ApplyPerkEntryPoint_Deprecated(
                    actor,
                    RE::PerkEntryPoint::kCalculateMyCriticalHitDamage,
                    std::span<RE::TESForm *>(args),
                    &perkCritDamage,
                    "",
                    0,
                    PEPE::EntryPointFlag::None);

            if (result != PEPE::RequestResult::Success)
            {
                logger::warn(
                    "CalculateMyCriticalHitDamage failed: {}",
                    static_cast<int>(result));
            }
        }

        // Base critical damage.
        float criticalDamage =
            1.0f + g_criticalDamageMultiplier;

        // Perk critical-damage modifiers are percentage increases.
        criticalDamage *=
            1.0f + perkCritDamage;

        logger::info(
            "CriticalCalcs: Crit Damage | BaseMultiplier={} | PerkModifier={} | FinalMultiplier={}",
            g_criticalDamageMultiplier,
            perkCritDamage,
            criticalDamage);

        return criticalDamage;
    }

} // namespace CriticalCalcs
