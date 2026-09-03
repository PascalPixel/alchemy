#include "types.h"

#define FieldScene_RunScene3c8SequenceB Func_02002f30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002fc4();
void Func_02007dc8();
void Func_02007df6();
void Func_02007e08();
void Func_02007e22();
void Func_02007e2c();
void Func_02007e92();
void Func_02007eb8();
void Func_02007f26();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02007dc8(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02007df6, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02007e2c, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02007eb8, a0, a1, a2)
#define BattleEffect_RunRisingObjectSequence_1(args...) Func_02007e92(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02007e08(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02007e22(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void FieldScene_RunScene3c8SequenceB(void)
{
    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0x8000, 0x4000);
    ObjectMotion_SetPositionAndReset_1(0, 0x208, 0x2c8);
    ObjectMotion_ArmCallback_1(0, 0x4000, 10);
    (void)Func_02002fc4(0x2080000, 0, 0x3100000, 223);
    BattleEffect_RunRisingObjectSequence_1(0, 6, 0);
    BattleRuntime_WaitIfModeZero_1(60);
    Func_02007f26(20); /* main:0808a248 */
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
