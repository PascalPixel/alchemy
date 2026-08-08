#include "types.h"

/*
 * resource_38e owner at 0x020001e4, 632 bytes: a scene-script callback with
 * three outcomes - a short "already seen it" line, a shorter refusal line, or
 * the full two-speaker beat that ends by setting event flag 0x84a.
 *
 * Named before it was opened: the odd in-image pool word 0x020081e5 appears in
 * `assets/code/resource_38e_overlay.s`, and under the proven 0x02008000 link
 * base that is file offset 0x01e4 plus the Thumb bit.
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_38e` finds no `bl` inside the
 * overlay targeting 0x01e4, so, like 0x0200045c, the scene-script table is the
 * only way in.
 *
 * Complete owner: `push {lr}` at 0x020001e4 through the single epilogue at
 * 0x02000420, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x020001e4-0x02000423 is reached as
 * instructions, 0x02000424-0x0200045b is not - fourteen words.  576 code +
 * 56 pool = 632, exactly the inventory row's code_bytes and span_bytes.
 * `overlay_show` again lists a phantom reference (0x02000788) produced by the
 * pool word 0x00004ccc decoding as `ldr r4,[pc,#816]`; the walk never reaches
 * 0x02000454, so it is data - 0.3 in 16.16, paired with 0x00009999 = 0.6.
 *
 * Calls: 55 sites against an advertised 51 - a beat of four, the safe
 * direction.  Per-target multiset from `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_38e 1e4`, each count reproduced by hand from the listing:
 *   Func_0808a1b8  x8   Func_0808a0d0  x6   Func_0808a188  x6
 *   Func_0808a170  x4
 *   Func_080770c0  x3   Func_0808a080  x3   Func_0808a090  x3
 *   Func_0808a130  x3   Func_0808a180  x3   Func_0808a1e8  x3
 *   Func_080770c8  x2
 *   Func_0808a010  x1   Func_0808a018  x1   Func_0808a020  x1
 *   Func_0808a070  x1   Func_0808a0f0  x1   Func_0808a100  x1
 *   Func_0808a110  x1   Func_0808a138  x1   Func_0808a168  x1
 *   Func_0808a178  x1
 *   Func_020008e8  x1  (this overlay, prologue site, byte-exact C exists)
 *
 * TWO shared tails, both spelled with a label and a `goto` rather than copied
 * per arm.  0x0200041c (Func_0808a020) is reached from three places and
 * 0x020003fa (Func_0808a168) from two.  Duplicating them would have inflated
 * the multiset by two and one respectively; the handover's rule is that a
 * shared tail is evidence for a label, not for duplication.
 *
 * The increment at 0x02000278 is the documented SKIP-BEAT COUNTER in its
 * canonical spelling - `movs r3, #236 / lsls r3, r3, #1` off the 0x03001ebc
 * workspace pointer, bumping the u16 at workspace+472.  It sits on the path
 * that plays the full beat, so it counts how many times this scene has been
 * played out rather than skipped.
 *
 * `cmp r3, r2 / bgt` against the pooled 0x010dffff is a SIGNED comparison on
 * entity 0's z, and the block it guards runs when z is at or below that bound.
 * Reading 0x010dffff as a mask rather than a coordinate bound is the easy
 * mistake: it is 269.99998 in 16.16, i.e. "just under 270".
 *
 * Text ids 0x140d, 0x1413, 0x1414 and 0x1416 are one contiguous family, as are
 * this overlay's 0x1720/0x1724/0x1726 in 0x020005dc.  0x02008bf4 is even, so
 * under the link base it is in-image DATA at file offset 0x0bf4 - the same
 * table 0x0200050c hands to the same import.
 */

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

struct SceneEntity {
    u8 unknown_00[16];
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x46];
    u8 flags5a;                     /* 0x5a */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_02000cbc();
s32 Func_02000ca2();
s32 Func_02000cae();
s32 Func_02000cb8();
void Func_02000d62();
void Func_02000d84();
void Func_02000da6();
void Func_02000d98();
void Func_02000d78();
void Func_02000cf6();
void Func_02000d94();
void Func_02000dac();
void Func_02000da4();
void Func_02000dbc();
void Func_02000db4();
void Func_02000dc4();
s32 Func_02000d5c();
void Func_02000dfc();
struct SceneEntity *Func_02000d8a();
void Func_02000dc2();
void Func_02000dd0();
void Func_02000e4c();
void Func_02000e58();
void Func_02000e64();
void Func_02000e78();
void Func_02000e38();
void Func_02000e72();
void Func_02000e96();
void Func_02000e56();
void Func_02000e90();
void Func_02000e68();
void Func_02000ebc();
void Func_02000ec8();
void Func_02000eca();
void Func_02000ee6();
void Func_02000e74();
struct SceneEntity *Func_02000e72_b();
void Func_02000ea0();
void Func_02000e66();
struct SceneEntity *Func_02000e94();
void Func_02000eb0();
void Func_02000ecc();
void Func_02000ed8();
void Func_02000f54();
void Func_02000cc4(void);
void Func_02000ef4();
void Func_02000f16();
void Func_02000eb4();
void Func_02000f50();
void Func_02000f72();
void Func_02000f96();
void Func_02000efa();
                                      /* scene entity by selector */
                                      /* read an event flag */
                                      /* set an event flag */
                                      /* text/beat still running? */
                                      /* wait this many frames */
                                      /* scene bracket open */
                                      /* scene bracket close */
                                      /* set an x/y scale pair, 16.16 */
                                      /* move an entity to a waypoint */
                                      /* place/aim the camera at a point */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* three-argument presentation call */
                                      /* show a text entry by id */
                                      /* advance the text entry */
                                      /* close the text entry */
                                      /* three-argument presentation call */
                                      /* three-argument presentation call */
                                      /* three-argument presentation call */

                                      /* this overlay, byte-exact source */

void Func_020001e4(void)
{
    u8 *workspace;
    struct SceneEntity *speaker;

    Func_02000cbc();

    if (Func_02000ca2(0x84a) != 0) {
        /* Already played out once. */
        if (Func_02000cae(772) != 0) {          /* 193 << 2 */
            if (Func_02000cb8(0x201) == 0) {
                Func_02000d62(0x1414);
                Func_02000d84(12, 0, 10);
                Func_02000da6(12, 263, 40);
                Func_02000d98(12, 0, 10);
                Func_02000d78(12, 2);
                Func_02000cf6(0x201);
            }
            Func_02000d94(0x1416);
            Func_02000dac(12, 0);
            goto close;
        }

        Func_02000da4(0x1413);
        Func_02000dbc(12, 0);
        goto handoff;
    }

    Func_02000db4(0x140d);
    Func_02000dc4(12, 0);

    if (Func_02000d5c(0, 0) != 0) {
        goto interrupted;
    }

    /* Skip-beat counter: the u16 at workspace+472. */
    *(u16 *)(workspace + 472) += 1;             /* 236 << 1 */
    workspace = Data_03001ebc;

    Func_02000dfc(12, 0, 10);

    /* 0x010dffff is 269.99998 in 16.16, a coordinate bound, not a mask. */
    if (Func_02000d8a(0)->z <= 0x010dffff) {
        Func_02000da4(12, 0xcccc, 0x6666);      /* 0.8, 0.4 */
        Func_02000dc2(0, 346, 274);             /* 173 << 1, 137 << 1 */
        Func_02000dd0(0, 328, 282);             /* 164 << 1, 141 << 1 */
        Func_02000e4c(0, 0xc000, 0);            /* 192 << 8 */
    }

    Func_02000e58(11, 0x1000, 0);               /* 128 << 5 */
    Func_02000e64(12, 0x7000, 20);              /* 224 << 7 */
    Func_02000e78(11, 258, 20);                 /* 129 << 1 */
    Func_02000e38(11, 1);
    Func_02000e72(11, 0, 10);
    Func_02000e96(12, 264, 60);                 /* 132 << 1 */
    Func_02000e56(12, 1);
    Func_02000e90(12, 0, 20);
    Func_02000e58(11, 3);
    Func_02000e68(12, 3);
    Func_02000ebc(11, 0x3000, 0);               /* 192 << 6 */
    Func_02000ec8(12, 0x5000, 10);              /* 160 << 7 */
    Func_02000e90(11, 1);
    Func_02000eca(11, 0, 20);
    Func_02000ee6(11, 0xf000, 0);               /* 240 << 8 */

    Func_02000e74(12, 0x00010000, 0x00008000);  /* 1.0, 0.5 */

    speaker = Func_02000e72_b(12);
    speaker->flags5a = (u8)(speaker->flags5a & ~1);

    Func_02000ea0(12, 346, 263);
    Func_02000e66(1);

    speaker->flags5a = (u8)(speaker->flags5a | 1);
    speaker = Func_02000e94(12);

    Func_02000eb0(11, 0x9999, 0x4ccc);          /* 0.6, 0.3 */
    Func_02000ecc(11, 328, 263);
    Func_02000ed8(11, 328, 252);
    Func_02000f54(11, 0xc000, 10);

    Func_02000cc4();

    Func_02000ef4(11, 328, 246);
    Func_02000f16(11, 0, 0);
    Func_02000eb4(0x84a);

handoff:
    /* 0x02008bf4 = in-image data at file offset 0x0bf4. */
    Func_02000f50(12, 0x00010000, (void *)0x02008bf4);
    goto close;

interrupted:
    /* The player broke off before the beat completed. */
    Func_02000f72(12, 0);
    Func_02000f96(12, 0x3000, 10);              /* 192 << 6 */

close:
    Func_02000efa();
}
