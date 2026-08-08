#include "types.h"

/*
 * resource_389 owner at 0x0200121c, 396 bytes: the overlay's ENTRY-0 root.
 *
 * The image's offset 0 is an exported-entry veneer table of the documented
 * `ldr r4,[pc,#0] / bx r4 / .word <target>` shape (HANDOVER: "an overlay's
 * image offset 0 can be an exported-entry veneer table").  Its six words are
 * 0x0200921d, 0x02008b51, 0x02008b5d, 0x02008b65, 0x020090c1 and 0x02008b59;
 * under the proven 0x02008000 link base entry 0 is file offset 0x121c plus the
 * Thumb bit, which is this owner.  So this is the overlay's first root and the
 * call graph was worked down from here.
 *
 * Link base, proved independently of the table: this overlay's byte-exact
 * siblings sit at 0x02000a00, 0x02001088, 0x0200109c and 0x020010b4, and
 * `assets/code/resource_389_overlay.s` holds the odd pool words 0x02008a01,
 * 0x02009089, 0x0200909d and 0x020090b5 - each exactly those offsets plus
 * 0x8000 plus the Thumb bit.  Four witnesses, one base.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200121c through the single
 * interworking epilogue `pop {r5, r6} / pop {r1} / bx r1` at 0x02001388.
 * Because the return address is popped into r1 rather than r0, r0 survives and
 * IS the result (HANDOVER section 0 epilogue rule); `movs r0, #0` immediately
 * before it, so the owner returns the constant 0.
 *
 * Pool map from a control-flow walk, not from a listing: every halfword from
 * 0x0200121c to 0x0200138e is reached as an instruction, and 0x02001390 to
 * 0x020013a7 (six words: 0x03001ebc, 0x00000302, 0x00000301, 0x02000240,
 * 0x020098ec, 0x020090c9) is never reached.  372 code + 24 pool = 396, which
 * is exactly the inventory row's code_bytes and span_bytes.
 *
 * Calls: 23 sites, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_389 121c` (the disassembler's own `bl` annotations are wrong by
 * construction - an overlay `bl` stores target_offset - 2).  Per-target
 * multiset, each site placed below in program order:
 *   Func_0808a080  x5   0x1238 0x12de 0x12ea 0x12f6 0x1348
 *   Func_080770c0  x3   0x1248 0x1288 0x12b0
 *   Func_0808a0f0  x4   0x125a 0x1296 0x12c2 0x133c
 *   Func_080091c0  x4   0x126c 0x1280 0x12aa 0x1314
 *   Func_0808a010  x2   0x1342 0x1360
 *   Func_0808a360  x1   0x132a
 *   Func_0808a370  x1   0x132e
 *   Func_0808a0c0  x1   0x135a
 *   Func_02000ce0  x1   0x1364   (this overlay's own owner, prologue site)
 *   Func_080000d0  x1   0x1382
 * The inventory advertises calls=22; 23 is a beat, which is the safe direction
 * (HANDOVER: the `calls` field is a FLOOR).
 *
 * The two 0x080091c0 arms at 0x0200126c and 0x02001280 differ in exactly one
 * argument (r0 = 24 against r0 = 18) and are kept as two separate call sites
 * rather than folded, per the "do not merge behaviourally near-twin arms" rule.
 *
 * The opening pair of stores is the documented displacement/value trap: r3
 * carries 448 as an offset, then `adds r3,#68` makes it the VALUE 516 stored at
 * workspace+448, then `subs r3,#60` makes it the OFFSET 456 for the next store.
 * Reading it as workspace+456 receiving 516 - 60 is the natural mistake.
 *
 * UNCERTAINTY: the call to this overlay's own Func_02000ce0 at 0x02001364 sets
 * no argument register - the preceding Func_0808a010(60) clobbers r0 - so it is
 * spelled with no argument here.  Whatever Func_02000ce0 reads in r0 is
 * whatever that import returned, which is not established.
 */

/* The shared cross-overlay scene workspace; see resource_370_c_02000384.c. */
extern s16 Data_02000240[];

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

/* In-image word at file offset 0x18ec, cleared before the task is installed. */
extern s32 Data_020098ec;

struct SceneActor {
    u8 unknown_00[12];
    s32 field0c;                    /* 0x0c */
    u8 unknown_10[0x12];
    u8 state22;                     /* 0x22 */
    u8 flags23;                     /* 0x23 */
    u8 unknown_24[0x35];
    u8 flags59;                     /* 0x59 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor *Func_0200268a();
s32 Func_0200266a();
void Func_020026e4();
void Func_0200265e();
void Func_02002672();
s32 Func_020026aa();
void Func_02002720();
void Func_0200269c();
s32 Func_020026d2();
void Func_0200274c();
struct SceneActor *Func_02002730();
struct SceneActor *Func_0200273c();
struct SceneActor *Func_02002748();
void Func_02002706();
void Func_02002824();
void Func_02002830();
void Func_020027c6();
void Func_0200277c();
struct SceneActor *Func_0200279a();
void Func_020027c4();
void Func_0200279a_b();
void Func_02002046();
void Func_02002734();
                                     /* scene actor by selector */
                                     /* read an event flag */
                                     /* place/aim the camera at a point */
                                     /* six-argument scripted-actor call */
                                     /* wait this many frames */
                                     /* scene bookkeeping, no arguments */
                                     /* scene bookkeeping, no arguments */
                                     /* three-argument scripted-actor call */
                                     /* install a task by entry and priority */

                                     /* this overlay, 0x02000ce0 */

s32 Func_0200121c(void)
{
    u8 *workspace = Data_03001ebc;
    struct SceneActor *actor;
    s16 *scene;

    /*
     * Displacement/value pair: 516 is stored AT workspace+448, and 456 is the
     * next store's offset, not 516 - 60 as a value.
     */
    *(s32 *)(workspace + 448) = 516;
    *(s32 *)(workspace + 456) = 24;

    actor = Func_0200268a(9);
    actor->flags59 = (u8)(actor->flags59 | 0x10);

    if (Func_0200266a(0x302) != 0) {
        Func_020026e4(8, 0x01580000, 0x00680000);   /* 172 << 17, 208 << 15 */
        Func_0200265e(24, 40, 6, 3, 18, 6);
    } else {
        Func_02002672(18, 40, 6, 3, 18, 6);
    }

    if (Func_020026aa(768) != 0) {                  /* 192 << 2 */
        Func_02002720(9, 0, 0);
        Func_0200269c(21, 45, 4, 2, 21, 11);
    }

    if (Func_020026d2(0x301) != 0) {
        Func_0200274c(10, 0x02680000, 0x00e80000);  /* 154 << 18, 232 << 16 */

        /*
         * `ldrh` then `subs #2` then `<< 16` compared against 0x80 << 9 is a
         * 16-bit windowed range test: the shift truncates to the low halfword
         * and preserves order, so it is exactly (u16)(value - 2) <= 1, i.e.
         * the scene id is 2 or 3.  Read without the truncation it looks like a
         * signed comparison against 0x10000.
         */
        if ((u16)((u16)Data_02000240[225] - 2) <= 1) {
            Func_02002730(10)->state22 = 2;
            Func_0200273c(10)->field0c -= 1;

            actor = Func_02002748(10);
            actor->flags23 = (u8)(actor->flags23 | 2);

            Func_02002706(36, 48, 5, 1, 36, 14);
        }
    }

    scene = &Data_02000240[225];

    if (*scene == 99) {
        Func_02002824();
        Func_02002830();
        Func_020027c6(9, 0x01800000, 0x00c00000);   /* 192 << 17, 192 << 16 */
        Func_0200277c(60);

        Func_0200279a(9)->state22 = 2;
        Func_020027c4(9, 408, 192);                 /* 204 << 1 */

        Func_0200279a_b(60);
        Func_02002046();
    }

    /* Byte offset 141 << 2 = 564, i.e. halfword index 282. */
    if (Data_02000240[282] != 0) {
        Data_020098ec = 0;
        /* 0x020090c9 = Func_020010c8 + Thumb bit, this overlay's own task. */
        Func_02002734((void *)0x020090c9, 3200);    /* 200 << 4 */
    }

    return 0;
}
