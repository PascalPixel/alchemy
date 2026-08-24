#include "resource_384.h"

extern void Func_02000404(void);
extern void Func_0200044a(s32);
extern void Func_0200045a(s32, s32);
extern void Func_0200041e(void);

#define BeginActor9Beat    Func_02000404
#define PlayActor9BeatCue  Func_0200044a
#define ApplyActor9Beat    Func_0200045a
#define EndActor9Beat      Func_0200041e

void Func_02000190(void)
{
    BeginActor9Beat();
    PlayActor9BeatCue(RESOURCE384_CUE_ACTOR_9_BEAT);
    ApplyActor9Beat(RESOURCE384_ACTOR_9, 0);
    EndActor9Beat();
}
