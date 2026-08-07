#include "ChocolatePoiseIntegration.h"
#include "../API/ChocolatePoiseAPI.h"

namespace PoiseIntegration
{
    float GetArmorPoiseDefense(RE::Actor *actor)
    {
        if (!actor ||
            !ChocolatePoiseAPI::Loaded ||
            !ChocolatePoiseAPI::GetArmorReducedStagger)
        {
            return 0.0f;
        }

        float reduced = ChocolatePoiseAPI::GetArmorReducedStagger(
            actor->GetFormID(),
            100.0f);

        return 100.0f - reduced;
    }
}