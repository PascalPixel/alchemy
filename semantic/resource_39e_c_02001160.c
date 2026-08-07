#include "types.h"

/*
 * resource_39e owner at 0x02001160, 384 bytes: a fully linear scripted
 * scene beat, roughly thirty calls, no branches -- the familiar
 * Func_0808a0xx/a1xx vocabulary plus one call into this overlay's own
 * already-drafted resource_39e_c_02000f80.c and a closing story-flag
 * set (Func_080770c8(0x301), the test/set pair idiom). Notable
 * mid-beat: dialogue 0x187a (Func_0808a170), the a090 call reusing the
 * 0xcccc/0x6666 fixed-point pair from the bf4/cd4 spawn bursts, two
 * Func_0808a0f0(id, 232<<16, 168<<16) calls for ids 19/20, four
 * consecutive slot-19 record fetches (Func_0808a080) each used for one
 * field write (+12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, and the
 * BAM angle u16 at +30 of the sub-object at +80 = 0x8000), sound 124,
 * then Func_0808a0d0(15, 216, 152) / Func_0808a1b8(15, 0x4000, 30).
 * Called 2x within this overlay.
 *
 * Complete owner: `push {lr}` at 0x02001160 through `pop {r0} / bx r0`
 * at 0x020012c4-0x020012c6, six pool words ending 0x020012df; the next
 * owner prologue (`push {lr}`) begins at 0x020012e0 -- another
 * boundary-gap function not in the unindexed population, left for its
 * own read.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_39e 1160 12c6's
 * +2 rule).
 *
 * Uncertainty: callees and argument roles identified by shape only;
 * the four record re-fetches are transcribed as written (the assembly
 * really calls Func_0808a080(19) four times rather than caching).
 */

extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
extern void Func_02000f80(void);

void Func_02001160(void)
{
    u8 *record;

    Func_0808a148(13, 19, 0);
    Func_0808a148(14, 19, 0);
    Func_0808a148(15, 19, 0);
    Func_0808a148(16, 19, 0);
    Func_0808a148(18, 19, 0);
    Func_0808a010(20);
    Func_0808a138(15, 2);
    Func_0808a010(20);
    Func_0808a170(0x187a);
    Func_0808a188(15, 0, 20);
    Func_0808a188(16, 0, 20);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a1e8(16, 0x101, 60);
    Func_0808a188(16, 0, 20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a1e8(16, 0x102, 60);
    Func_0808a188(16, 0, 20);
    Func_0808a150(15, 18, 0);
    Func_0808a010(20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a110(15, 3);
    Func_0808a010(20);
    *(s32 *)(record + 24) = 0xcccc;
    Func_0808a090(15, 0xcccc, 0x6666);
    Func_02000f80();
    Func_0808a138(15, 3);
    Func_0808a0f0(19, 232 << 16, 168 << 16);
    Func_0808a0f0(20, 232 << 16, 168 << 16);
    record = Func_0808a080(19);
    *(s32 *)(record + 12) = 192 << 12;
    record = Func_0808a080(19);
    record = Func_0808a080(19);
    record = Func_0808a080(19);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 128 << 8;
    *(u32 *)(record + 60) = 0x80000000;
    Func_080f9010(124);
    Func_0808a010(40);
    Func_0808a0d0(15, 216, 152);
    Func_0808a1b8(15, 128 << 7, 30);
    Func_080770c8(0x301);
}
