#include "types.h"

/*
 * Resource 39e facing-quadrant selector at 0x02000f80 (156 bytes, 9 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  The executable interval begins at 0x02000f80; the preceding
 * owner's final pool halfword is at 0x02000f7e, which is why the audited
 * interval currently starts two bytes late at 0x02000f82.  Walking from the
 * prologue at 0x02000f80, every branch
 * (0x02000f96, 0x02000fb2, 0x02000fbe, 0x02000fd0, 0x02000fde, 0x02001000)
 * lands inside 0x02000f80-0x02001018, and the walk stops at the interworking
 * return `pop {r0} / bx r0` at 0x02001018-0x0200101a.  So the executable
 * extent is exactly 0x02000f80-0x0200101b (156 bytes).  The popped register is
 * r0, so the popped value is the return address and the owner is `void`.
 *
 * Pool map — the walk never reaches these as instructions, and three of the
 * four disassemble as plausible code (`b.n`, `subs`, `adr`), which is the
 * "a pool word decodes as an instruction" trap:
 *   0x0200101c  0xffffe000   -0x2000
 *   0x02001020  0x3fff0000   the bound, pre-shifted
 *   0x02001024  0x00003fff   the same bound, unshifted
 *   0x02001028  0xffffa000   -0x6000
 * None is an address: all four are below the 0x02008000 link base and all four
 * are used as arithmetic operands.  The next prologue begins at 0x0200102c.
 *
 * All nine call sites were resolved with the overlay call-target listing, and
 * the per-target histogram is the scene-record fetch x1, scene-position x6,
 * and scene-heading x2:
 *   0x02000f84 -> scene-record fetch   0x02000fe6 -> scene-position
 *   0x02000f9e -> scene-position       0x02000ff0 -> scene-position
 *   0x02000fa8 -> scene-position       0x02000ffc -> scene-heading
 *   0x02000fc6 -> scene-position       0x02001008 -> scene-position
 *                                   0x02001014 -> scene-heading
 *
 * THREE OF THE FOUR ARMS SHARE ONE scene-heading SITE (0x02000ffc) and the
 * fourth has its own (0x02001014).  Writing the close into each arm would put
 * four scene-heading calls in the C against two in the assembly — the
 * bracket-close overcount the reconstruction's convention warns about — so
 * the three source calls are retained as one compiler-merged tail at the
 * witnessed site.
 *
 * The selector is the standard facing quantisation: the u16 at record+6 is
 * biased by -0x2000, -0x6000 and +0x6000 in turn and compared unsigned against
 * 0x3fff, so the four arms are the four 90-degree quadrants of the circle in
 * the order [0x2000,0x6000), [0x6000,0xa000), [0xa000,0xe000), rest.  The
 * first comparison is done on the value shifted left by 16 against the
 * pre-shifted bound 0x3fff0000 and the later ones on the value shifted back
 * down against 0x3fff — same test, two spellings, and reading either as a
 * signed magnitude test loses the wrap.
 *
 * The value handed to the shared tail (0x2000, 0x5000, 0xe000, and 0x2000 for
 * the fourth arm) is built with `movs`+`lsls` as a VALUE, not a displacement.
 */

/*
 * Overlay-local relocation names witnessed at the nine call sites.  The
 * aliases keep the source readable without replacing those machine-producing
 * symbols with guessed game-specific names.
 */
extern u8 *Func_0200538a();
extern void Func_020053d4();
extern void Func_020053de();
extern void Func_020053fc();
extern void Func_0200541c();
extern void Func_02005426();
extern void Func_0200543e();
extern void Func_020054c2();
extern void Func_020054da();

#define SceneRecordFetch Func_0200538a
#define ScenePosition_216_168 Func_020053d4
#define ScenePosition_224_168 Func_020053de
#define ScenePosition_232_160 Func_020053fc
#define ScenePosition_216_168_B Func_0200541c
#define ScenePosition_224_172 Func_02005426
#define ScenePosition_232_160_B Func_0200543e
#define SceneHeadingShared Func_020054c2
#define SceneHeadingFourth Func_020054da
#define SetupRoofActorsByFacing Func_02000f80

void SetupRoofActorsByFacing(void)
{
    u16 facing = *(u16 *)(SceneRecordFetch(0) + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        ScenePosition_216_168(15, 216, 168);
        ScenePosition_224_168(15, 224, 168);
        SceneHeadingShared(15, 128 << 6, 20);
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        ScenePosition_232_160(15, 232, 160);
        SceneHeadingShared(15, 160 << 7, 20);
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        ScenePosition_216_168_B(15, 216, 168);
        ScenePosition_224_172(15, 224, 172);
        SceneHeadingShared(15, 224 << 8, 20);
    } else {
        ScenePosition_232_160_B(15, 232, 160);
        SceneHeadingFourth(15, 128 << 6, 20);
    }
}
