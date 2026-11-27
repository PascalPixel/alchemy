/* @file */
/* The sound driver's empty callback. */

#include "types.h"

/* Does nothing: SoundInit points the CGB hooks and the external volume hook */
/* at it, and MPlayExtender then replaces the CGB ones. */
void DummyFunc(void) {}
