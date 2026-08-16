#include <spdlog/sinks/basic_file_sink.h>

#include "Utility.h"
#include "EventProcessor.h"
#include "Scaleform.h"
#include "CharacterSheet.h"
#define SMOOTHCAM_API_COMMONLIB
#include "SmoothCamAPI.h"
#include "APIManager.h"
#include "Serialization.h"
#include "PoiseAPI.h"
#include "ArmorResistance.h"
#include "CriticalCalcs.h"

void SKSEMessageHandler(SKSE::MessagingInterface::Message *message)
{
    auto eventProcessor = EventProcessor::GetSingleton();

    switch (message->type)
    {
    case SKSE::MessagingInterface::kDataLoaded:
        APIs::RequestAPIs();
        CriticalCalcs::Initialize();

        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(
            eventProcessor);

        Scaleform::CharacterSheet::Register();
        break;

    case SKSE::MessagingInterface::kInputLoaded:
        RE::BSInputDeviceManager::GetSingleton()
            ->AddEventSink<RE::InputEvent *>(eventProcessor);

        SKSE::GetModCallbackEventSource()->AddEventSink(eventProcessor);
        break;

    case SKSE::MessagingInterface::kPostLoadGame:
    case SKSE::MessagingInterface::kPostPostLoad:
    case SKSE::MessagingInterface::kPostLoad:
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kSaveGame:
    default:
        break;
    }
}

extern "C" [[maybe_unused]] __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface *skse)
{
    SKSE::Init(skse);

    SetupLog();

    auto *ser = SKSE::GetSerializationInterface();
    ser->SetUniqueID('CTTL');
    ser->SetRevertCallback(RevertCallback);
    ser->SetSaveCallback(SaveCallback);
    ser->SetLoadCallback(LoadCallback);

    SKSE::GetMessagingInterface()->RegisterListener(SKSEMessageHandler);
    pluginHandle = skse->GetPluginHandle();

    LoadDataFromINI();

    logger::info("========================================");
    logger::info("Character Menu initialization");
    logger::info("debugMode = {}", debugMode);
    logger::info("========================================");

    LoadFactionDefinitions();

    logger::info("Character Sheet successfully loaded.");

    return true;
}
