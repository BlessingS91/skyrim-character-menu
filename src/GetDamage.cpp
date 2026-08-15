#include "GetDamage.h"
#include "Utility.h"

namespace GetDamage
{
    float GetWeaponDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            logger::info("GetWeaponDamage | player=null");
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);
        const auto weapon = equippedObject
                                ? equippedObject->As<RE::TESObjectWEAP>()
                                : nullptr;

        logger::info(
            "GetWeaponDamage | hand={} | object={} | weapon={}",
            left ? "left" : "right",
            static_cast<void *>(equippedObject),
            static_cast<void *>(weapon));

        if (!weapon)
        {
            logger::info("GetWeaponDamage | no weapon");
            return 0.0f;
        }

        const auto equippedEntry = player->GetEquippedEntryData(left);

        logger::info(
            "GetWeaponDamage | hand={} | weapon={} | entry={}",
            left ? "left" : "right",
            weapon->GetName(),
            static_cast<void *>(equippedEntry));

        float damage = 0.0f;

        const auto weaponType = weapon->GetWeaponType();

        // ============================================================
        // ARROW / BOLT DAMAGE
        // ============================================================
        if (weaponType == RE::WEAPON_TYPE::kBow ||
            weaponType == RE::WEAPON_TYPE::kCrossbow)
        {
            const auto ammo = player->GetCurrentAmmo();

            if (ammo)
            {
                const float ammoDamage =
                    ammo->GetRuntimeData().data.damage;

                damage = ammoDamage;

                logger::info(
                    "GetWeaponDamage | {} | ammo={} | ammoDamage={} | damage={}",
                    weaponType == RE::WEAPON_TYPE::kBow
                        ? "bow"
                        : "crossbow",
                    ammo->GetName(),
                    ammoDamage,
                    damage);
            }
            else
            {
                logger::info(
                    "GetWeaponDamage | {} | no current ammo",
                    weaponType == RE::WEAPON_TYPE::kBow
                        ? "bow"
                        : "crossbow");
            }
        }

        // ============================================================
        // WEAPON DAMAGE
        // ============================================================
        if (equippedEntry)
        {
            const float weaponDamage =
                player->GetDamage(equippedEntry);

            logger::info(
                "GetWeaponDamage | GetDamage={} | previous={} | final={}",
                weaponDamage,
                damage,
                damage + weaponDamage);

            damage += weaponDamage;
        }
        else
        {
            logger::info(
                "GetWeaponDamage | equippedEntry=null");
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

    float GetHandDamage(RE::PlayerCharacter *player, bool left)
    {
        if (!player)
        {
            logger::info("GetHandDamage | player=null");
            return 0.0f;
        }

        const auto equippedObject = player->GetEquippedObject(left);

        logger::info(
            "GetHandDamage | hand={} | equippedObject={}",
            left ? "left" : "right",
            static_cast<void *>(equippedObject));

        // ============================================================
        // UNARMED
        // ============================================================
        if (!equippedObject)
        {
            const float unarmedDamage =
                player->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kUnarmedDamage);

            const float attackDamageMult =
                player->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kAttackDamageMult);

            float damage = unarmedDamage * attackDamageMult;

            // Hand to Hand adds 0.35 damage per point of Hand to Hand.
            const float handToHand =
                player->AsActorValueOwner()->GetActorValue(
                    RE::ActorValue::kLockpicking);

            damage += handToHand * 0.35f;

            RE::BGSEntryPoint::HandleEntryPoint(
                RE::BGSEntryPoint::ENTRY_POINT::kModAttackDamage,
                player,
                nullptr,
                nullptr,
                &damage);

            return damage;
        }
        // ============================================================
        // WEAPON
        // ============================================================
        if (const auto weapon = equippedObject->As<RE::TESObjectWEAP>())
        {
            logger::info(
                "GetHandDamage | {} weapon | name={} | type={}",
                left ? "left" : "right",
                weapon->GetName(),
                static_cast<int>(weapon->GetWeaponType()));

            // Two-handed weapons and bows use the right-hand damage
            // for both sides of the character sheet.
            const auto weaponType = weapon->GetWeaponType();

            const bool isTwoHanded =
                weaponType == RE::WEAPON_TYPE::kTwoHandSword ||
                weaponType == RE::WEAPON_TYPE::kTwoHandAxe ||
                weaponType == RE::WEAPON_TYPE::kBow ||
                weaponType == RE::WEAPON_TYPE::kCrossbow;

            if (isTwoHanded)
            {
                const float damage = GetWeaponDamage(player, false);

                logger::info(
                    "GetHandDamage | {} two-handed | using right-hand damage={}",
                    left ? "left" : "right",
                    damage);

                return damage;
            }

            // Staffs use their enchantment magnitude rather than
            // physical weapon damage.
            if (weaponType == RE::WEAPON_TYPE::kStaff)
            {
                const float damage = GetStaffDamage(player, left);

                logger::info(
                    "GetHandDamage | {} staff | damage={}",
                    left ? "left" : "right",
                    damage);

                return damage;
            }

            const float damage = GetWeaponDamage(player, left);

            logger::info(
                "GetHandDamage | {} weapon | final={}",
                left ? "left" : "right",
                damage);

            return damage;
        }

        // ============================================================
        // SPELL
        // ============================================================
        if (equippedObject->As<RE::SpellItem>())
        {
            const float damage = GetSpellDamage(player, left);

            logger::info(
                "GetHandDamage | {} spell | final={}",
                left ? "left" : "right",
                damage);

            return damage;
        }

        // ============================================================
        // SCROLL
        // ============================================================
        if (equippedObject->As<RE::ScrollItem>())
        {
            const float damage = GetScrollDamage(player, left);

            logger::info(
                "GetHandDamage | {} scroll | final={}",
                left ? "left" : "right",
                damage);

            return damage;
        }

        logger::info(
            "GetHandDamage | {} | unknown equipped object type -> 0",
            left ? "left" : "right");

        return 0.0f;
    }
}