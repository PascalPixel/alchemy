#include "types.h"

#define SceneAudio_PlayCue123AndDispatchWork364 Func_02001748
extern u8 *Data_03001ebc;

void Func_020034b8(s32 cue);
void Func_020034a6(s32 val);

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */

void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    s32 val = *(s16 *)(Data_03001ebc + 364);

    Func_020034b8(123);
    Func_020034a6(val);
}
