#include "resource_384.h"

extern void Func_02000384(void);
extern s32 Func_020003b4(s32, s32);
extern void Func_0200038a(s32);
extern void Func_020003d8(s32);
extern void Func_020003e8(s32, s32);
extern void Func_020003ac(void);

#define BeginActor9Intro     Func_02000384
#define SetActor9Mode        Func_020003b4
#define WaitActor9Intro      Func_0200038a
#define PlayActor9IntroCue   Func_020003d8
#define FinishActor9Intro    Func_020003e8
#define EndActor9Intro       Func_020003ac

void Func_02000110(void)
{
    BeginActor9Intro();
    SetActor9Mode(RESOURCE384_ACTOR_9, 2);
    WaitActor9Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor9IntroCue(RESOURCE384_CUE_ACTOR_9_INTRO);
    FinishActor9Intro(RESOURCE384_ACTOR_9, 0);
    EndActor9Intro();
}
