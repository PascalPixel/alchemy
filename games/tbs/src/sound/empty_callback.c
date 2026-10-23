#include "unit/sound/empty_callback.h"
#include "types.h"

/// Provides the default empty callback before the sound driver installs its
/// handlers: AudioEngine_Initialize puts it in the work's cgbUpdate, cgbMute,
/// cgbFrequency and externalVolumePitch, and CgbAudio_Initialize replaces the
/// first three.
void Audio_EmptyCallback(void) {}
