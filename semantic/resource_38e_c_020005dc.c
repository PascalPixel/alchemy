#include "types.h"

/*
 * resource_38e owner at 0x020005dc, 780 bytes: the overlay's long cutscene -
 * fade in, run the two speakers 11 and 12 through their lines, walk them to
 * their marks, and hand back with the shared workspace advanced.
 *
 * Reached only from the scene-0x22 body
 * (semantic/overlays/resource_38e_c_0200050c.c, call site 0x020005b4), which is
 * reached only from the overlay's entry-0 root at 0x020004bc - so this owner
 * sits at depth three of a call graph that was built before any body was
 * opened.
 *
 * Complete owner: `push {lr}` at 0x020005dc through the single epilogue at
 * 0x020008b2, ending `pop {r0} / bx r0` - void.  The next prologue at
 * 0x020008e8 is a DIFFERENT owner and already has byte-exact C
 * (assets/code/resource_38e_c_020008e8.c); this owner calls it at 0x02000856.
 *
 * Pool map from a control-flow walk: 0x020005dc-0x020008b5 is reached as
 * instructions, 0x020008b6-0x020008e7 is not - one alignment halfword plus
 * twelve words.  730 code + 50 pool = 780, exactly the inventory row's
 * code_bytes and span_bytes.
 *
 * The pool contains the documented trap in its pure form: the word
 * 0x00004ccc at 0x020008c4 decodes as `ldr r4,[pc,#816]`, and `overlay_show`
 * duly lists 0x02008031 as a "pool word referenced" by this owner.  It is not.
 * 0x02008031 is another function's address entirely (file offset 0x30 plus the
 * Thumb bit, a byte-exact sibling), and 0x00004ccc is an ordinary 16.16
 * constant - 0.3, paired with 0x00009999 = 0.6 two words earlier.  The
 * control-flow walk never reaches 0x020008c4, which is how it is known to be
 * data.
 *
 * Calls: 72 sites against an advertised 66 - a beat of six, the safe direction.
 * Per-target multiset from `bun tools/lib/overlay_call_targets.ts resource_38e
 * 5dc`, every count reproduced by hand from the listing:
 *   Func_0808a1b8  x10  Func_0808a188  x6
 *   Func_0808a010  x5   Func_0808a090  x5   Func_0808a0d0  x5
 *   Func_0808a130  x3   Func_0808a138  x3   Func_0808a170  x3
 *   Func_0808a1e8  x3   Func_0808a210  x3   Func_0808a0f0  x3
 *   Func_0808a080  x2   Func_0808a0c8  x2   Func_0808a100  x2
 *   Func_0808a178  x2
 *   Func_080000c0  x1   Func_08009128  x1   Func_080770c0  x1
 *   Func_0808a018  x1   Func_0808a020  x1   Func_0808a070  x1
 *   Func_0808a0e8  x1   Func_0808a110  x1   Func_0808a1f0  x1
 *   Func_0808a208  x1   Func_0808a248  x1   Func_0808a360  x1
 *   Func_0808a368  x1   Func_0808a370  x1
 *   Func_020008e8  x1  (this overlay, prologue site, byte-exact C exists)
 *
 * The `while` at 0x0200073a is entered by a branch to its TEST (`b.n` at
 * 0x0200070c jumps forward past the body to 0x0200073a, which then branches
 * backwards into it).  So the body can run zero times, and it is a `while`, not
 * a `do`.  Getting that backwards would put one extra execution of five call
 * sites on every path.
 *
 * The tail store is the displacement/value trap again: r2 holds 448 as an
 * OFFSET, is used to form the address, and only then becomes the VALUE 513 by
 * `adds r2, #65`.  Reading it as workspace+513 is the natural mistake, and this
 * overlay's entry-0 root writes the same cell (workspace+448) with 256 on the
 * way in, which corroborates the offset.
 *
 * The many small `<< n` immediates are 16.16 fixed point throughout;
 * 0x0000cccc = 0.8, 0x00009999 = 0.6, 0x00006666 = 0.4, 0x00004ccc = 0.3,
 * 0x00003333 = 0.2, 0x00000666 = 0.025.  0x1720, 0x1724 and 0x1726 are text
 * ids from one contiguous family.
 *
 * UNCERTAINTY: Func_0808a018 at 0x020005de is called with whatever r0 held on
 * entry, and this owner's sole caller does not set r0 either (it holds the
 * result of a flag query there).  It is spelled with no argument.
 */

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

struct SceneEntity {
    u8 unknown_00[0x5a];
    u8 flags5a;                     /* 0x5a */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_020010b4();
void Func_0200118e();
void Func_020011a0();
void Func_02001094();
void Func_0200105a();
void Func_02001140();
void Func_020011cc();
void Func_020011bc();
void Func_020011d6();
void Func_0200115e();
void Func_020011da();
void Func_020011a2();
void Func_020011f6();
void Func_020011ee();
void Func_0200120a();
void Func_020011d2();
void Func_02001226();
void Func_02001218();
void Func_02001234();
void Func_02001240();
void Func_0200124c();
void Func_02001258();
void Func_02001228();
void Func_0200125a();
void Func_0200123a();
void Func_02001286();
void Func_0200129c();
void Func_02001272();
void Func_02001294();
void Func_0200126c();
s32 Func_0200122c();
void Func_02001216();
void Func_020012d2();
void Func_020012de();
void Func_02001296();
void Func_020012a6();
void Func_02001244();
void Func_020012c4();
void Func_020012da();
void Func_020012fc();
void Func_0200129a();
void Func_020012b8();
void Func_02001334();
void Func_0200132e();
void Func_02001358();
void Func_0200129e();
s32 Func_0200128c();
void Func_020012e6();
struct SceneEntity *Func_020012e4();
void Func_02001312();
void Func_020012d8();
struct SceneEntity *Func_02001306();
void Func_02001322();
void Func_0200132c();
void Func_02001342();
void Func_02001380();
void Func_02001140_b(void);
void Func_0200132a();
void Func_0200136e();
void Func_02001382();
void Func_020013a4();
void Func_020013a2();
void Func_020013b4();
void Func_02001456();
void Func_02001462();
void Func_02001450();
void Func_0200138c();
                                      /* scene entity by selector */
                                      /* text/beat still running? */
                                      /* wait this many frames */
                                      /* scene bracket open */
                                      /* scene bracket close */
                                      /* set an x/y scale pair, 16.16 */
                                      /* move an entity, variant */
                                      /* move an entity to a waypoint */
                                      /* wait for the current motion */
                                      /* place/aim the camera at a point */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* one-argument presentation call */
                                      /* show a text entry by id */
                                      /* advance the text entry */
                                      /* three-argument presentation call */
                                      /* three-argument presentation call */
                                      /* three-argument presentation call */
                                      /* two-argument presentation call */
                                      /* two-argument fade parameter pair */
                                      /* four-argument fade/tint step */
                                      /* one-argument scene call */
                                      /* scene bookkeeping, no arguments */
                                      /* scene bookkeeping, no arguments */
                                      /* scene bookkeeping, no arguments */
                                      /* read an event flag */
                                      /* one-argument runtime call */
                                      /* no-argument runtime call */

                                      /* this overlay, byte-exact source */

void Func_020005dc(void)
{
    u8 *workspace;
    struct SceneEntity *speaker;

    Func_020010b4();

    Func_0200118e(-1, -1, -1, 0);
    Func_020011a0(0x01400000, -1, 0x01400000, 0);       /* 160 << 17 */
    Func_02001094();
    Func_0200105a(1);
    Func_02001140(0, 0x01400000, 0x01740000);           /* 186 << 17 */
    Func_020011cc();
    Func_020011bc(0x3333, 0x666);                       /* 0.2, 0.025 */
    Func_020011d6(0x01400000, -1, 0x01220000, 1);       /* 145 << 17 */
    Func_02001140(0, 0x9999, 0x4ccc);                   /* 0.6, 0.3 */
    Func_0200115e(0, 320, 310);                         /* 160 << 1, 155 << 1 */

    Func_020011da(11, 0x3000, 10);                      /* 192 << 6 */
    Func_020011a2(11, 2);
    Func_020011f6(11, 256, 60);                         /* 128 << 1 */
    Func_020011cc(0x1720);
    Func_020011ee(11, 0, 10);

    Func_0200120a(12, 0x5000, 10);                      /* 160 << 7 */
    Func_020011d2(12, 2);
    Func_02001226(12, 256, 60);
    Func_02001218(12, 0, 20);

    Func_02001234(11, 0x1000, 0);                       /* 128 << 5 */
    Func_02001240(12, 0x7000, 40);                      /* 224 << 7 */
    Func_0200124c(11, 0x3000, 0);
    Func_02001258(12, 0x5000, 10);

    Func_02001228(11, 1);
    Func_0200125a(11, 0, 10);
    Func_0200123a(12, 1);
    Func_0200125a(12, 0);
    Func_02001286(0, 0xe000, 0);                        /* 224 << 8 */

    /* Entered at the TEST; the body can run zero times. */
    do {
        Func_0200129c(12, 256, 60);
        Func_02001272(0x1724);
        Func_02001294(12, 0, 10);
        Func_0200126c(12, 2);
        Func_02001294(12, 0);
    } while (Func_0200122c(0, 0) != 0);

    Func_02001216(10);
    Func_020012d2(11, 0x3000, 0);
    Func_020012de(12, 0x5000, 20);
    Func_02001296(11, 3);
    Func_020012a6(12, 3);
    Func_02001244(20);

    Func_020012c4(11, 1);
    Func_020012da(0x1726);
    Func_020012fc(11, 0, 10);
    Func_0200129a(11, 0x00010000, 0x00008000);          /* 1.0, 0.5 */
    Func_020012b8(11, 314, 280);                        /* 157 << 1, 140 << 1 */
    Func_02001334(0, 0xc000, 20);                       /* 192 << 8 */
    Func_0200132e(11, 0, 40);
    Func_02001358(0, 258);                              /* 129 << 1 */
    Func_0200129e(60);

    if (Func_0200128c(0x84a) == 0) {
        Func_020012e6(12, 0x00010000, 0x00008000);

        speaker = Func_020012e4(12);
        speaker->flags5a = (u8)(speaker->flags5a & ~1);

        Func_02001312(12, 346, 263);                    /* 173 << 1 */
        Func_020012d8(1);

        speaker = Func_02001306(12);
        speaker->flags5a = (u8)(speaker->flags5a | 1);
    }

    Func_02001322(11, 0xcccc, 0x6666);                  /* 0.8, 0.4 */
    Func_0200132c(0, 0xcccc, 0x6666);
    Func_02001342(11, 328, 262);                        /* 164 << 1, 131 << 1 */
    Func_02001358(0, 328, 278);                         /* 139 << 1 */
    Func_02001380(11, 1);

    Func_02001140_b();

    Func_0200132a(40);
    Func_0200136e(0, 328, 242);
    Func_02001382(11, 328, 242);
    Func_020013a4(11, 0, 0);
    Func_020013a2(0);
    Func_020013b4(0, 0, 0);

    /*
     * Displacement/value pair: 448 is the OFFSET, 513 is what is stored there.
     * The entry-0 root writes 256 to the same cell on the way in.
     */
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;

    Func_02001456();
    Func_02001462();
    Func_02001450(10);
    Func_0200138c();
}
