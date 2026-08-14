#include "GetDamage.h"

namespace GetDamage
{
    float GetWeaponDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto weapon = equippedObject ? equippedObject->As<RE::TESObjectWEAP>() : nullptr;

        if (!weapon)
        {
            return 0.0f;
        }

        const auto equippedEntry = player->GetEquippedEntryData(left);

        float damage = 0.0f;

        // Match the existing WidgetEquip calculation for bows.
        if (player->GetCurrentAmmo() &&
            weapon->HasKeywordString("WeapTypeBow"))
        {

            float scale = 1.0f;

            RE::BGSEntryPoint::HandleEntryPoint(
                RE::BGSEntryPoint::ENTRY_POINT::kModAttackDamage,
                player,
                nullptr,
                nullptr,
                &scale);

            damage = player->GetCurrentAmmo()->data.damage * scale;
        }

        // Skyrim's equipped weapon damage calculation.
        if (equippedEntry)
        {
            damage += player->GetDamage(equippedEntry);
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
        const auto spell = equippedObject ? equippedObject->As<RE::SpellItem>() : nullptr;

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
        const auto staff = equippedObject ? equippedObject->As<RE::TESObjectWEAP>() : nullptr;

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

        // Fall back to the weapon's base enchantment.
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
        const auto scroll = equippedObject ? equippedObject->As<RE::ScrollItem>() : nullptr;

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

    float GetHandDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);

        // Unarmed.
        if (!equippedObject)
        {
            float damage =
                player->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kUnarmedDamage) *
                player->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kAttackDamageMult);

            RE::BGSEntryPoint::HandleEntryPoint(
                RE::BGSEntryPoint::ENTRY_POINT::kModAttackDamage,
                player,
                nullptr,
                nullptr,
                &damage);

            return damage;
        }

        // Weapon / bow / crossbow / etc.
        if (equippedObject->As<RE::TESObjectWEAP>())
        {
            const auto weapon = equippedObject->As<RE::TESObjectWEAP>();

            if (weapon->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
            {
                return GetStaffDamage(player, left);
            }

            return GetWeaponDamage(player, left);
        }

        // Spell.
        if (equippedObject->As<RE::SpellItem>())
        {
            return GetSpellDamage(player, left);
        }

        // Scroll.
        if (equippedObject->As<RE::ScrollItem>())
        {
            return GetScrollDamage(player, left);
        }

        return 0.0f;
    }
}