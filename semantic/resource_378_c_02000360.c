#include "types.h"

/*
 * Resource 378 owner at 0x02000360 (392 bytes, through 0x020004e7).
 *
 * The body has 44 imported/direct calls and nine branch sites.  The word at
 * 0x020004e4 is the sole literal-pool entry; 0x020004e8 is the next
 * prologue.  Three tests gate a common finishing sequence: the first two
 * test the result of Func_02003982/Func_020039ac/Func_020039c8 (0, 0), and
 * the last path checks the unsigned halfword at offset +6 of the actor
 * returned by Func_020039e2/Func_020039f2 (8).  The successful range is
 * inclusive [0xa000, 0xe000].
 *
 * The three early exits (the three checked-flag branches) do NOT run the
 * shared "common" tail (Func_0200187c .. Func_0808a248) -- they jump
 * straight past it to the final no-arg call before the epilogue.  Only the
 * success path (falling into "common") and the fallback path run the full
 * tail.  This mirrors the reference machine layout exactly (two separate
 * landing labels, 0x4aa and 0x4da).
 *
 * The two byte writes at +0x5a are observable state changes, not temporary
 * compiler bookkeeping: the first clears bit 0 and the second sets it.
 * The direct call at 0x020004aa resolves to the in-overlay owner at
 * 0x0200187c (the listing's PC-relative spelling is not the loaded target),
 * and is retained as an ordinary call here.
 *
 * Call symbols are strictly per-site (see craft brief); several veneer
 * addresses are shared by two call sites that pass a different number of
 * arguments or use the return value differently, so all externs here are
 * declared old-style (no parameter list) to allow that reuse.
 */

extern void Func_02003954();
extern s32 Func_02003a12();
extern void Func_02003a22();
extern s32 Func_02003982();
extern void Func_020039ee();
extern void Func_02003974();
extern void Func_0200397c();
extern void Func_02003a4c();
extern s32 Func_020039ac();
extern void Func_02003998();
extern void Func_02003a68();
extern s32 Func_020039c8();
extern void Func_020039b4();
extern u8 *Func_020039e2();
extern u8 *Func_020039f2();
extern void Func_02003ac4();
extern u8 *Func_02003a20();
extern void Func_02003a6c();
extern u8 *Func_02003a40();
extern void Func_02003a28();
extern void Func_02003ab0();
extern void Func_02003a36();
extern void Func_02003aa0();
extern void Func_02003aa2();
extern void Func_02003a50();
extern void Func_02003aba();
extern void Func_02003b3e();
extern void Func_02003ad4();
extern void Func_02003ad0();
extern void Func_02003a7e();
extern void Func_02003b62();
extern void Func_02003af8();
extern void Func_0200187c(void);
extern void Func_02003bbc();
extern void Func_02003bca();
extern void Func_02003aa8();
extern void Func_02003bf6();
extern void Func_02003c02();
extern void Func_02003aaa();
extern void Func_02003bd8();

void Func_02000360(void)
{
    u8 *actor;

    Func_02003954();
    Func_02003a12(0x1164);
    Func_02003a22(8, 0);

    if (Func_02003982(0, 0) == 0) {
        Func_020039ee(8, 3);
        Func_02003974(20);
        goto tail_only;
    }

    Func_0200397c(20);
    Func_02003a4c(8, 0);
    if (Func_020039ac(0, 0) != 0) {
        goto tail_only;
    }

    Func_02003998(20);
    Func_02003a68(8, 0);
    if (Func_020039c8(0, 0) != 0) {
        goto tail_only;
    }

    Func_020039b4(20);
    actor = Func_020039e2(8);
    if (*(u16 *)(actor + 6) < 0xa000) {
        goto fallback;
    }

    actor = Func_020039f2(8);
    if (*(u16 *)(actor + 6) > 0xe000) {
        goto fallback;
    }

    Func_02003a12(8, 0x8000, 0x4000);
    Func_02003ac4(8, 0, 0);
    Func_020039f2(10);

    *(Func_02003a20(8) + 0x5a) &= (u8)~1;

    Func_02003a6c(8, 0x98, 0x78);
    Func_02003a12(1);

    *(Func_02003a40(8) + 0x5a) |= 1;

    Func_02003a28(20);
    Func_02003ab0(8, 3);
    Func_02003a36(20);
    Func_02003aa0(0, 0xa8, 0x78);
    Func_02003aa2(0, 0xc0, 0xa8);
    Func_02003a50(20);
    Func_02003aba(8, 0xa8, 0x78);
    Func_02003b3e(8, 0x3000, 0);
    Func_02003ad4(0);
    goto common;

fallback:
    Func_02003ad0(0, 0xc0, 0xa8);
    Func_02003a7e(20);
    Func_02003b62(8, 0x3000, 0);
    Func_02003af8(0);

common:
    Func_0200187c();
    Func_02003bbc(0, 0);
    Func_02003bca(120);
    Func_02003aa8(120);
    Func_02003bf6(86);
    Func_02003c02();
    Func_02003aaa(0x9f0);
    Func_02003bd8(30);

tail_only:
    Func_02003ad4();
}
