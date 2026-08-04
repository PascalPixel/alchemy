#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x020017c8 (2816 bytes, 293 call sites).
 *
 * Complete owner: prologue `push {r5, r6, r7, lr}` plus the high-register
 * saves (`fp, sl, r9` then `r8`) and `sub sp, #8` at 0x020017c8, and the
 * single interworking return at 0x0200229a restoring sp, r8-fp and
 * `pop {r0} ; bx r0`.  r0 holds the popped return address, so the owner is
 * `void`.  The body is entirely straight-line apart from one conditional
 * (0x02001b58) and the two `b.n` hops over interior literal pools.
 *
 * Three literal pools sit inside the span and are branched over, not executed:
 * 0x02001bf4-0x02001c3b (jumped by `b.n 0x02001c3c` at 0x02001bf2),
 * 0x02002082-0x020020a3 (jumped by `b.n 0x020020a4` at 0x02002080) and
 * 0x020022ac-0x020022c7 after the epilogue.  Each is reached only through
 * `ldr rN, [pc, #imm]`.  The next prologue at 0x020022c8 is the byte-exact
 * `assets/code/resource_374_c_020022c8.c`.
 *
 * The eight-byte stack frame carries the fifth and sixth arguments of the one
 * six-argument import, Func_08009180 at 0x020017f2 (`str r3, [sp, #0]` /
 * `str r2, [sp, #4]`), so it is spelled with six arguments here.
 *
 * Link base 0x02008000: the six in-image pool words 0x0200ac00, 0x0200ac14,
 * 0x0200ac90, 0x0200acf8, 0x0200ad74 and 0x0200adf0 are file offsets 0x2c00,
 * 0x2c14, 0x2c90, 0x2cf8, 0x2d74 and 0x2df0.  All are even, so they are data
 * (animation scripts, the same family 0x02000940, 0x02001274 and 0x02000bbc
 * pass to Func_0808a098 / Func_0808a0b0 / Func_0808a168), not Thumb entries.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, never from the
 * disassembler's `bl` annotations: 232 import veneers and 61 prologues in this
 * overlay, the latter all the byte-exact 0x020022c8 (22 sites) and 0x020022e0
 * (39 sites).  39 distinct callees for 293 sites.
 *
 * Long-lived register aliases recovered from the assembly, because the shift
 * chains that build them are far from their uses:
 *   r7 = 0x3000, fp = 0x1000, r9 = 0x5000 (fixed for the whole owner);
 *   r8 = 0x7000 until 0x02001d2a, then 0xb000;
 *   r6 = 0 at 0x02001cb4, then 0x9000 from 0x02001d00;
 *   r5 is reused for four unrelated values (0x0200ac00, the mask 254,
 *   0x0200ac90, 0xd000, 1, 0x0200adf0) and is tracked per use below;
 *   sl = 0xc000 from 0x02001d1a.
 * They are spelled as literals at each call site rather than as variables.
 *
 * Uncertainties:
 *  - Func_0808a010 appears at 30 sites with 10/20/40/60/80, consistent with a
 *    frame count but not asserted;  Func_0808a100 at 33 sites and
 *    Func_0808a128 at 22 always take a small mode number, so old-style
 *    declarations are used for every import;
 *  - the record returned by Func_0808a080 is touched only at +8/+16 (words,
 *    once) and at +0x23 (a byte whose bit 0 is cleared for handles 30/31 and
 *    set for handles 0/1), so byte offsets are used rather than a struct;
 *  - at 0x02002266 the workspace words written are at +448 (value 32) and
 *    +232 (value 60).  The second offset is built by `subs r3,#192` followed
 *    by `adds r3,#200` on the same register, i.e. 448-192+200 = 456 would be
 *    wrong: r3 is 32 when it is stored and 232 when it is used as the second
 *    displacement.  That reading is deliberate and is the only non-obvious
 *    arithmetic in the owner.
 */

/* Workspace pointer global. */
extern u8 *Data_03001ebc;

/* In-image data under the 0x02008000 link base. */
extern u8 Data_02002c00[];
extern u8 Data_02002c14[];
extern u8 Data_02002c90[];
extern u8 Data_02002cf8[];
extern u8 Data_02002d74[];
extern u8 Data_02002df0[];

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_020022c8(s32, s32);
void Func_020022e0(s32, s32, s32);

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
void Func_080000c0();          /* frame wait */
void Func_08009128();
void Func_08009180();          /* six arguments, last two stack-carried */
void Func_080091e0();
void Func_08015040();
void Func_08077260();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a060();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a168();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
u8 *Func_0808a228();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_0808a588();

void Func_020017c8(void)
{
    u8 *workspace;
    u8 *record;

    Func_08077260(1);
    Func_0808a018();
    Func_08009180(42, 53, 42, 54, 3, 1);
    Func_0808a210(0x00b40000, 0x00100000, 0x026a0000, 0);
    Func_08009128();
    Func_080000c0(1);

    Func_080091e0(Func_0808a080(22), 0);
    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);
    Func_080091e0(Func_0808a080(25), 0);
    Func_080091e0(Func_0808a080(26), 0);
    Func_080091e0(Func_0808a080(29), 0);

    Func_0808a1e0(0, 1);
    Func_0808a1e0(1, 1);
    Func_0808a1e0(17, 1);
    Func_0808a1e0(16, 1);
    Func_0808a1e0(15, 1);

    Func_0808a0f0(0, 0x00d00000, 0x032e0000);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(80);

    Func_0808a1e8(12, 0x101, 40);
    Func_020022e0(12, 0x7000, 20);
    Func_0808a170(0x11fa);
    Func_020022c8(12, 10);
    Func_0808a1e8(11, 258, 20);
    Func_020022e0(11, 0x1000, 10);
    Func_020022c8(11, 10);
    Func_0808a110(12, 3);
    Func_0808a010(10);
    Func_0808a138(11, 2);
    Func_020022c8(11, 10);
    Func_0808a1e8(12, 256, 40);

    Func_0808a090(12, 0x0000cccc, 0x00006666);
    Func_0808a0d0(12, 184, 0x26a);
    Func_020022e0(12, 0x3000, 60);
    Func_020022c8(12, 20);
    Func_0808a090(11, 0x10000, 0x8000);
    Func_0808a0d0(11, 168, 0x26a);
    Func_020022e0(11, 0xf000, 10);
    Func_0808a100(11, 4);
    Func_020022c8(11, 20);
    Func_020022e0(12, 0x7000, 10);
    Func_0808a138(12, 1);
    Func_0808a180(12, 0);
    Func_0808a010(10);

    Func_0808a090(30, 0x00026666, 0x00013333);
    Func_0808a0f0(30, 0x006e0000, 0x02e80000);
    Func_080000c0(2);
    Func_0808a100(30, 3);
    Func_0808a098(30, Data_02002c14);
    Func_0808a010(40);
    Func_0808a168(11, 0x0001001e, Data_02002c00);
    Func_0808a168(12, 0x0001001e, Data_02002c00);
    Func_0808a0a0(30);
    Func_0808a0a8(11);
    Func_0808a0a8(12);
    Func_0808a010(60);

    Func_0808a1e8(11, 0x105, 0);
    Func_0808a1e8(12, 0x105, 120);
    Func_0808a1b8(11, 0x1000, 0);
    Func_020022e0(12, 0x7000, 80);
    Func_020022e0(11, 0x5000, 40);
    Func_020022e0(11, 0x1000, 20);
    Func_0808a110(11, 3);
    Func_0808a010(20);
    Func_020022e0(12, 0x5000, 60);
    Func_020022e0(12, 0x3000, 40);
    Func_020022e0(12, 0x5000, 60);
    Func_0808a1e8(12, 0x101, 80);
    Func_0808a1b8(11, 0x3000, 0);
    Func_0808a0d0(12, 184, 0x276);
    Func_0808a010(20);
    Func_020022e0(12, 0x3000, 20);
    Func_020022e0(12, 0x5000, 20);
    Func_020022e0(12, 0x3000, 20);
    Func_0808a1e8(12, 0x101, 40);
    Func_0808a1e8(11, 0x101, 40);
    Func_0808a0d0(11, 168, 0x276);
    Func_0808a010(20);
    Func_020022e0(11, 0x3000, 40);
    Func_020022e0(11, 0x5000, 40);
    Func_020022e0(11, 0x3000, 40);
    Func_0808a1e8(11, 0x101, 40);
    Func_020022e0(11, 0x1000, 10);
    Func_020022c8(11, 20);
    Func_0808a110(12, 3);
    Func_020022c8(12, 10);
    Func_0808a1e8(11, 256, 20);
    Func_020022e0(11, 0x5000, 20);
    Func_020022e0(11, 0x3000, 20);
    Func_020022e0(11, 0x5000, 20);
    Func_020022e0(11, 0x5000, 60);
    Func_0808a110(11, 3);
    Func_020022c8(11, 10);

    record = Func_0808a080(30);
    if (record != NULL) {
        Func_0808a0f0(31, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_080000c0(2);

    record = Func_0808a080(30);
    record[0x23] &= 0xfe;
    record = Func_0808a080(31);
    record[0x23] &= 0xfe;

    Func_0808a1e0(30, 2);
    Func_0808a1e0(31, 2);
    Func_0808a090(31, 0x00039999, 0x0001cccc);
    Func_0808a100(31, 2);
    Func_0808a098(31, Data_02002c90);
    Func_0808a010(20);
    Func_0808a100(30, 3);
    Func_0808a090(30, 0x0004cccc, 0x00026666);
    Func_0808a0b0(30, Data_02002c90);
    Func_0808a010(60);
    Func_0808a138(12, 2);
    Func_020022e0(12, 0x7000, 10);
    Func_020022c8(12, 10);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_020022e0(11, 0x1000, 10);
    Func_0808a110(11, 3);
    Func_020022c8(11, 20);
    Func_0808a100(12, 3);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(20);

    Func_0808a090(11, 0x00026666, 0x00013333);
    Func_0808a090(12, 0x00026666, 0x00013333);
    Func_0808a098(11, Data_02002cf8);
    Func_0808a010(10);
    Func_0808a208(0x00026666, 0x00004ccc);

    record = Func_0808a228();
    record[0x55] = 0;

    Func_0808a210(0x00d70000, 0x00100000, 0x03210000, 1);
    Func_0808a010(10);
    Func_0808a098(12, Data_02002d74);
    Func_0808a0a0(12);
    Func_020022e0(12, 0x3000, 120);
    Func_0808a138(13, 2);
    Func_0808a010(20);
    Func_020022c8(13, 20);

    Func_0808a1b8(0, 0, 0);
    Func_020022e0(1, 0x9000, 20);
    Func_020022e0(0, 0xc000, 10);
    Func_020022e0(1, 0xb000, 10);
    Func_0808a100(0, 3);
    Func_0808a010(10);
    Func_0808a110(1, 3);
    Func_0808a010(40);
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_020022c8(16, 10);

    Func_0808a090(16, 0x10000, 0x8000);
    Func_0808a0d0(16, 216, 800);
    Func_0808a1b8(16, 0x4000, 0);
    Func_0808a060(180, 0);
    Func_0808a0d0(16, 264, 800);
    Func_0808a1b8(16, 0x6000, 0);
    Func_0808a1e8(1, 258, 40);
    Func_020022e0(1, 0xf000, 10);
    Func_020022c8(1, 10);
    Func_0808a110(17, 4);
    Func_020022c8(17, 10);
    Func_0808a1b8(1, 0x1000, 0);
    Func_0808a1e8(1, 0x103, 20);
    Func_0808a128(1, 4, 60);
    Func_0808a138(14, 2);
    Func_0808a010(20);
    Func_020022e0(14, 0xd000, 10);
    Func_020022c8(14, 60);

    Func_0808a1e8(1, 258, 0);
    Func_0808a1e8(16, 258, 0);
    Func_0808a1e8(17, 258, 0);
    Func_0808a1e8(18, 258, 0);
    Func_0808a1e8(19, 258, 80);
    Func_0808a1e8(17, 256, 0);
    Func_020022c8(17, 60);
    Func_0808a588(0, 17);
    Func_0808a588(1, 17);
    Func_0808a0d0(17, 216, 800);
    Func_0808a1b8(17, 0x4000, 0);
    Func_020022c8(17, 60);
    Func_0808a060(207, 0);
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0d0(17, 272, 816);
    Func_0808a1b8(17, 0x8000, 0);
    Func_0808a138(1, 2);
    Func_020022e0(1, 0x9000, 10);
    Func_020022c8(1, 10);
    Func_0808a1b8(14, 0x3000, 0);
    Func_020022e0(0, 0, 10);
    Func_0808a1e8(0, 0x101, 60);
    Func_0808a138(16, 1);
    Func_020022c8(16, 10);
    Func_020022e0(1, 0xf000, 10);
    Func_0808a1e8(1, 0x101, 20);
    Func_0808a110(16, 4);
    Func_020022c8(16, 10);
    Func_0808a110(18, 3);
    Func_0808a180(18, 0);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a100(18, 4);
    Func_0808a180(18, 0);
    Func_0808a130(18, 3);
    Func_0808a180(18, 0);
    Func_0808a110(16, 3);

    Func_0808a100(19, 3);
    Func_0808a100(17, 3);
    Func_0808a010(10);
    Func_0808a100(24, 3);
    Func_0808a100(18, 3);
    Func_0808a100(27, 3);
    Func_0808a010(10);
    Func_0808a100(28, 3);
    Func_0808a010(10);
    Func_0808a100(25, 3);
    Func_0808a100(20, 3);
    Func_0808a110(21, 3);

    Func_0808a128(15, 2, 10);
    Func_0808a128(15, 4, 40);
    Func_020022c8(15, 10);
    Func_0808a1b8(1, 0xb000, 0);
    Func_020022e0(0, 0xc000, 20);
    Func_020022e0(15, 0xd000, 10);
    Func_020022c8(15, 10);
    Func_020022e0(15, 0x9000, 20);
    Func_020022e0(15, 0x5000, 10);

    Func_0808a100(11, 3);
    Func_0808a100(14, 3);
    Func_0808a100(17, 3);
    Func_0808a100(20, 3);
    Func_0808a100(23, 3);
    Func_0808a100(26, 3);
    Func_0808a100(29, 3);
    Func_0808a010(10);
    Func_0808a100(12, 3);
    Func_0808a100(15, 3);
    Func_0808a100(18, 3);
    Func_0808a100(21, 3);
    Func_0808a100(24, 3);
    Func_0808a100(27, 3);
    Func_0808a010(10);
    Func_0808a100(13, 3);
    Func_0808a100(16, 3);
    Func_0808a100(19, 3);
    Func_0808a100(22, 3);
    Func_0808a100(25, 3);
    Func_0808a110(28, 3);
    Func_0808a010(80);

    Func_0808a128(11, 4, 0);
    Func_0808a128(14, 4, 0);
    Func_0808a128(17, 4, 0);
    Func_0808a128(20, 4, 0);
    Func_0808a128(23, 4, 0);
    Func_0808a128(26, 4, 0);
    Func_0808a128(29, 4, 0);
    Func_0808a128(12, 4, 0);
    Func_0808a128(15, 4, 0);
    Func_0808a128(18, 4, 0);
    Func_0808a128(21, 4, 0);
    Func_0808a128(24, 4, 0);
    Func_0808a128(27, 4, 0);
    Func_0808a128(13, 4, 0);
    Func_0808a128(16, 4, 0);
    Func_0808a128(19, 4, 0);
    Func_0808a128(22, 4, 0);
    Func_0808a128(25, 4, 0);
    Func_0808a128(28, 4, 0);

    Func_08015040(0x1214, 1);
    Func_0808a010(80);

    record = Func_0808a080(0);
    record[0x23] |= 1;
    record = Func_0808a080(1);
    record[0x23] |= 1;

    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 80);
    Func_020022e0(0, 0x4000, 10);
    Func_020022e0(1, 0x5000, 20);
    Func_0808a090(0, 0x0000cccc, 0x00006666);
    Func_0808a098(0, Data_02002df0);
    Func_0808a010(20);
    Func_0808a208(0x00006666, 0x00000ccc);
    Func_0808a210(0x00d80000, 0x00100000, 0x03890000, 1);
    Func_0808a010(20);
    Func_0808a090(1, 0x0000cccc, 0x00006666);
    Func_0808a098(1, Data_02002df0);
    Func_0808a010(60);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 32;
    *(s32 *)(workspace + 232) = 60;

    Func_0808a368();
    Func_0808a370();
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a248(10);
    Func_0808a020();
}
