#include "resource_384.h"

extern void Func_020003e4(void);
extern void Func_0200042a(s32);
extern void Func_0200043a(s32, s32);
extern void Func_020003fe(void);

#define BeginActor8Beat    Func_020003e4
#define PlayActor8BeatCue  Func_0200042a
#define ApplyActor8Beat    Func_0200043a
#define EndActor8Beat      Func_020003fe

void Func_02000170(void)
{
    BeginActor8Beat();
    PlayActor8BeatCue(RESOURCE384_CUE_ACTOR_8_BEAT);
    ApplyActor8Beat(RESOURCE384_ACTOR_8, 0);
    EndActor8Beat();
}
