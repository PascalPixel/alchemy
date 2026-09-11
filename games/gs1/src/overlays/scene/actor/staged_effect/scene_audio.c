#include "types.h"

#define SceneAudio_PlayCue118AndReturnZero Func_02000d48
void Func_02003362(s32 cue);

/* Play the footprint-motion completion cue. */

s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Func_02003362(118);
    return 0;
}
