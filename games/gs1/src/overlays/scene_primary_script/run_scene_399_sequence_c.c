#include "types.h"

#define RunEventScript02 Func_020019bc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003bf0();
void Func_02003c16();
void Func_02003c20();
void Func_02003c5a();
void Func_02003d44();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define BattleRuntime_Reset_1(args...) Func_02003bf0(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_02003d44(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02003c20, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_1(args...) Func_02003c5a(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Func_02003c16(args)

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

/* Runs a short fixed sequence of two calls, one 3-argument call passing a
 * fixed-point-looking pair of constants, one 3-argument call passing
 * (0, 232, 204), and a final call, in that order. */
void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(0, 0x20000, 0x1999);
    ObjectMotion_SetPositionAndCommit_1(0, 232, 204);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}
