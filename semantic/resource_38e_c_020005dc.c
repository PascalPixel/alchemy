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
 * Per-target multiset from `bun tools/overlay_call_targets.ts resource_38e
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
struct SceneEntity *Func_0808a080();  /* scene entity by selector */
s32 Func_0808a070();                  /* text/beat still running? */
void Func_0808a010();                 /* wait this many frames */
void Func_0808a018();                 /* scene bracket open */
void Func_0808a020();                 /* scene bracket close */
void Func_0808a090();                 /* set an x/y scale pair, 16.16 */
void Func_0808a0c8();                 /* move an entity, variant */
void Func_0808a0d0();                 /* move an entity to a waypoint */
void Func_0808a0e8();                 /* wait for the current motion */
void Func_0808a0f0();                 /* place/aim the camera at a point */
void Func_0808a100();                 /* one-argument presentation call */
void Func_0808a110();                 /* one-argument presentation call */
void Func_0808a130();                 /* one-argument presentation call */
void Func_0808a138();                 /* one-argument presentation call */
void Func_0808a170();                 /* show a text entry by id */
void Func_0808a178();                 /* advance the text entry */
void Func_0808a188();                 /* three-argument presentation call */
void Func_0808a1b8();                 /* three-argument presentation call */
void Func_0808a1e8();                 /* three-argument presentation call */
void Func_0808a1f0();                 /* two-argument presentation call */
void Func_0808a208();                 /* two-argument fade parameter pair */
void Func_0808a210();                 /* four-argument fade/tint step */
void Func_0808a248();                 /* one-argument scene call */
void Func_0808a360();                 /* scene bookkeeping, no arguments */
void Func_0808a368();                 /* scene bookkeeping, no arguments */
void Func_0808a370();                 /* scene bookkeeping, no arguments */
s32 Func_080770c0();                  /* read an event flag */
void Func_080000c0();                 /* one-argument runtime call */
void Func_08009128();                 /* no-argument runtime call */

void Func_020008e8(void);             /* this overlay, byte-exact source */

void Func_020005dc(void)
{
    u8 *workspace;
    struct SceneEntity *speaker;

    Func_0808a018();

    Func_0808a210(-1, -1, -1, 0);
    Func_0808a210(0x01400000, -1, 0x01400000, 0);       /* 160 << 17 */
    Func_08009128();
    Func_080000c0(1);
    Func_0808a0f0(0, 0x01400000, 0x01740000);           /* 186 << 17 */
    Func_0808a360();
    Func_0808a208(0x3333, 0x666);                       /* 0.2, 0.025 */
    Func_0808a210(0x01400000, -1, 0x01220000, 1);       /* 145 << 17 */
    Func_0808a090(0, 0x9999, 0x4ccc);                   /* 0.6, 0.3 */
    Func_0808a0d0(0, 320, 310);                         /* 160 << 1, 155 << 1 */

    Func_0808a1b8(11, 0x3000, 10);                      /* 192 << 6 */
    Func_0808a130(11, 2);
    Func_0808a1e8(11, 256, 60);                         /* 128 << 1 */
    Func_0808a170(0x1720);
    Func_0808a188(11, 0, 10);

    Func_0808a1b8(12, 0x5000, 10);                      /* 160 << 7 */
    Func_0808a130(12, 2);
    Func_0808a1e8(12, 256, 60);
    Func_0808a188(12, 0, 20);

    Func_0808a1b8(11, 0x1000, 0);                       /* 128 << 5 */
    Func_0808a1b8(12, 0x7000, 40);                      /* 224 << 7 */
    Func_0808a1b8(11, 0x3000, 0);
    Func_0808a1b8(12, 0x5000, 10);

    Func_0808a138(11, 1);
    Func_0808a188(11, 0, 10);
    Func_0808a138(12, 1);
    Func_0808a178(12, 0);
    Func_0808a1b8(0, 0xe000, 0);                        /* 224 << 8 */

    /* Entered at the TEST; the body can run zero times. */
    do {
        Func_0808a1e8(12, 256, 60);
        Func_0808a170(0x1724);
        Func_0808a188(12, 0, 10);
        Func_0808a130(12, 2);
        Func_0808a178(12, 0);
    } while (Func_0808a070(0, 0) != 0);

    Func_0808a010(10);
    Func_0808a1b8(11, 0x3000, 0);
    Func_0808a1b8(12, 0x5000, 20);
    Func_0808a100(11, 3);
    Func_0808a110(12, 3);
    Func_0808a010(20);

    Func_0808a138(11, 1);
    Func_0808a170(0x1726);
    Func_0808a188(11, 0, 10);
    Func_0808a090(11, 0x00010000, 0x00008000);          /* 1.0, 0.5 */
    Func_0808a0d0(11, 314, 280);                        /* 157 << 1, 140 << 1 */
    Func_0808a1b8(0, 0xc000, 20);                       /* 192 << 8 */
    Func_0808a188(11, 0, 40);
    Func_0808a1f0(0, 258);                              /* 129 << 1 */
    Func_0808a010(60);

    if (Func_080770c0(0x84a) == 0) {
        Func_0808a090(12, 0x00010000, 0x00008000);

        speaker = Func_0808a080(12);
        speaker->flags5a = (u8)(speaker->flags5a & ~1);

        Func_0808a0d0(12, 346, 263);                    /* 173 << 1 */
        Func_0808a010(1);

        speaker = Func_0808a080(12);
        speaker->flags5a = (u8)(speaker->flags5a | 1);
    }

    Func_0808a090(11, 0xcccc, 0x6666);                  /* 0.8, 0.4 */
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0c8(11, 328, 262);                        /* 164 << 1, 131 << 1 */
    Func_0808a0d0(0, 328, 278);                         /* 139 << 1 */
    Func_0808a100(11, 1);

    Func_020008e8();

    Func_0808a010(40);
    Func_0808a0c8(0, 328, 242);
    Func_0808a0d0(11, 328, 242);
    Func_0808a0f0(11, 0, 0);
    Func_0808a0e8(0);
    Func_0808a0f0(0, 0, 0);

    /*
     * Displacement/value pair: 448 is the OFFSET, 513 is what is stored there.
     * The entry-0 root writes 256 to the same cell on the way in.
     */
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(10);
    Func_0808a020();
}
