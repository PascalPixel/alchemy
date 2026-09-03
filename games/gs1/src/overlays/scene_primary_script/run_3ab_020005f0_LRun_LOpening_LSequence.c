#include "types.h"

#define FieldScene_RunOpeningSequence Func_020005f0

extern u8 Value_02008469;
void Func_02001fa8();
void Func_02001fc2();
void Func_02001fd4();
void Func_02001fd6();
void Func_02001fea();
void *Scene_GetRecord_1();
void Func_02002000();
void Func_02002042();
void Func_02002046();
void Func_0200204c();
void Func_02002050();
void Func_02002072();
void *Scene_GetRecord_2();
s32 Func_02002098();
void Func_020020d0();
void Func_0200214c();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(args...) Func_02001ff8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_02002042, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_0200204c, a0, a1, a2)
#define EffectRuntime_SetCurrentPosition_1(a0, a1, a2) Call3(Func_0200214c, a0, a1, a2)
#define GameFlag_Clear_1(a0) Call1(Func_02002050, a0)
#define Object_SetModeById_1(a0, a1) Call2(Func_020020d0, a0, a1)
#define Scene_GetRecord_2(args...) Func_0200208e(args)
#define Scene_GetRecord_3(a0) Value1(Func_02002098, a0)
#define GameFlag_Set_1(a0) Value1(Func_02002072, a0)
void *Func_02001ff8();
void *Func_0200208e();
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


/* Sets up a handful of numbered records with position/pose-shaped argument
 * groups, then clears the s32 field at +108 of record 8 and feeds record 8's
 * id back into two more calls before two final single-argument calls. */
void Func_020005f0(void)
{
    void *unused;
    void *record8;

    unused = Scene_GetRecord_1(0);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(19, 0, 0);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(8, 0, 0);
    Call6(Func_02001fa8, 38, 38, 1, 1, 46, 4); /* main:080091b8 */
    Call6(Func_02001fc2, 37, 37, 3, 3, 13, 3); /* main:080091c0 */
    Call6(Func_02001fd6, 37, 37, 1, 1, 14, 2); /* main:080091c0 */
    Call6(Func_02001fea, 8, 16, 1, 1, 7, 16); /* main:080091c0 */
    EffectRuntime_SetCurrentPosition_1(102, 0, 0);
    Call6(Func_02002000, 32, 42, 3, 2, 1, 15); /* main:080091b8 */
    GameFlag_Clear_1(512);
    Object_SetModeById_1(8, 1);
    record8 = Scene_GetRecord_2(8);
    /* Clear the s32 field at offset 108 of record 8. */
    *(s32 *)(record8 + 108) = 0;
    Value2(Func_02002046, Scene_GetRecord_3(8), 0); /* main:08009240 */
    Value1(Func_02001fd4, (s32)&Value_02008469); /* main:080000d8 */
    GameFlag_Set_1(0x201);
}
