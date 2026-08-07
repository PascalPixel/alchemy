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
s32 Func_0200583c();
void Func_02005866();
s32 Func_0200584c();
u8 *Func_020058a6();
u8 *Func_020058ae();
void Func_0200585e();
void Func_02005a5c();
void Func_020057ea();
void Func_02005a68();
void Func_020059c0();
u8 *Func_020058ee();
void Func_02005962();
void Func_02005976();
void Func_020058f4();
void Func_02005aba();
void Func_020058d2();
void Func_020058e0();
void Func_02005acc();
void Func_0200590a();
void Func_02005a32();
u8 *Func_02005960();
void Func_02002538();
void Func_02005932();
s32 Func_02005930();
s32 Func_0200593a();
s32 Func_02005946();
u8 *Func_020059a0();
void Func_02002ce8();
void Func_02005a0a();
void Func_02002cfc();
void Func_02005a1e();
void Func_0200598e();
void Func_020059b2();

                    

                     

                     

                     

                     

void Func_02001154(void)
{
    u8 *p;
    u8 *q;
    u8 *saved;      /* r8: q + 35 */
    u8 held;        /* sl */
    s32 bits;       /* r5 */
    u8 *t;
    u8 *rec;

    if (Func_0200583c(0x312) != 0) {
        return;
    }

    Func_02005866();

    if (Func_0200584c(0x832) == 0) {
        p = Func_020058a6(13);
        saved = q + 35;
        q = Func_020058ae(0);
        bits = (*(u8 **)(q + 80))[9];
        held = *saved;

        Func_0200585e(0x40000, 0x40000, 0x10000);  /* 128<<11, 128<<11, 128<<9 */
        Func_02005a5c(141);
        Func_020057ea(40);
        Func_02005a68(145);
        Func_020059c0(0, 3);

        t = Func_020058ee(0);
        t[35] |= 2;

        Func_02005962(13, 0, 0x2bf0000);

        *(s32 *)(p + 12) += 0x500000;              /* 160 << 15 */
        *(s32 *)(p + 60) = *(s32 *)(p + 12);
        *(s32 *)(p + 68) = 0x8000;                 /* 128 << 8 */
        *(s32 *)(p + 48) = 0x18000;                /* 192 << 9 */
        *(s32 *)(p + 52) = 0x18000;

        Func_02005976(13, 64, 0x2bf);
        Func_020058f4(40);
        Func_02005aba(0x121);

        /* (bits << 28) >> 30 in the original: bits 2-3 of the byte. */
        bits = (bits >> 2) & 3;
        Func_020058d2(-1, -1, 0xe666);

        Func_020058e0();
        Func_02005acc();
        Func_0200590a(0x832);
        Func_02005a32(0, bits);

        t = Func_02005960(0);
        t[35] |= 1;

        *saved = held;
    }

    Func_02002538();
    Func_02005932(0x312);

    if (Func_02005930(0x837) != 0 && Func_0200593a(0x841) == 0 &&
        Func_02005946(0x30c) == 0) {               /* 195 << 2 */
        rec = Func_020059a0(0);
        if (*(s32 *)(rec + 16) > 0x02b4ffff) {     /* signed */
            Func_02002ce8(75, 0x2cb);
            Func_02005a0a(0, 67, 0x2f5);
        } else {
            Func_02002cfc(62, 0x29d);
        }
            Func_02005a1e(0, 27, 0x273);
        Func_0200598e(0x30c);
    }

    Func_020059b2();
}
