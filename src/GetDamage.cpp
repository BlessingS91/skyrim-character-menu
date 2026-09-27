#include "GetDamage.h"
#include "Utility.h"
#include "APIManager.h"
#include "PCH.h"
namespace GetDamage
{
    float GetWeaponDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto weapon = equippedObject
                                ? equippedObject->As<RE::TESObjectWEAP>()
                                : nullptr;

        if (!weapon)
        {
            return 0.0f;
        }

        const auto equippedEntry = player->GetEquippedEntryData(left);

        float damage = 0.0f;

        const auto weaponType = weapon->GetWeaponType();

        // Bow / Crossbow:
        // Match WidgetEquip exactly.
        if (weaponType == RE::WEAPON_TYPE::kBow ||
            weaponType == RE::WEAPON_TYPE::kCrossbow)
        {
            if (const auto ammo = player->GetCurrentAmmo())
            {
                float scale = 1.0f;

                RE::BGSEntryPoint::HandleEntryPoint(
                    RE::BGSEntryPoint::ENTRY_POINT::kModAttackDamage,
                    player,
                    nullptr,
                    nullptr,
                    &scale);

                damage = ammo->GetRuntimeData().data.damage * scale;
            }
        }

        // Normal weapon damage:
        // Same calculation used by WidgetEquip::MakeWeaponInfo().
        if (equippedEntry)
        {
            damage += player->GetDamage(equippedEntry);
        }

        if (debugMode)
        {
            logger::info(
                "GetWeaponDamage | Hand={} | Weapon={} | Damage={}",
                left ? "Left" : "Right",
                weapon->GetName(),
                damage);
        }

        return damage;
    }

    float GetSpellDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto spell = equippedObject
                               ? equippedObject->As<RE::SpellItem>()
                               : nullptr;

        if (!spell ||
            spell->effects.empty() ||
            !spell->effects[0] ||
            !spell->effects[0]->baseEffect)
        {
            return 0.0f;
        }

        float scale = 1.0f;

        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModSpellMagnitude,
            player,
            spell,
            nullptr,
            &scale);

        return spell->effects[0]->effectItem.magnitude * scale;
    }

    float GetStaffDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto staff = equippedObject
                               ? equippedObject->As<RE::TESObjectWEAP>()
                               : nullptr;

        if (!staff ||
            staff->GetWeaponType() != RE::WEAPON_TYPE::kStaff)
        {
            return 0.0f;
        }

        const auto entry = player->GetEquippedEntryData(left);

        if (!entry || !entry->extraLists)
        {
            return 0.0f;
        }

        RE::EnchantmentItem *enchantment = nullptr;

        for (const auto &extraList : *entry->extraLists)
        {
            if (!extraList)
            {
                continue;
            }

            if (const auto extraEnchantment =
                    extraList->GetByType<RE::ExtraEnchantment>();
                extraEnchantment && extraEnchantment->enchantment)
            {
                enchantment = extraEnchantment->enchantment;
                break;
            }
        }

        if (!enchantment)
        {
            enchantment = staff->formEnchanting;
        }

        if (!enchantment ||
            enchantment->effects.empty() ||
            !enchantment->effects[0] ||
            !enchantment->effects[0]->baseEffect)
        {
            return 0.0f;
        }

        float scale = 1.0f;

        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModSpellMagnitude,
            player,
            enchantment,
            nullptr,
            &scale);

        return enchantment->effects[0]->effectItem.magnitude * scale;
    }

    float GetScrollDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto scroll = equippedObject
                                ? equippedObject->As<RE::ScrollItem>()
                                : nullptr;

        if (!scroll ||
            scroll->effects.empty() ||
            !scroll->effects[0] ||
            !scroll->effects[0]->baseEffect)
        {
            return 0.0f;
        }

        float scale = 1.0f;

        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModSpellMagnitude,
            player,
            scroll,
            nullptr,
            &scale);

        return scroll->effects[0]->effectItem.magnitude * scale;
    }

    float GetUnarmedDamage(RE::PlayerCharacter *player, RE::TESObjectWEAP *unarmedWeapon = nullptr)
    {
        if (!player)
        {
            return 0.0f;
        }

        auto *actorValueOwner = player->AsActorValueOwner();
        if (!actorValueOwner)
        {
            return 0.0f;
        }

        auto scale =
            actorValueOwner->GetActorValue(RE::ActorValue::kUnarmedDamage) *
            actorValueOwner->GetActorValue(RE::ActorValue::kAttackDamageMult);

        RE::BGSEntryPoint::HandleEntryPoint(
            RE::BGSEntryPoint::ENTRY_POINT::kModAttackDamage,
            player,
            unarmedWeapon, // Pass the 0x1F4 weapon form here!
            nullptr,
            &scale);

        return scale;
    }

    float GetHandDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);

        // Nothing equipped = unarmed.
        if (!equippedObject)
        {
            return GetUnarmedDamage(player);
        }

        // Weapon / Staff / Unarmed
        if (const auto weapon = equippedObject->As<RE::TESObjectWEAP>())
        {
            // Skyrim's built-in Unarmed weapon.
            if (weapon->formID == 0x000001F4)
            {
                return GetUnarmedDamage(player, weapon);
            }

            const auto weaponType = weapon->GetWeaponType();

            // Two-handed weapon.
            // Both hands should display the same damage.
            if (weaponType == RE::WEAPON_TYPE::kTwoHandSword ||
                weaponType == RE::WEAPON_TYPE::kTwoHandAxe ||
                weaponType == RE::WEAPON_TYPE::kBow ||
                weaponType == RE::WEAPON_TYPE::kCrossbow)
            {
                return GetWeaponDamage(player, false);
            }

            // Staff
            if (weaponType == RE::WEAPON_TYPE::kStaff)
            {
                return GetStaffDamage(player, left);
            }

            // One-handed weapon
            return GetWeaponDamage(player, left);
        }

        // Spell
        if (equippedObject->As<RE::SpellItem>())
        {
            return GetSpellDamage(player, left);
        }

        // Scroll
        if (equippedObject->As<RE::ScrollItem>())
        {
            return GetScrollDamage(player, left);
        }

        // Anything else = unarmed.
        return GetUnarmedDamage(player);
    }
}
