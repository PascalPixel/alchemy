#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_0200565a();
void Func_02005676();
void Func_02005684();
u8 *Scene_GetRecord_1();
void Func_020056b0();
u8 *Scene_GetRecord_2();
void Func_02005702();
u8 *ObjectMotion_SetHorizontalPositionWithTerrain_1();
void Func_02005720();
void Func_0200575a();
void Func_0200578a();
void Func_02005794();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_0200585a();
void Func_02005874();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Scene_GetRecord_1(args...) Func_0200568a(args)
#define GameFlag_IsSet_1(a0) Value1(Func_0200565a, a0)
#define BattleRuntime_Reset_1(args...) Func_02005684(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_0200578a, a0, a1, a2, a3)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(args...) Func_02005712(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_0200575a(args)
#define Scene_GetRecord_2(args...) Func_020056d8(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020057fa(args)
#define ObjectMotion_SetSpeedParameters_1(args...) Func_0200580e(args)
#define Audio_PlayCue_1(args...) Func_0200583c(args)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_02005702, a0, a1, a2)
#define ObjectMotion_CommitPositionAndActivate_1(args...) Func_0200583c(args)
#define ObjectGroup_ConfigureChildValue_2(args...) Func_02005794(args)
#define Scene_GetRecord_3(args...) Func_02005712(args)
#define ObjectMotion_CommitPositionAndActivate_2(args...) Func_0200585a(args)
#define BattleEffect_PlayQueuedSound_1(args...) Func_02005874(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02005720(args)
u8 *Func_0200568a();
u8 *Func_02005712();
u8 *Func_020056d8();
u8 *Func_02005712();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Scene_GetRecord_1(0);
    guard = GameFlag_IsSet_1(0x109);
    if (guard == 0) {
        BattleRuntime_Reset_1();
        ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        ObjectMotion_SetHorizontalPositionWithTerrain_1(0, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        ObjectGroup_ConfigureChildValue_1(0, 15);
        sub = Scene_GetRecord_2(0);
        Func_02005676(sub, 0);
        BattleRuntime_WaitIfModeZero_1();
        ObjectMotion_SetSpeedParameters_1();
        Audio_PlayCue_1(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = 0x20086a1;
        ObjectMotion_SetSpeedParameters_2(0, 0x6666, 0x3333);
        ObjectMotion_CommitPositionAndActivate_1(0, 0, 8);
        ObjectGroup_ConfigureChildValue_2(0, 0);
        sub = Scene_GetRecord_3(0);
        Func_020056b0(sub, 1);
        ObjectMotion_CommitPositionAndActivate_2(0, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleEffect_PlayQueuedSound_1();
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}
