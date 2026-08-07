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
 * `bun tools/lib/overlay_call_targets.ts resource_38f 03c8`.  Every site appears
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
void Func_02002e06();
void Func_02002f22();
void Func_02002f3a();
void Func_02002f46();
void Func_02002e24();
void Func_02002f38();
void Func_02002ee8();
void Func_02002f16();
void Func_02002f2e();
void Func_02002f66();
void Func_02002f7e();
void Func_02002ea0();
void Func_02002ee4();
void Func_02002f78();
void Func_02002f20();
void Func_02002e8e();
void Func_02002f26();
void Func_02002f86();
void Func_02002fa2();
void Func_02002f62();
void Func_02002eb8();
u8 *Func_02002ee6();
void Func_02002f0e();
void Func_02002f80();
void Func_02002f52();
void Func_02003090();
void Func_02002ef6();
void Func_02002f68();
void Func_02002f06();
void Func_020030ac();
void Func_02002f12();
void Func_02002f2c();
void Func_02002f0c();
void Func_02002f42();
void Func_02003046();
void Func_02002f64();
void Func_02002f7c();
void Func_02003122();
struct Obj *Func_02002f44();
void Func_02002f90();
s32 Func_02002f4a();
void Func_02002f8e();
void Func_02002f9e();
void Func_02002fee();
void Func_0200300a();
void Func_02003028();
void Func_020030f0();
void Func_02003056();
void Func_0200304c();
void Func_02003140();
void Func_02003076();
void Func_0200318a();
void Func_02003080();
void Func_0200309c();

                     

                     

                     

                     

                     

                     

                     

                            

                     

void Func_020003c8(void)
{
    u8 *record;
    struct Obj *p;
    struct Sub *sub;
    u32 i;
    s32 x;
    s32 speed;

    Func_02002e06();

    Func_02002f22(0x20000, 0x4000);           /* 2.0 and 0.25 in 16.16 */
    Func_02002f3a(0x00a80000, 0, 0x00f60000, 1);
    Func_02002f46();
    Func_02002e24(20);

    Func_02002f38(8, 256, 40);
    Func_02002ee8(8, 2);
    Func_02002f16(0x1786);
    Func_02002f2e(8, 0);

    Func_02002f66(0x6666, 0xccc);             /* 0.4 and 0.05 */
    Func_02002f7e(0x00a80000, 0, 0x00ea0000, 1);
    Func_02002ea0(0, 0x9999, 0x4ccc);         /* 0.6 and 0.3 */
    Func_02002ee4(0, 174, 278);
    Func_02002f78(0, 0xe000, 20);             /* 224 << 8 */
    Func_02002f20(0, 3);
    Func_02002e8e(20);

    Func_02002f26(8, 3);
    Func_02002f86(8, 0);
    Func_02002fa2(8, 0x9000, 20);             /* 144 << 8 */
    Func_02002f62(8, 1);
    Func_02002eb8(20);

    record = Func_02002ee6(8);
    record[0x5a] = record[0x5a] & 0xfe;

    Func_02002f0e(8, 0x20000, 0x10000);       /* 2.0 and 1.0 */
    Func_02002f80(8, 2, 0);
    Func_02002f52(8, 224, 197);
    Func_02003090(176);
    Func_02002ef6(10);
    Func_02002f68(8, 234, 200);
    Func_02002f06(10);
    Func_020030ac(198);
    Func_02002f12(30);

    Func_02002ef6(91, 0, 72, 9, 5, 4);
    Func_02002f2c(12);
    Func_02002f0c(91, 4, 72, 9, 5, 4);
    Func_02002f42(9);
    Func_02003046(0, 0xc000, 0);              /* 192 << 8 */
    Func_02002f2e(91, 8, 72, 9, 5, 5);
    Func_02002f64(6);
    Func_02002f46(91, 13, 72, 9, 5, 6);
    Func_02002f7c(3);
    Func_02003122(188);

    /* Ten objects along a line: x starts at 148.0 and steps by 4.0. */
    x = 0x00940000;
    for (i = 0; i <= 9; i++) {
        x = x + 0x40000;
        p = Func_02002f44(222, x, 0, 0x01020000);
        if (p != 0) {
            p->f55 = 0;
            Func_02002f90(p, 0);
            sub = p->f50;
            p->f24 = speed;
            sub->f09 = sub->f09 & ~13;
            p->f64 = (u16)((((u32)(Func_02002f4a() * 40)) >> 16) + 40);
            p->f2c = 0x10000;
            if ((i & 1) != 0) {
                p->f24 = -speed;
            }
            Func_02002f8e(p, 1);
            Func_02002f9e(p, Data_0200ae20);
            speed = (s32)(((i & 3) << 16) + 0x10000) >> 1;
        }
    }

    Func_02002fee(91, 19, 72, 9, 5, 7);
    Func_0200300a(23, 11, 5, 7, 8, 11);
    Func_02003028(0, 0x40000, 0x10000);
    Func_02003046(10);
    Func_020030f0(0, 6, 0);
    Func_02003056(20);
    Func_0200304c(-1, -1, 0xe666);
    Func_02003140(8, 0x10000, Data_0200ae34);
    Func_02003076(60);
    Func_0200318a(0, 258, 60);
    Func_02003080(0x847);

    Func_0200309c();
}
