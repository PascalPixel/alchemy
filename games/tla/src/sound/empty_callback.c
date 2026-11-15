#include "types.h"

/// Provides the default empty callback before the sound driver installs its
/// handlers.
// As ☀️'s DummyFunc.
/// Provides the default empty callback before the sound driver installs its
/// handlers: SoundInit puts it in the work's cgbUpdate, cgbMute,
/// cgbFrequency and externalVolumePitch, and MPlayExtender replaces the
/// first three.
void DummyFunc(void) {}
