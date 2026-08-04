#include "types.h"

/*
 * resource_38f owner at 0x020003c8, 716 bytes: code 0x020003c8-0x0200066d,
 * two bytes of alignment at 0x0200066e, and the nine-word literal pool at
 * 0x02000670-0x02000693.  The next inventory row starts at 0x02000694 with its
 * own `push {r5, lr}`, so the owner is exactly its advertised span.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} / sub sp, #8`;
 * epilogue `add sp, #8 / pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0`.  r0 holds the popped return address, so the owner returns nothing,
 * and no incoming argument register is read before the first branch, so it
 * takes none - the caller's r0-r3 reach Func_0808a018 unchanged.
 *
 * The 8-byte stack frame carries the fifth and sixth arguments of the
 * Func_08009180 / Func_080091c0 family, written as `str rN, [sp, #0]` and
 * `[sp, #4]` before each site.  The byte-exact sibling
 * assets/code/resource_38f_c_02002910.c documents the same six-argument shape.
 *
 * Call accounting: 58 `bl` sites, 29 distinct import veneers, no intra-overlay
 * call and no `call_via` slot; all resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 03c8`.  Every site appears
 * below exactly once.  (The inventory row advertises `calls=54`; the extra
 * four are the sites inside the ten-iteration spawn loop, which the row counts
 * once.)  The disassembler's own `bl` annotations are wrong in the usual
 * overlay way and were not used.
 *
 * This is one scripted scene, bracketed by the Func_0808a018 / Func_0808a020
 * pair that resource_373 and resource_39f scripted owners use, with the same
 * import shapes those files settled: Func_0808a010(frames) waits,
 * Func_0808a080(selector) returns the actor record, Func_0808a090 sets a 16.16
 * scale pair, Func_0808a0d0/Func_0808a1b8 drive position and motion,
 * Func_080770c8(id) is the one-argument service.  The object ABI in the spawn
 * loop is the one the byte-exact sibling
 * assets/code/resource_38f_c_02002608.c proves: Func_080090c8(kind, x, y, z)
 * returns the object or 0, +0x50 is the sub-record whose +0x09 bitfield is
 * masked, and +0x55 is cleared.
 *
 * Data_0200ae20 and Data_0200ae34 are in-image data under this overlay's
 * confirmed 0x02008000 link base (jump-table witness in
 * semantic/overlays/resource_38f_c_02000304.c): file offsets 0x2e20 and
 * 0x2e34.  The remaining pool words are plain constants.
 *
 * UNCERTAINTIES:
 *  - Func_0808a218 at 0x020003f0 is reached with no argument register written
 *    since the preceding call, so it is spelled with no arguments; whether it
 *    actually reads the leftovers is not decidable here.
 *  - The spawn loop's per-object +0x24 is `((i & 3) << 16 | 0x10000) >> 1`
 *    negated on odd i, and +0x2c is the constant 0x10000.  That reads as a
 *    signed horizontal velocity paired with a fixed vertical one, but only the
 *    arithmetic is asserted.
 *  - +0x64 receives `((u32)(rand * 40) >> 16) + 40` as a halfword - a lifetime
 *    in 40..79 frames on the usual reading of Func_080000f8 as the image's
 *    random source, which the three no-argument call sites in
 *    semantic/overlays/resource_38f_c_020027ac.c establish.
 *  - Func_08009180 and Func_080091c0 take six plain integers each; only the
 *    varying ones are commented, their meaning is not reconstructed.
 */

struct Sub {
    u8 pad00[9];
    u8 f09;
};

struct Obj {
    u8 pad00[0x24];
    s32 f24;
    u8 pad28[4];
    s32 f2c;
    u8 pad30[0x20];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
    u8 pad56[14];
    u16 f64;
};

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200ae20[];
extern u8 Data_0200ae34[];

/* Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0c0();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a168();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_080f9010();
void Func_080770c8();
void Func_08009080();
void Func_08009098();
struct Obj *Func_080090c8();
void Func_08009180();
void Func_080091c0();
void Func_080091e0();
void Func_080091f0();
s32 Func_080000f8();

void Func_020003c8(void)
{
    u8 *record;
    struct Obj *p;
    struct Sub *sub;
    s32 i;
    s32 x;
    s32 speed;

    Func_0808a018();

    Func_0808a208(0x20000, 0x4000);           /* 2.0 and 0.25 in 16.16 */
    Func_0808a210(0x00a80000, 0, 0x00f60000, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_0808a1e8(8, 256, 40);
    Func_0808a138(8, 2);
    Func_0808a170(0x1786);
    Func_0808a180(8, 0);

    Func_0808a208(0x6666, 0xccc);             /* 0.4 and 0.05 */
    Func_0808a210(0x00a80000, 0, 0x00ea0000, 1);
    Func_0808a090(0, 0x9999, 0x4ccc);         /* 0.6 and 0.3 */
    Func_0808a0d0(0, 174, 278);
    Func_0808a1b8(0, 0xe000, 20);             /* 224 << 8 */
    Func_0808a110(0, 3);
    Func_0808a010(20);

    Func_0808a100(8, 3);
    Func_0808a180(8, 0);
    Func_0808a1b8(8, 0x9000, 20);             /* 144 << 8 */
    Func_0808a138(8, 1);
    Func_0808a010(20);

    record = Func_0808a080(8);
    record[0x5a] = record[0x5a] & 0xfe;

    Func_0808a090(8, 0x20000, 0x10000);       /* 2.0 and 1.0 */
    Func_0808a128(8, 2, 0);
    Func_0808a0c0(8, 224, 197);
    Func_080f9010(176);
    Func_0808a010(10);
    Func_0808a0c0(8, 234, 200);
    Func_0808a010(10);
    Func_080f9010(198);
    Func_0808a010(30);

    Func_08009180(91, 0, 72, 9, 5, 4);
    Func_0808a010(12);
    Func_08009180(91, 4, 72, 9, 5, 4);
    Func_0808a010(9);
    Func_0808a1b8(0, 0xc000, 0);              /* 192 << 8 */
    Func_08009180(91, 8, 72, 9, 5, 5);
    Func_0808a010(6);
    Func_08009180(91, 13, 72, 9, 5, 6);
    Func_0808a010(3);
    Func_080f9010(188);

    /* Ten objects along a line: x starts at 148.0 and steps by 4.0. */
    x = 0x00940000;
    for (i = 0; i <= 9; i++) {
        p = Func_080090c8(222, x, 0, 0x01020000);
        if (p != 0) {
            p->f55 = 0;
            Func_080091e0(p, 0);
            sub = p->f50;
            sub->f09 = sub->f09 & ~13;
            p->f64 = (u16)((((u32)(Func_080000f8() * 40)) >> 16) + 40);
            speed = (s32)(((i & 3) << 16) + 0x10000) >> 1;
            p->f2c = 0x10000;
            p->f24 = speed;
            if ((i & 1) != 0) {
                p->f24 = -speed;
            }
            Func_08009080(p, 1);
            Func_08009098(p, Data_0200ae20);
        }
        x = x + 0x40000;
    }

    Func_08009180(91, 19, 72, 9, 5, 7);
    Func_080091c0(23, 11, 5, 7, 8, 11);
    Func_080091f0(0, 0x40000, 0x10000);
    Func_0808a010(10);
    Func_0808a128(0, 6, 0);
    Func_0808a010(20);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a168(8, 0x10000, Data_0200ae34);
    Func_0808a010(60);
    Func_0808a1e8(0, 258, 60);
    Func_080770c8(0x847);

    Func_0808a020();
}
