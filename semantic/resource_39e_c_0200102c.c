#include "types.h"

/*
 * Resource 39e three-variant scripted scene at 0x0200102c (282 bytes,
 * 30 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Its lower bound is confirmed from the other side: the sibling
 * reconstruction's resource_39e_c_02000f80.c ends its literal pool at 0x0200102b and
 * names 0x0200102c as the next prologue.  Walking from that prologue, both
 * forward branches (0x0200104a -> 0x020010a4 and 0x020010a6 -> 0x020010bc)
 * land inside the body and the walk stops at the interworking return
 * `pop {r5,r6} / pop {r0} / bx r0` at 0x02001140-0x02001144.  So the executable
 * extent is exactly 0x0200102c-0x02001145 (282 bytes).  The popped register is
 * r0, so the popped value is the return address and the owner is `void`.
 *
 * Pool map — the halfword at 0x02001146 is `0x0000` alignment and the six
 * words after it are literals the walk never reaches as instructions.  Four of
 * the six objdump renders as plausible `adds`/`lsls`, the pool-word-decodes-as-
 * an-instruction trap:
 *   0x02001148  0x0000cccc   0.8 in 16.16
 *   0x0200114c  0x00006666   0.4 in 16.16
 *   0x02001150  0x0000183a   dialogue id
 *   0x02001154  0x00000101   animation id
 *   0x02001158  0x000018ae   dialogue id
 *   0x0200115c  0x000018ac   dialogue id
 * None is an address: all six are far below the 0x02008000 link base under
 * which this overlay's data resolves, and all six are used as immediate call
 * arguments or as values stored into 16.16 fields.  The next prologue
 * (`push {lr}`) begins at 0x02001160.
 *
 * All 30 call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 102c 1146`, and the
 * per-target histogram is Func_0808a010 x5, Func_0808a080 x4, Func_0808a170
 * x4, Func_0808a188 x4, Func_0808a138 x3, Func_0808a0f0 x2, Func_0808a110 x2,
 * Func_0808a090 x1, Func_0808a0d0 x1, Func_0808a1b8 x1, Func_0808a1e8 x1,
 * Func_080f9010 x1, Func_02000f80 x1:
 *   0x02001036 -> 0x04414 Func_0808a090   0x020010b8 -> 0x043d4 Func_0808a010
 *   0x0200103c -> 0x043d4 Func_0808a010   0x020010c2 -> 0x044b4 Func_0808a188
 *   0x02001044 -> 0x0449c Func_0808a170   0x020010c6 -> 0x00f80 Func_02000f80
 *   0x0200104e -> 0x0449c Func_0808a170   0x020010ce -> 0x04474 Func_0808a138
 *   0x02001058 -> 0x044cc Func_0808a1e8   0x020010dc -> 0x0444c Func_0808a0f0
 *   0x02001062 -> 0x044b4 Func_0808a188   0x020010ea -> 0x0444c Func_0808a0f0
 *   0x0200106a -> 0x04474 Func_0808a138   0x020010f0 -> 0x04404 Func_0808a080
 *   0x02001070 -> 0x0449c Func_0808a170   0x020010fc -> 0x04404 Func_0808a080
 *   0x0200107a -> 0x044b4 Func_0808a188   0x02001108 -> 0x04404 Func_0808a080
 *   0x02001082 -> 0x0445c Func_0808a110   0x02001112 -> 0x04404 Func_0808a080
 *   0x02001088 -> 0x043d4 Func_0808a010   0x02001120 -> 0x0457c Func_080f9010
 *   0x02001092 -> 0x044b4 Func_0808a188   0x02001126 -> 0x043d4 Func_0808a010
 *   0x0200109a -> 0x0445c Func_0808a110   0x02001130 -> 0x04434 Func_0808a0d0
 *   0x020010a0 -> 0x043d4 Func_0808a010   0x0200113c -> 0x044c4 Func_0808a1b8
 *   0x020010aa -> 0x0449c Func_0808a170   0x020010b2 -> 0x04474 Func_0808a138
 * The one intra-overlay call is to Func_02000f80, the facing-quadrant selector
 * the sibling reconstruction converted from this same sweep — so two of the owners this
 * sweep found call each other, which is why neither is reachable from the
 * inventory.
 *
 * The owner takes one argument in r0, kept in r6 across the whole body and
 * tested twice (`cmp #0` and `cmp #2`), so it is a scene variant selector.
 * Variant 0 gets the long middle section; variant 2 gets a short one; variant 1
 * (and anything else) gets neither and falls straight through to the common
 * tail.  Both tests are forward skips over straight-line code, so they are
 * plain `if`s and no arm is duplicated.
 *
 * `subs r0, r5, #1` on the dialogue id is the tell that 0x183a and 0x1839 are
 * consecutive lines of one message pair, held in r5 rather than loaded twice.
 *
 * The tail places both entities 19 and 20 at the same (232.0, 168.0) — the
 * `lsls #16` on each is the 16.16 promotion of an integer coordinate, not a
 * scale — then writes four fields of record 19 through four separate
 * Func_0808a080 fetches: +12 = 12.0, +60 = 0x80000000, +24 = 0.8, and the u16
 * at +30 of the sub-object at +80 = 0x8000, which is the half-turn BAM heading.
 * Four fetches for four stores is what the assembly does; folding them into one
 * cached pointer would drop three sites from the multiset.
 */

void Func_0808a090();          /* 16.16 pair setter */
void Func_0808a010();          /* wait this many frames */
void Func_0808a170();          /* show a dialogue line by id */
void Func_0808a1e8();          /* play animation id for n frames */
void Func_0808a188();          /* wait for the slot's action, with a duration */
void Func_0808a138();          /* select an entity presentation */
void Func_0808a110();          /* scene-presentation request */
void Func_0808a0f0();          /* (selector, x, z) placement */
void Func_0808a0d0();          /* place an entity */
void Func_0808a1b8();          /* face slot along heading */
u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_080f9010();          /* one-argument notify */
void Func_02000f80(void);      /* local: facing-quadrant selector */

void Func_0200102c(s32 variant)
{
    s32 line = 0x183a;

    Func_0808a090(15, 0xcccc, 0x6666);
    Func_0808a010(60);
    Func_0808a170(line);

    if (variant == 0) {
        Func_0808a170(line - 1);
        Func_0808a1e8(15, 0x101, 60);
        Func_0808a188(15, 0, 20);
        Func_0808a138(15, 2);
        Func_0808a170(0x18ae);
        Func_0808a188(15, 0, 20);
        Func_0808a110(15, 4);
        Func_0808a010(20);
        Func_0808a188(15, 0, 20);
        Func_0808a110(15, 3);
        Func_0808a010(20);
    }

    if (variant == 2) {
        Func_0808a170(0x18ac);
        Func_0808a138(15, 2);
        Func_0808a010(20);
    }

    Func_0808a188(15, 0, 20);
    Func_02000f80();
    Func_0808a138(15, 3);

    Func_0808a0f0(19, 232 << 16, 168 << 16);
    Func_0808a0f0(20, 232 << 16, 168 << 16);

    *(s32 *)(Func_0808a080(19) + 12) = 0xc0000;        /* 192 << 12, 12.0 */
    *(s32 *)(Func_0808a080(19) + 60) = 0x80000000;     /* 128 << 24 */
    *(s32 *)(Func_0808a080(19) + 24) = 0xcccc;
    *(u16 *)(*(u8 **)(Func_0808a080(19) + 80) + 30) = 0x8000;   /* 128 << 8 */

    Func_080f9010(124);
    Func_0808a010(40);
    Func_0808a0d0(15, 216, 152);
    Func_0808a1b8(15, 0x2000, 30);                     /* 128 << 6 */
}
