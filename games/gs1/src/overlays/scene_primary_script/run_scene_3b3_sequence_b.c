#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020026e0();
void Func_020026e6();
void Func_020026ec();
void Func_020026f2();
void Func_020026f8();
void Func_020026fe();
void Func_02002704();
void Func_02002822();
void Func_02002828();
void Func_0200282e();
void Func_02002834();
void Func_0200283a();
void Func_02002840();
void Func_02002846();
void Func_02002e0e();
void Func_02002ec0();
void Func_02004244();
void Func_0200426e();
s32 Func_0200427e();
void Func_0200428c();
void Func_02004296();
void Func_020042b2();
s32 Func_020042de();
u8 *Scene_GetRecord_2();
void Func_020042ea();
void Func_02004302();
void Func_02004306();
void Func_0200430e();
void Func_02004314();
void Func_0200432a();
void Func_02004330();
void Func_0200433a();
void Func_0200434c();
void Func_02004352();
void Func_0200435a();
void Func_0200435c();
void Func_02004368();
void Func_02004374();
s32 Func_02004398();
u8 *Scene_GetRecord_4();
void Func_020043c6();
void Func_020043e6();
void Func_020043f6();
void Func_02004406();
void Func_02004414();
void Func_02004422();
void Func_0200442a();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_0200426e(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Func_020042ea, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_02004302, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_0200430e(args)
#define UiText_DrawMessage_1(a0, a1) Call2(Func_0200426e, a0, a1)
#define GameFlag_IsSet_1(a0) Value1(Func_0200427e, a0)
#define Audio_PlayCue_1(args...) Func_02004352(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020042b2(args)
#define Audio_PlayCue_2(args...) Func_02004368(args)
#define ObjectMotion_SetActionVariant_1(args...) Func_02004330(args)
#define Scene_GetRecord_1(args...) Func_020042de(args)
#define Scene_GetRecord_2(args...) Func_020042e8(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200432a, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_0200433a(args)
#define GameFlag_Set_1(a0) Call1(Func_0200434c, a0)
#define Audio_PlayCue_3(args...) Func_02004414(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_02004374(args)
#define Audio_PlayCue_4(args...) Func_0200442a(args)
#define Scene_GetRecord_3(args...) Func_02004398(args)
#define Scene_GetRecord_4(args...) Func_020043a4(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Call3(Func_020043e6, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_020043f6(args)
#define GameFlag_Clear_1(a0) Call1(Func_02004406, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02004422(args)
u8 *Func_020042e8();
u8 *Func_020043a4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Record id used for the single record configured below in either branch. */
#define REC_ID 16

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */
#define QUERY_FLAG 0x200

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0x10000, 0x2000);
    ObjectMotion_PlaceWithinCameraBounds_1(0x1190000, -1, 0x1b00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    UiText_DrawMessage_1(0x1528, 1);
    queried = GameFlag_IsSet_1(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue_1(232);
        Call3(Func_02004244, 0x200ada8, 84, 24);
        BattleRuntime_WaitIfModeZero_1(30);
        Audio_PlayCue_2(240);
        ObjectMotion_SetActionVariant_1(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Scene_GetRecord_2(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        ObjectMotion_SetHorizontalPositionWithTerrain_1(REC_ID, 0x1100000, 0x1a00000);
        Object_SetModeById_1(REC_ID, 1);
        Call3(Func_0200428c, 0x200adfc, 80, 24);
        Call3(Func_02004296, 0x200ae50, 80, 28);
        Call6(Func_020042b2, 65, 40, 16, 27, 2, 4);
        Func_02002e0e();
        Func_020026e0(9);
        Func_020026e6(10);
        Func_020026ec(11);
        Func_020026f2(12);
        Func_020026f8(13);
        Func_020026fe(14);
        Func_02002704(15);
        Call6(Func_02004314, 24, 3, 1, 1, 24, 8);
        GameFlag_Set_1(QUERY_FLAG);
    } else {
        Audio_PlayCue_3(232);
        Call3(Func_02004306, 0x200add2, 84, 24);
        BattleRuntime_WaitIfModeZero_2(30);
        Audio_PlayCue_4(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Scene_GetRecord_4(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        ObjectMotion_SetHorizontalPositionWithTerrain_2(REC_ID, 0x1100000, 0x1b40000);
        Object_SetModeById_2(REC_ID, 2);
        Call6(Func_0200435a, 65, 45, 16, 27, 2, 4);
        Call3(Func_0200435c, 0x200ae26, 80, 24);
        Func_02002ec0();
        Func_02002822(9);
        Func_02002828(10);
        Func_0200282e(11);
        Func_02002834(12);
        Func_0200283a(13);
        Func_02002840(14);
        Func_02002846(15);
        Call6(Func_020043c6, 24, 4, 1, 1, 24, 8);
        GameFlag_Clear_1(QUERY_FLAG);
    }
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
