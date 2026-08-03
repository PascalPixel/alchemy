#include "resource_384.h"

extern void Func_020003b4(void);
extern s32 Func_020003d4(s32, s32);
extern void Func_020003ba(s32);
extern void Func_02000408(s32);
extern void Func_02000418(s32, s32);
extern void Func_020003dc(void);

#define BeginActor10Intro    Func_020003b4
#define SetActor10Mode       Func_020003d4
#define WaitActor10Intro     Func_020003ba
#define PlayActor10IntroCue  Func_02000408
#define FinishActor10Intro   Func_02000418
#define EndActor10Intro      Func_020003dc

void Func_02000140(void)
{
    BeginActor10Intro();
    SetActor10Mode(RESOURCE384_ACTOR_10, 4);
    WaitActor10Intro(RESOURCE384_ACTOR_FLAG);
    PlayActor10IntroCue(RESOURCE384_CUE_ACTOR_10_INTRO);
    FinishActor10Intro(RESOURCE384_ACTOR_10, 0);
    EndActor10Intro();
}
