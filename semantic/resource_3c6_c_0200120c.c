#include "types.h"

/*
 * resource_3c6 scene setup at 0x0200120c, 716 bytes (0x0200120c-0x020014d7).
 *
 * Complete owner: the `push {r5, r6, r7, lr}` plus r8/r9/sl save at
 * 0x0200120c and the 8-byte outgoing-argument frame (`sub sp, #8`), through
 * the single epilogue at 0x020014c0-0x020014d0.  That epilogue is
 * `pop {r1} / bx r1`, i.e. N != 0, so r0 survives and IS the result; r0 is
 * set to 0 at 0x020014c0 on every path, so the owner returns the constant 0.
 * Nothing reads r0-r3 before they are first written, so it takes no
 * arguments.
 *
 * Three literal pools sit inside the span and are branched over rather than
 * fallen into; the pool map was derived by walking the owner's control flow
 * from the prologue, not from a referenced-words listing:
 *   0x020013a2-0x020013b7  (alignment nop + 0x02000240, 0x09a7, 0x09bf,
 *                           0x03001ebc, 0x09bb) - hopped by the `b.n` at
 *                           0x020013a0 and by the `beq` at 0x02001336;
 *   0x02001422-0x02001427  (alignment nop + 0x00000000) - hopped by the lone
 *                           unconditional `b.n` at 0x02001420, which splits a
 *                           single Func_080091c0 call: r0/r1 and sp[0] are set
 *                           before the pool and r2/r3/sp[4] after it, so the
 *                           reassembled call is one six-argument site;
 *   0x020014d2-0x020014d7  (alignment nop + 0x09b8) - past the `bx`.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * None of the pool words here lies in 0x0200_8xxx-0x0200_bxxx: 0x02000240 is
 * the RAM global array the rest of the project already uses, 0x03001ebc is
 * the overlay's IWRAM workspace pointer cell (same cell as the byte-exact
 * resource_3c6_c_020001f0.c), and the rest are small integer ids.
 *
 * Call accounting: 45 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 120c` - 44 import veneers
 * over 12 distinct imports plus one intra-overlay prologue call to
 * Func_02000218; no `call_via` site.  The multiset of `Func_...(` calls below
 * equals that multiset: Func_0808a080 x16, Func_080091c0 x8, Func_080091e0
 * x5, Func_080770c8 x4, Func_0808a100 x3, Func_080770c0 x3, and one each of
 * Func_08000140, Func_08000150, Func_080001c8, Func_08015250, Func_0808a0f0,
 * Func_02000218 - 45 in total.  (The inventory's `calls=45` agrees here.)
 *
 * UNCERTAINTY 1: the two byte fields cleared at record + 0x59 and set/cleared
 * at record + 0x5b are visibly a pair of per-record enable flags, but only
 * their values are established here, not their meaning.
 * UNCERTAINTY 2: Func_02000218 is called with r0 = 1 still live from the
 * `mov sl` constant used for the two preceding stores.  Func_02000218's own
 * first instruction is a `bl` with r0 untouched, so the value is forwarded
 * rather than dead; it is written as an argument below.
 * UNCERTAINTY 3: the last guard reads the same halfword twice, once sign
 * extended for the == 90 test and once unsigned.  The final test
 * `(u16)(v - 20) << 16 > 0x10000` admits exactly v == 20 and v == 21; it is
 * spelled as that range test below.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay.
 * Func_080770c0 is used in conditions, so it needs a real return type. */
s32 Func_08000140();            /* allocate/acquire a runtime handle */
void Func_08000150();           /* release the handle */
void Func_080001c8();           /* fixed transfer into the acquired block */
void Func_080091c0();           /* six-argument scene presentation request */
void Func_080091e0();           /* per-record presentation request */
void Func_08015250();           /* runtime helper by id */
s32 Func_080770c0();            /* query an audio/sequence cue */
void Func_080770c8();           /* start an audio/sequence cue */
u8 *Func_0808a080();            /* scene record by slot selector */
void Func_0808a0f0();           /* place an entity at (x, z) */
void Func_0808a100();           /* per-record scene request */

/* Intra-overlay call, resolved to the prologue at file offset 0x0218. */
void Func_02000218();

extern s16 Data_02000240[];     /* RAM global array, below the overlay image */
extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

s32 Func_0200120c(void)
{
    s16 *progress = (s16 *)((u8 *)Data_02000240 + 450);
    u8 *workspace;
    u8 *record;
    u8 *sub;
    s32 handle;
    s32 flag;

    if (*progress == 90) {
        Func_080770c8(0x9a7);
        Func_080770c8(0x9bf);
    }

    /* The halfword is compared unsigned here: `lsls #16` on both sides. */
    if (*(u16 *)progress == 91) {
        Func_080770c8(0x9a7);
    }

    workspace = Data_03001ebc;
    /* r3 carries 448 as a displacement and then 256 as the stored value. */
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 24;

    Func_0808a100(19, 3);
    Func_0808a080(19)[0x59] = 0;
    Func_080091e0(Func_0808a080(19), 0);

    Func_0808a100(20, 3);
    Func_0808a080(20)[0x59] = 0;
    Func_080091e0(Func_0808a080(20), 0);

    Func_0808a100(21, 3);
    Func_0808a080(21)[0x59] = 0;
    Func_080091e0(Func_0808a080(21), 0);

    Func_080091e0(Func_0808a080(25), 0);

    record = Func_0808a080(25);
    record[0x5c] = 1;
    record[0x55] = 0;
    sub = *(u8 **)(record + 0x50);
    *(s32 *)(record + 0x0c) = 0x000a0000;
    sub[0x27] = 0;
    sub[5] &= (u8)~0x20;
    sub[9] &= 0x0f;

    handle = Func_08000140(17, 1544);
    Func_08015250(242);
    Func_080001c8(sub[28], 128, handle + 1024);
    Func_08000150(17);

    flag = Func_080770c0(0x9a7);
    if (flag != 0) {
        Func_080091e0(Func_0808a080(24), 0);
        Func_0808a080(24)[0x59] = 0;

        Func_080091c0(20, 23, 1, 1, 14, 4);
        Func_080091c0(20, 23, 1, 1, 15, 4);
        Func_080091c0(20, 23, 1, 1, 16, 4);

        if (Func_080770c0(0x9bb) != 0) {
            Func_0808a0f0(18, 0x00380000, 0x00b80000);
        }
        return 0;
    }

    record = Func_0808a080(8);
    record[0x59] = 0;
    record[0x23] |= 2;
    sub = *(u8 **)(record + 0x50);
    sub[0x26] = 0;
    *(u16 *)(*(u8 **)(record + 0x50) + 30) = 0xc000;

    record = Func_0808a080(9);
    record[0x59] = 0;
    record[0x23] |= 2;
    sub = *(u8 **)(record + 0x50);
    sub[0x26] = 0;
    *(u16 *)(*(u8 **)(record + 0x50) + 30) = 0x4000;

    Func_080091c0(20, 23, 1, 1, 13, 23);
    Func_080091c0(20, 23, 1, 1, 14, 23);
    Func_080091c0(20, 23, 1, 1, 78, 23);
    Func_080091c0(20, 23, 1, 1, 17, 23);
    Func_080091c0(20, 23, 1, 1, 18, 23);

    if ((u16)(*(u16 *)progress - 20) > 1) {
        return 0;
    }
    if (Func_080770c0(0x9b8) != 0) {
        return 0;
    }

    Func_080770c8(0x9b8);
    Func_0808a080(11)[0x5b] = 1;
    Func_0808a080(17)[0x5b] = 1;
    Func_02000218(1);
    Func_0808a080(11)[0x5b] = 0;
    Func_0808a080(17)[0x5b] = 0;
    return 0;
}
