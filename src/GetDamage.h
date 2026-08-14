#pragma once

#include "RE/Skyrim.h"

namespace GetDamage
{
    float GetWeaponDamage(RE::PlayerCharacter *player, bool left);
    float GetSpellDamage(RE::PlayerCharacter *player, bool left);
    float GetStaffDamage(RE::PlayerCharacter *player, bool left);
    float GetScrollDamage(RE::PlayerCharacter *player, bool left);
    float GetHandDamage(RE::PlayerCharacter *player, bool left);
}