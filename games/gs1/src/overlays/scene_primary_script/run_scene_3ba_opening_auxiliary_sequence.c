#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_0200004c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c41c[];
void Func_02003c7e();
void Func_02003c90();
void Func_02003c9a();
void Func_02003cc0();
void Func_02003cd2();
void Func_02003cf0();
void Func_02003d02();
void Func_02003d14();
void Func_02003daa();
void Func_02003dda();
void Func_02003e0a();

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */
#define Object_SetModeById_1(args...) Func_02003dda(args)
#define Object_SetModeById_2(args...) Func_02003daa(args)
#define Object_SetModeById_3(args...) Func_02003dda(args)
#define Object_SetModeById_4(args...) Func_02003e0a(args)

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

/* A countdown word this overlay owns at Data_0200c41c: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */
#define AUX_COUNTDOWN (*(volatile s32 *)Data_0200c41c)

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void FieldScene_RunOpeningAuxiliarySequence(void)
{
    switch ((u32)AUX_COUNTDOWN) {
    case 66:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_1(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Call6(Func_02003c7e, 92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003c90, 92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Call6(Func_02003c9a, 50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Object_SetModeById_2(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_3(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Call6(Func_02003cf0, 92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003d02, 92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_4(16, 12); /* object 16, action 12 */
        Call6(Func_02003d14, 50, 24, 6, 1, 50, 12); /* main:080091c0 */
        AUX_COUNTDOWN = 120;
        break;
    }
    AUX_COUNTDOWN = AUX_COUNTDOWN - 1;
}
