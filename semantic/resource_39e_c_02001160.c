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
 * by `bl` (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 1160 12c6's
 * +2 rule).
 *
 * Uncertainty: callees and argument roles identified by shape only;
 * the four record re-fetches are transcribed as written (the assembly
 * really calls Func_0808a080(19) four times rather than caching).
 */

/* The overlay's import veneers are distinct from the logical ROM callees.
 * These addresses are paired with the annotated call stream at this owner;
 * suffixes preserve separate C return types when one veneer is reused. */
void Func_0200556c(s32);
void Func_0200557a(s32);
void Func_020055c0(s32);
void Func_020055e6(s32, s32, s32);
void Func_020055f0(s32, s32, s32);
void Func_020055fa(s32, s32, s32);
void Func_020055fe(s32, s32);
void Func_02005604(s32, s32, s32);
void Func_0200560c(s32, s32);
void Func_0200560e(s32, s32, s32);
void Func_02005614(s32, s32);
void Func_02005642(s32, s32);
void Func_02005648(s32);
void Func_02005656(s32);
void Func_0200566a(s32, s32, s32);
void Func_02005674(s32, s32, s32);
void Func_0200567a(s32);
void Func_02005680(s32);
u8 *Func_02005680_b(s32);
void Func_02005686(s32);
u8 *Func_0200568c(s32);
void Func_0200568e(s32);
u8 *Func_02005696_b(s32);
void Func_02005692(s32, s32, s32);
void Func_02005696(s32, s32, s32);
void Func_0200569a(s32, s32, s32);
void Func_020056a0(s32, s32, s32);
void Func_020056a8(s32, s32);
void Func_020056aa(s32, s32, s32);
void Func_020056b6(s32, s32, s32);
void Func_020056c0(s32, s32, s32);
void Func_020056c2(s32, s32, s32);
void Func_020056ce(s32, s32, s32);
void Func_020056e4(s32);
void Func_02005780(s32, s32, s32);
u8 *Func_0200581c(s32, s32, s32);
u8 *Func_0200581c_b(s32);
void Func_020021c6(void);

void Func_02001160(void)
{
    u8 *record;

    Func_020055e6(13, 19, 0);
    Func_020055f0(14, 19, 0);
    Func_020055fa(15, 19, 0);
    Func_02005604(16, 19, 0);
    Func_0200560e(18, 19, 0);
    Func_0200556c(20);
    Func_02005614(15, 2);
    Func_0200557a(20);
    Func_02005648(0x187a);
    Func_0200566a(15, 0, 20);
    Func_02005674(16, 0, 20);
    Func_02005696(18, 0x105, 60);
    Func_020056a0(16, 0x101, 60);
    Func_02005692(16, 0, 20);
    Func_02005642(18, 4);
    Func_020055c0(20);
    Func_020056aa(18, 0, 20);
    Func_020056ce(16, 0x102, 60);
    Func_020056c0(16, 0, 20);
    Func_0200569a(15, 18, 0);
    Func_02005680(20);
    Func_020055fe(18, 3);
    Func_0200568e(20);
    Func_0200560c(15, 3);
    Func_02005656(20);
    Func_020056c2(15, 0xcccc, 0x6666);
    Func_020021c6();
    Func_020056a8(15, 3);
    Func_020056b6(19, 232 << 16, 168 << 16);
    Func_02005674(20, 232 << 16, 168 << 16);
    record = Func_02005680_b(19);
    *(s32 *)(record + 12) = 192 << 12;
    record = Func_0200568c(19);
    *(u32 *)(record + 60) = 0x80000000;
    record = Func_02005696_b(19);
    *(s32 *)(record + 24) = 0xcccc;
    record = Func_0200581c_b(19);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 128 << 8;
    Func_0200567a(124);
    Func_020056e4(40);
    Func_02005780(15, 216, 152);
    Func_0200581c(15, 128 << 7, 30);
    Func_02005686(0x301);
}
