#include "types.h"

/*
 * Resource 372 guarded cutscene step at 0x02001154 (412 bytes, 33 calls).
 *
 * Complete owner: `push {r5, r6, lr} / mov r6, sl / mov r5, r8 /
 * push {r5, r6}` at 0x02001154, matching `pop {r3, r5} / mov r8, r3 /
 * mov sl, r5 / pop {r5, r6} / pop {r0} / bx r0` at 0x020012b0.
 * `pop {r0} ; bx r0` — r0 is the popped return address, so the owner returns
 * nothing.  0x020012bc-0x020012ef is the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Fourth member of the guarded-step family (`..._02000d5c.c`, `..._02000f38.c`,
 * `..._02001348.c`): outer flag guard 0x312, one-shot object-setup guard 0x832,
 * then a trailing section guarded by 0x837 / 0x841 / 0x30c that picks one of two
 * script branches on a signed comparison of the word at +16 of a queried record
 * against 0x02b4ffff.
 *
 * Two values are carried across calls in the saved high registers, which is why
 * the prologue saves r8 and sl:
 *   r8  = q + 35, where q = 0x020058ae(0); the byte there is read into sl before
 *         the sequence runs and written back unchanged at the end — a
 *         save/restore of one flag byte around the whole setup.
 *   r5  = bits 2-3 of the byte at +9 of *(void **)(q + 80), extracted by
 *         `lsls #28 / lsrs #30` around the 0x020058d2 call and passed as the
 *         second argument of 0x02005a32.
 *
 * Object layout touched here (p = 0x020058a6(13)):
 *   p[+0x0c]  s32, advanced by 0x500000 and mirrored to p[+0x3c]
 *   p[+0x30]  s32 = 0x18000        p[+0x34]  s32 = 0x18000
 *   p[+0x44]  s32 = 0x8000
 * — the same field set as the sibling steps.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.  0x02002538, 0x02002ce8 and 0x02002cfc are in that class.
 *
 * Uncertainties: all flag and message identifiers are unresolved; 0xe666
 * recurs across this overlay as the third argument of a (-1, -1, k) call.
 */

/* Imports, old-style: interfaces unknown. */
s32 Func_080770c0();
void Func_0808a018();
u8 *Func_0808a080();
void Func_080091f0();
void Func_080f9010();
void Func_080000c0();
void Func_0808a1e0();
void Func_0808a0f0();
void Func_0808a0d0();
void Func_0808a010();
void Func_080091f8();
void Func_0808a5e8();
void Func_080770c8();
void Func_020012f0();
void Func_02001a64();
void Func_0808a020();

void Func_02001154(void)
{
    u8 *p;
    u8 *q;
    u8 *saved;      /* r8: q + 35 */
    u8 held;        /* sl */
    s32 bits;       /* r5 */
    u8 *t;
    u8 *rec;

    if (Func_080770c0(0x312) != 0) {
        return;
    }

    Func_0808a018();

    if (Func_080770c0(0x832) == 0) {
        p = Func_0808a080(13);
        saved = q + 35;
        q = Func_0808a080(0);
        bits = (*(u8 **)(q + 80))[9];
        held = *saved;

        Func_080091f0(0x40000, 0x40000, 0x10000);  /* 128<<11, 128<<11, 128<<9 */
        Func_080f9010(141);
        Func_080000c0(40);
        Func_080f9010(145);
        Func_0808a1e0(0, 3);

        t = Func_0808a080(0);
        t[35] |= 2;

        Func_0808a0f0(13, 0, 0x2bf0000);

        *(s32 *)(p + 12) += 0x500000;              /* 160 << 15 */
        *(s32 *)(p + 60) = *(s32 *)(p + 12);
        *(s32 *)(p + 68) = 0x8000;                 /* 128 << 8 */
        *(s32 *)(p + 48) = 0x18000;                /* 192 << 9 */
        *(s32 *)(p + 52) = 0x18000;

        Func_0808a0d0(13, 64, 0x2bf);
        Func_0808a010(40);
        Func_080f9010(0x121);

        /* (bits << 28) >> 30 in the original: bits 2-3 of the byte. */
        bits = (bits >> 2) & 3;
        Func_080091f0(-1, -1, 0xe666);

        Func_080091f8();
        Func_0808a5e8();
        Func_080770c8(0x832);
        Func_0808a1e0(0, bits);

        t = Func_0808a080(0);
        t[35] |= 1;

        *saved = held;
    }

    Func_020012f0();
    Func_080770c8(0x312);

    if (Func_080770c0(0x837) != 0 && Func_080770c0(0x841) == 0 &&
        Func_080770c0(0x30c) == 0) {               /* 195 << 2 */
        rec = Func_0808a080(0);
        if (*(s32 *)(rec + 16) > 0x02b4ffff) {     /* signed */
            Func_02001a64(75, 0x2cb);
            Func_0808a0d0(0, 67, 0x2f5);
        } else {
            Func_02001a64(62, 0x29d);
        }
            Func_0808a0d0(0, 27, 0x273);
        Func_080770c8(0x30c);
    }

    Func_0808a020();
}
