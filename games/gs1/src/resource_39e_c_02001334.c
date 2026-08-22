#include "types.h"

/*
 * resource_39e owner at 0x02001334, 132 bytes: a four-way facing selector.
 * It reads slot 0's actor record once and dispatches to one of four
 * overlay-local branches according to the record's unsigned halfword at +6.
 *
 * Complete owner: `push {r5, lr}` at 0x02001334 through
 * `pop {r5} / pop {r0} / bx r0` at 0x020013a2-0x020013a6, then the
 * four-word literal pool 0x020013a8-0x020013b7. The next owner's
 * prologue is exactly at 0x020013b8. **132 bytes**, measured to the
 * epilogue.
 *
 * The boundary is witnessed by the prologue at 0x02001334, the
 * interworking return at 0x020013a2-0x020013a6, the four-word pool through
 * 0x020013b7, and the next prologue at 0x020013b8. The span is therefore
 * 132 bytes. The 11 branch-and-link sites were resolved against the local
 * overlay import table; four of them target the neighboring selector
 * branches at 0x02000e94, 0x02000bf4, 0x02000db4, and 0x02000cd4.
 *
 * The selector fetches the angle once (unlike
 * 0x020012e0, which re-fetches) into r2 as the u16 at record[6], and each
 * test rotates it by a different bias before comparing against a quarter
 * turn:
 *   angle - 0x2000 <= 0x3fff  -> RunFacingVariantA
 *   angle - 0x6000 <= 0x3fff  -> RunFacingVariantB
 *   angle + 0x6000 <= 0x3fff  -> RunFacingVariantC
 *   otherwise                 -> RunFacingVariantD
 * This partitions the unsigned 16-bit value at the four witnessed
 * boundaries 0x2000, 0x6000, 0xa000, and 0xe000. The branch labels below
 * are deliberately descriptive but do not assign identities to the
 * scene's four local variants.
 *
 * The compiler spells the FIRST comparison in the high half -- it shifts
 * the biased angle left 16 and compares against 0x3fff0000 -- and the
 * other two in the low half, with the `lsls #16 / lsrs #16` zero-extend
 * pair. Both spellings compute the same predicate. The first is written
 * here in its shifted form so the asm is recoverable from the source; a
 * reader who normalises it silently loses the fact that GCC picked two
 * different encodings for one idiom inside one function.
 *
 * The two negative biases and the pre-shifted bound are pool words
 * (0xffffe000, 0xffffa000, and 0x3fff0000); the positive 0x6000 bias is
 * formed as 192 << 7, and the unshifted 0x3fff is the fourth pool word.
 *
 * The +6 field is called `facing` only because the neighboring selector at
 * 0x020012e0 reads the same field and compares it with the same quarter-turn
 * constants. The two mode calls and their numeric arguments are transcribed
 * from the call sites; their higher-level scene meaning is not assumed.
 */

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

extern void Func_02005722(s32 frames);
extern void Func_0200571c(void);
extern void Func_02005784(void);
extern struct SceneActor_02001334 *Func_0200573e(s32 slot);
extern void Func_0200579c(s32 slot, s32 mode);
extern void Func_020057ea(s32 slot, s32 mode);
extern void Func_02001f68(void);
extern void Func_0200213c(void);
extern void Func_02002062(void);
extern void Func_020021f6(void);
extern void Func_020023c8(s32 variant);

#define AdvanceSceneStep       Func_02005722
#define OpenSceneSection        Func_0200571c
#define CloseSceneSection       Func_02005784
#define GetSceneRecord          Func_0200573e
#define SetSceneActorMode       Func_0200579c
#define SetSceneActorModeAfterBranch Func_020057ea
#define RunFacingVariantA       Func_020021f6
#define RunFacingVariantB       Func_02001f68
#define RunFacingVariantC       Func_0200213c
#define RunFacingVariantD       Func_02002062
#define RunSceneVariant         Func_020023c8
#define RunFacingSceneSelector  Func_02001334

void RunFacingSceneSelector(void)
{
    struct SceneActor_02001334 *record = GetSceneRecord(0);
    u16 angle;

    OpenSceneSection();
    SetSceneActorMode(0, 8);
    AdvanceSceneStep(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    SetSceneActorModeAfterBranch(0, 1);
    RunSceneVariant(1);
    CloseSceneSection();
}
