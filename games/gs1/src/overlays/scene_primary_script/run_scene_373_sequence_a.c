#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006ec8();
void Func_02006fc4();
void Func_020070aa();
void Func_020070de();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Audio_PlayCue_1(args...) Func_020070de(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02006fc4, a0, a1, a2)

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Runs a short scripted step, then two 3-argument setup calls, then another
 * short scripted step; none of the callees' effects are visible here. */
void FieldScene_RunScene373SequenceA(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02006ec8, 0x200f586, 52, 76); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x176, 0x4d6); /* object_id 0, x 0x176, z 0x4d6 */
    Func_020070aa(9); /* main:0808a248 */
}
