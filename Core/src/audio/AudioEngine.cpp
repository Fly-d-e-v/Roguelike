#include "AudioEngine.h"

#include  "FMOD/fmod.hpp"
#include  "FMOD/fmod_errors.h"
#include  "FMOD/fmod_common.h"
#include  "FMOD/fmod_studio.hpp"
#include  "FMOD/fmod_studio_common.h"

std::shared_ptr<AudioEngine> AudioEngine::s_Instance = std::make_shared<AudioEngine>();

std::shared_ptr<AudioEngine> AudioEngine::Instance() {
    if (s_Instance == nullptr) {
        s_Instance = std::make_shared<AudioEngine>();
    }
    return s_Instance;
}

bool AudioEngine::Init() {

    FMOD_RESULT result;
    FMOD::Studio::System* system = NULL;

    result = FMOD::Studio::System::create(&system); // Create the Studio System object.
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return false;
    }

    // Initialize FMOD Studio, which will also initialize FMOD Core
    result = system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        return false;
    }

    return false;
}
