#include "types.h"

/*
 * Resource 372 guarded cutscene step at 0x02000d5c (360 bytes, 27 calls).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000d5c, matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000e96.  `pop {r0} ; bx r0` —
 * r0 is the popped return address, so the owner returns nothing.
 * 0x02000e9c-0x02000ec3 is the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Same three-part shape as `semantic/overlays/resource_372_c_02001348.c`: an
 * outer flag guard (0x310) that skips the whole body, a one-shot guard (0x830)
 * around the object setup, and a trailing guarded section that picks one of two
 * script branches on a signed comparison of the word at +12 of a queried record
 * against 0x800000.
 *
 * Object layout touched here (p = 0x020054ac(11), o = *(void **)(p + 80)):
 *   p[+0x0c]  s32, advanced by 0xf00000 and mirrored to p[+0x3c]
 *   p[+0x30]  s32 = 0x18000        p[+0x34]  s32 = 0x18000
 *   p[+0x44]  s32 = 0x6666
 *   p[+0x23]  u8  flag byte: bit 0 cleared then set
 *   o[+9]     u8  field: bits 2-3 replaced by 4, then bits 2-3 set
 * The +0x50 / +9 pair matches the byte-exact `assets/code/resource_372_c_
 * 02000434.c`, which reaches the same u8 field through the same +0x50 pointer.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; for this overlay that address is an import identity, not a place to
 * disassemble.  0x02001cf4, 0x020028d8 and 0x020028ec are in that class.
 *
 * Uncertainties: the flag identifiers (0x310, 0x830, 0x837, 0x841, 0x30c) and
 * the message identifiers (0x121, 0x123, 0x146, 0x14f, 0x396, 0x3bd) are not
 * resolved; 0xe666 recurs across this overlay as the third argument of a
 * (-1, -1, k) call.
 */

/* Imports, old-style: interfaces unknown. */
s32 Func_080770c0();
void Func_0808a018();
u8 *Func_0808a080();
void Func_080091f0();
void Func_080f9010();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a0d0();
void Func_0808a010();
void Func_080091f8();
void Func_0808a5e8();
void Func_080770c8();
void Func_02000ec4();
void Func_02001a64();
void Func_0808a020();

void Func_02000d5c(void)
{
    u8 *p;
    u8 *o;
    u8 *flag;
    u8 *rec;

    if (Func_080770c0(0x310) != 0) {   /* 196 << 2 */
        return;
    }

    Func_0808a018();

    if (Func_080770c0(0x830) == 0) {   /* 131 << 4 */
        p = Func_0808a080(11);
        o = *(u8 **)(p + 80);
        Func_080091f0(0x40000, 0x40000, 0x10000);   /* 128<<11, 128<<11, 128<<9 */
        Func_080f9010(141);
        flag = p + 35;
        Func_080000c0(40);
        Func_080f9010(145);

        o[9] = (u8)((o[9] & ~12) | 4);
        *flag = (u8)(*flag & 254);

        Func_0808a0f0(11, 0x1d90000, 0x3a40000);    /* 233 << 18 */

        *(s32 *)(p + 60) = *(s32 *)(p + 12);
        *(s32 *)(p + 12) += 0xf00000;               /* 240 << 16 */
        *(s32 *)(p + 48) = 0x18000;                 /* 192 << 9 */
        *(s32 *)(p + 52) = 0x18000;
        *(s32 *)(p + 68) = 0x6666;

        Func_0808a0d0(11, 0x158, 0x3a4);            /* 172<<1, 233<<2 */

        o[9] |= 12;
        *flag = (u8)(*flag | 1);

        Func_0808a010(40);
        Func_080f9010(0x121);
        Func_080091f0(-1, -1, 0xe666);
        Func_080091f8();
        Func_0808a5e8();
        Func_080770c8(0x830);
    }

    Func_02000ec4();
    Func_080770c8(0x310);

    if (Func_080770c0(0x837) != 0 && Func_080770c0(0x841) == 0 &&
        Func_080770c0(0x30c) == 0) {                /* 195 << 2 */
        rec = Func_0808a080(0);
        if (*(s32 *)(rec + 12) > 0x800000) {        /* 128 << 16, signed */
        }
            Func_02001a64(0x146, 0x396);            /* 163 << 1 */
            Func_0808a0d0(0, 0x123, 0x396);
        } else {
            Func_02001a64(0x14f, 0x3bd);
        Func_080770c8(0x30c);
    }

    Func_0808a020();
}
