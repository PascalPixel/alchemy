#include "types.h"

void Func_020024f0(void);
void Func_02002528(const void *);
void Func_02002544(void);
void Func_02002560(s32, s32);
void Func_02002574(s32, s32, s32);
void Func_02002582();
void Func_0200258e(s32);
void Func_0200259a(s32, s32);
void Func_020025a6(s32, s32);
void Func_020025d2(s32, s32, s32);

static __inline__ void SetActorDirection(s32 actor, s32 direction, s32 duration)
{
    Func_020025d2(actor, direction, duration);
}

#define ActorPresentation_RunActorZeroCameraScene Func_02000b0c

void ActorPresentation_RunActorZeroCameraScene(void)
{
    Func_020024f0();
    Func_02002560(0, 1);
    Func_0200258e(9423);
    Func_020025a6(1, 0);
    SetActorDirection(0, 258, 100);
    Func_02002582(0, 2);
    Func_02002574(0, 0, 12);
    Func_02002582(0);
    Func_0200259a(0, 1);
    Func_02002528((const void *)0x243);
    Func_02002544();
}
