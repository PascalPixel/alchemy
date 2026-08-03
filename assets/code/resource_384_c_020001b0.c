#include "resource_384.h"

extern void Func_02000424(void);
extern void Func_0200046a(s32);
extern void Func_0200047a(s32, s32);
extern void Func_0200043e(void);

#define BeginActor10Beat    Func_02000424
#define PlayActor10BeatCue  Func_0200046a
#define ApplyActor10Beat    Func_0200047a
#define EndActor10Beat      Func_0200043e

void Func_020001b0(void)
{
    BeginActor10Beat();
    PlayActor10BeatCue(RESOURCE384_CUE_ACTOR_10_BEAT);
    ApplyActor10Beat(RESOURCE384_ACTOR_10, 0);
    EndActor10Beat();
}
