#include "unit/sound/empty_callback.h"
#include "types.h"

/// Provides the default empty callback before the sound driver installs its
/// handlers: SoundInit puts it in the work's cgbUpdate, cgbMute,
/// cgbFrequency and externalVolumePitch, and MPlayExtender replaces the
/// first three.
void DummyFunc(void) {}
