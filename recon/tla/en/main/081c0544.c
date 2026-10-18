// Draft, differ 3 of 4: the work copy of the mixer frame entry, as ☀️'s
// AudioEngine_RunMixer (games/common/src/sound/sound.c). Its call reaches a
// stale address before the far-call banks, which the draft names as the
// driver's tick. A sound-library function: the draft compiles with the game
// profile, where the build routes the library through old_agbcc with
// -mthumb-interwork. Other: only the return: the game profile's pop {pc}
// against the interworking pop {r0}; bx r0.
#include "types.h"

void AudioEngine_RunMixerTick(void);

void Func_081c0544(void) { AudioEngine_RunMixerTick(); }
