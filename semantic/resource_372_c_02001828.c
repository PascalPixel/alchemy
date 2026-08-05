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
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a1f0();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1e8();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a110();
void Func_0808a090();
void Func_0808a0b0();
void Func_0808a098();
void Func_0808a150();
void Func_0808a010();
s32 *Func_0808a080();
void Func_0808a138();
void Func_0808a190();
void Func_0808a188();
void Func_0808a100();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a048();
void Func_080770c8();
void Func_0808a020();

/* In-image data blocks (pool words 0x0200c934 and 0x0200c984). */
extern u8 Data_0200c934[];
extern u8 Data_0200c984[];

void Func_02001828(void)
{
    s32 msg = 0xe74;      /* r5 */
    s32 one = 0x10000;    /* r6, 128 << 9 */
    s32 *obj;
    s16 *pos;

    if (Func_080770c0(0x837) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a1f0(22, 0x100);                     /* 128 << 1 */
    Func_0808a170(msg);
    Func_0808a180(22, 0);
    Func_0808a1e8(0, 0x100, 20);
    Func_0808a1b8(0, 0x4000, 0);                  /* 128 << 7 */
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(0x1000000, -1, 0x24c0000, 1);   /* 128<<17, 147<<18 */
    Func_0808a090(22, 0x20000, 0x10000);          /* 128<<10, 128<<9 */
    Func_0808a0b0(22, Data_0200c934);
    Func_0808a150(0, 22, 0);
    Func_0808a010(30);
    Func_0808a098(22, Data_0200c984);
    Func_0808a180(22, 0);

    obj = Func_0808a080(22);
    Func_0808a138(22, 1);
    obj[7] = one;                                 /* +0x1c */

    Func_0808a010(20);
    Func_0808a190(22, 0);
    Func_0808a010(40);
    Func_0808a138(22, 1);
    Func_0808a170(msg);
    msg += 5;
    Func_0808a188(22, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a110(22, 3);                         /* two arguments here */
    Func_0808a180(22, 0);
    Func_0808a090(22, one, 0x8000);               /* 128 << 8 */
    Func_0808a100(22, 2);

    pos = Func_0808a080(0);
    if (pos != 0) {
        /* signed halfword fields at +10 and +18, as in 0x0200173c */
        Func_0808a0b8(22, pos[5], pos[9]);
    }

    Func_0808a0e8(22);
    Func_0808a0f0(22, 0, 0);
    Func_0808a048(1, 1);
    Func_0808a100(21, 3);
    Func_080770c8(0x837);
    Func_0808a020();
}
