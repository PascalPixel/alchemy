#include "types.h"

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Func_080f9010, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern void Func_02007e72(void);              /* Func_0808a018 veneer #1 (mode==0 arm) */
extern void Func_02007fc6(void);              /* Func_0808a360 veneer #1 */
extern void Func_02007fda(void);              /* Func_0808a370 veneer #1 */
extern void Func_02007e78(s32 frames);        /* Func_0808a010 veneer #1 */
extern void Func_02008026(s32 cue);           /* Func_080f9010 veneer #1 */
extern void Func_020062e0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200686c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02007e92(s32 frames);        /* Func_0808a010 veneer #2 */
extern void Func_02007ea6(void);              /* Func_0808a020 veneer #1 */

extern void Func_02008046(s32 cue);           /* Func_080f9010 veneer #2 (main arm) */
extern void Func_02007eaa(void);              /* Func_0808a018 veneer #2 */
extern void Func_02007ffe(void);              /* Func_0808a360 veneer #2 */
extern void Func_02008012(void);              /* Func_0808a370 veneer #2 */
extern void Func_02007eba(s32 frames);        /* Func_0808a010 veneer #3 */
extern void Func_0200806a(s32 cue);           /* Func_080f9010 veneer #3 */
extern void Func_02006324(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_020068b0(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern void Func_02007ed6(s32 frames);        /* Func_0808a010 veneer #4 */

extern void Func_02007d26(s32 frames);        /* Func_080000c0 veneer (loop body) */
extern s32 Func_02008092(void);               /* Func_080f9048 veneer (loop check) */

extern void Func_02008094(s32 cue);           /* Func_080f9010 veneer #4 */
extern void Func_0200634e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020068da(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_020080a8(s32 cue);           /* Func_080f9010 veneer #5 */
extern void Func_02007f06(s32 frames);        /* Func_0808a010 veneer #5 */
extern void Func_020068ee(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_020080bc(s32 cue);           /* Func_080f9010 veneer #6 */
extern void Func_02007f1a(s32 frames);        /* Func_0808a010 veneer #6 */
extern void Func_0200637c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_02006908(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_020080d6(s32 cue);           /* Func_080f9010 veneer #7 */
extern void Func_02007f34(s32 frames);        /* Func_0808a010 veneer #7 */
extern void Func_02006396(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_02006922(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_020080f0(s32 cue);           /* Func_080f9010 veneer #8 */
extern void Func_020080cc(void);              /* Func_0808a4f0 veneer */
extern void Func_02007f60(void);              /* Func_0808a020 veneer #2 */
extern void Func_02007f16(s32 flag);          /* Func_080770c8 veneer */

void Func_02003468(s32 mode)
{
    if (mode == 0) {
        Func_02007e72();
        Func_02007fc6();
        Func_02007fda();
        Func_02007e78(30);
        Func_02008026(0x59);
        Func_020062e0(0);
        Func_0200686c(1, 0);
        Func_02007e92(120);
        Func_02007ea6();
        return;
    }

    Func_02008046(0xf7);
    Func_02007eaa();
    Func_02007ffe();
    Func_02008012();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Func_02007eba(30);
    Func_0200806a(mode + 0x5a);
    Func_02006324(mode);
    Func_020068b0(1, 0);
    Func_02007ed6(120);

    goto check_transition;
wait_transition:
    Func_02007d26(1);
check_transition:
    if (Func_02008092() != 0)
        goto wait_transition;

    Func_02008094(0x121);
    Func_0200634e(5);
    Func_020068da(2, 0);
    Func_020080a8(0xec);
    Func_02007f06(60);
    Func_020068ee(2, 1);
    Func_020080bc(0xec);
    Func_02007f1a(60);
    Func_0200637c(6);
    Func_02006908(2, 0);
    Func_020080d6(0xec);
    Func_02007f34(60);
    Func_02006396(7);
    Func_02006922(4, 0);
    Func_020080f0(0xed);
    Func_020080cc();
    Func_02007f60();
    Func_02007f16(0x123);
}
