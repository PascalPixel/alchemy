#include "types.h"

/*
 * Resource 372 guarded cutscene script step at 0x02001828 (356 bytes,
 * 33 distinct call targets across 35 call sites).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001828, matching
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x0200196c.  `pop {r0} ; bx r0` — r0 is
 * the popped return address, so the owner returns nothing.
 * 0x02001972-0x0200198b is alignment plus the literal pool (0x837, 0xe74,
 * 0x6666, 0xccc, 0x0200c934, 0x0200c984), reached only by `ldr rN, [pc,#imm]`.
 *
 * The body is the same actor-script family as the byte-exact sibling
 * `assets/code/resource_372_c_0200173c.c` and as
 * `semantic/overlays/resource_372_c_0200198c.c`: an outer one-shot flag guard
 * (0x837, tested at entry and set at exit), then a long straight-line drive of
 * actor 22, ending with the same optional position fix-up read through
 * `*(s16 *)(p + 10)` / `*(s16 *)(p + 18)`.
 *
 * r5 carries a message identifier 0xe74 that is bumped by 5 to 0xe79 before its
 * second use; r6 holds 0x10000, stored into +0x1c of a queried object and later
 * reused as an argument.
 *
 * Note three imports take different argument counts at different sites:
 * 0x020060a8 (four arguments at 0x02001882, two at 0x0200190a), 0x020060a0
 * (two at both sites but with unrelated values) and 0x02005ff6 (two sites,
 * different in-image data blocks).  Old-style declarations are mandatory.
 *
 * LINK BASE / call-target reading.  The two data pool words 0x0200c934 and
 * 0x0200c984 resolve, under this overlay's proven 0x02008000 link base, to file
 * offsets 0x4934 and 0x4984 — inside the same address band that this overlay's
 * `bl` instructions appear to target (e.g. `bl sub_0200491e` in the owner at
 * 0x02000278).  A `bl` cannot land in a proven data block, so the encoded
 * branch targets in this overlay are load-time-fixed-up import identities, not
 * locations.  That is why every import below is named by the address its call
 * site computes and is never resolved.
 *
 * `a prior working note` parks this row for the byte-exact
 * reconstruction over constant re-materialisation (0x837 twice cross-block, 0x100 twice
 * inside one basic block).  That is a codegen obstacle, not a semantic one.
 */

/* Imports, old-style: interfaces unknown, and several take different argument
 * counts at different sites. */
s32 Func_02005f0a();
void Func_02005f34();
void Func_02006056();
void Func_0200601e();
void Func_02006036();
void Func_0200606a();
void Func_02006066();
void Func_0200608e();
void Func_020060a8();
void Func_02005fce();
void Func_02005ff6();
void Func_02006068();
void Func_02005f9e();
void Func_0200609e();
s32 *Func_02005fec();
void Func_02006082();
void Func_02005fc8();
void Func_020060d0();
void Func_02005fd6();
void Func_020060a0();
void Func_020060c6();
void Func_020060e8();
void Func_020060f8();
void Func_0200605c();
void Func_020060bc();
s32 *Func_0200605a();
void Func_020060a4();
void Func_020060ca();
void Func_020060dc();
void Func_0200606c();
void Func_020060f4();
void Func_0200604a();
void Func_0200606e();

                     

                     

                     

                     

                     

                     

                     

                     

/* In-image data blocks (pool words 0x0200c934 and 0x0200c984). */
extern u8 Data_0200c934[];
extern u8 Data_0200c984[];

void Func_02001828(void)
{
    s32 msg = 0xe74;      /* r5 */
    s32 one = 0x10000;    /* r6, 128 << 9 */
    s32 *obj;
    s16 *pos;

    if (Func_02005f0a(0x837) != 0) {
        return;
    }

    Func_02005f34();
    Func_02006056(22, 0x100);                     /* 128 << 1 */
    Func_0200601e(msg);
    Func_02006036(22, 0);
    Func_0200606a(0, 0x100, 20);
    Func_02006066(0, 0x4000, 0);                  /* 128 << 7 */
    Func_0200608e(0x6666, 0xccc);
    Func_020060a8(0x1000000, -1, 0x24c0000, 1);   /* 128<<17, 147<<18 */
    Func_02005fce(22, 0x20000, 0x10000);          /* 128<<10, 128<<9 */
    Func_02005ff6(22, Data_0200c934);
    Func_02006068(0, 22, 0);
    Func_02005f9e(30);
    Func_02005ff6(22, Data_0200c984);
    Func_0200609e(22, 0);

    obj = Func_02005fec(22);
    Func_02006082(22, 1);
    obj[7] = one;                                 /* +0x1c */

    Func_02005fc8(20);
    Func_020060d0(22, 0);
    Func_02005fd6(40);
    Func_020060a0(22, 1);
    Func_020060c6(msg);
    msg += 5;
    Func_020060e8(22, 0, 20);
    Func_020060a0(0, 3);
    Func_020060a8(22, 3);                         /* two arguments here */
    Func_020060f8(22, 0);
    Func_0200605c(22, one, 0x8000);               /* 128 << 8 */
    Func_020060bc(22, 2);

    pos = Func_0200605a(0);
    if (pos != 0) {
        /* signed halfword fields at +10 and +18, as in 0x0200173c */
        Func_020060a4(22, pos[5], pos[9]);
    }

    Func_020060ca(22);
    Func_020060dc(22, 0, 0);
    Func_0200606c(1, 1);
    Func_020060f4(21, 3);
    Func_0200604a(0x837);
    Func_0200606e();
}
