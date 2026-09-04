#include "types.h"

#define SceneAudio_PlayCue118AndReturnZero Func_02000d48

/* Play the footprint-motion completion cue. */
extern void Func_02003362(s32 cue);
s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Func_02003362(118);
    return 0;
}
