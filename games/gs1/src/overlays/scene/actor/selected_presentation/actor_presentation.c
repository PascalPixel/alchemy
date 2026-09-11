#include "types.h"

#define FieldScene_RunActor8Step Func_02000558
#define FieldScene_RunActor9Step Func_02000564
#define FieldScene_RunActor10Step Func_02000570
#define FieldScene_RunActor11Step Func_0200057c
#define FieldScene_RunActor12Step Func_02000588
#define FieldScene_RunLateActor8Step Func_020007fc
#define FieldScene_RunLateActor9Step Func_02000808
#define FieldScene_RunLateActor10Step Func_02000814
#define FieldScene_RunLateActor11Step Func_02000820
#define FieldScene_RunLateActor12Step Func_0200082c
#define PlaceActorTwelveWhenFlagClear Func_02000adc
void Func_020009c2(int actor);
void Func_020009ce(int actor);
void Func_020009da(int actor);
void Func_020009e6(int actor);
void Func_020009f2(int actor);
void Func_02000d96(int actor);
void Func_02000da2(int actor);
void Func_02000dae(int actor);
void Func_02000dba(int actor);
void Func_02000dc6(int actor);
s32 Func_02001d2e(s32);
void Func_02001d40(s32);
void Func_02001d9e(s32, const void *);
void Func_02001dc6(s32, s32, s32);

/* Actor-step entry points for the two scene phases. */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001dc6(actor, x, y);
}

void FieldScene_RunActor8Step(void) { Func_020009c2(8); }

void FieldScene_RunActor9Step(void) { Func_020009ce(9); }

void FieldScene_RunActor10Step(void) { Func_020009da(10); }

void FieldScene_RunActor11Step(void) { Func_020009e6(11); }

void FieldScene_RunActor12Step(void) { Func_020009f2(12); }

void FieldScene_RunLateActor8Step(void) { Func_02000d96(8); }

void FieldScene_RunLateActor9Step(void) { Func_02000da2(9); }

void FieldScene_RunLateActor10Step(void) { Func_02000dae(10); }

void FieldScene_RunLateActor11Step(void) { Func_02000dba(11); }

void FieldScene_RunLateActor12Step(void) { Func_02000dc6(12); }

void PlaceActorTwelveWhenFlagClear(void)
{
    if (Func_02001d2e(2487) == 0) {
        Func_02001d40(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Func_02001d9e(12, (const void *)0x020097a8);
    }
}
