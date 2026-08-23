#include "types.h"

/*
 * resource_3bb scripted transition owner at 0x020029d0, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */

extern void Func_02006912(void);              /* Func_0808a018 veneer #1 (mode==0 arm) */
extern void Func_02006a3e(void);              /* Func_0808a360 veneer #1 */
extern void Func_02006a52(void);              /* Func_0808a370 veneer #1 */
extern void Func_02006918(s32 frames);        /* Func_0808a010 veneer #1 */
extern void Func_02006a9e(s32 cue);           /* Audio_PlayCue veneer #1 */
extern void Func_02004db0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200533c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02006932(s32 frames);        /* Func_0808a010 veneer #2 */
extern void Func_02006946(void);              /* Func_0808a020 veneer #1 */

extern void Func_02006abe(s32 cue);           /* Audio_PlayCue veneer #2 (main arm) */
extern void Func_0200694a(void);              /* Func_0808a018 veneer #2 */
extern void Func_02006a76(void);              /* Func_0808a360 veneer #2 */
extern void Func_02006a8a(void);              /* Func_0808a370 veneer #2 */
extern void Func_0200695a(s32 frames);        /* Func_0808a010 veneer #3 */
extern void Func_02006ae2(s32 cue);           /* Audio_PlayCue veneer #3 */
extern void Func_02004df4(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_02005380(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern void Func_02006976(s32 frames);        /* Func_0808a010 veneer #4 */

extern void Func_020067ee(s32 frames);        /* Func_080000c0 veneer (loop body) */
extern s32 Func_02006b0a(void);               /* Func_080f9048 veneer (loop check) */

extern void Func_02006b0c(s32 cue);           /* Audio_PlayCue veneer #4 */
extern void Func_02004e1e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020053aa(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_02006b20(s32 cue);           /* Audio_PlayCue veneer #5 */
extern void Func_020069a6(s32 frames);        /* Func_0808a010 veneer #5 */
extern void Func_020053be(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_02006b34(s32 cue);           /* Audio_PlayCue veneer #6 */
extern void Func_020069ba(s32 frames);        /* Func_0808a010 veneer #6 */
extern void Func_02004e4c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_020053d8(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_02006b4e(s32 cue);           /* Audio_PlayCue veneer #7 */
extern void Func_020069d4(s32 frames);        /* Func_0808a010 veneer #7 */
extern void Func_02004e66(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_020053f2(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_02006b68(s32 cue);           /* Audio_PlayCue veneer #8 */
extern void Func_02006b44(void);              /* Func_0808a4f0 veneer */
extern void Func_02006a00(void);              /* Func_0808a020 veneer #2 */
extern void Func_020069b6(s32 flag);          /* Func_080770c8 veneer */

void Func_020029d0(s32 mode)
{
    if (mode == 0) {
        Func_02006912();
        Func_02006a3e();
        Func_02006a52();
        Func_02006918(30);
        Func_02006a9e(0x59);
        Func_02004db0(0);
        Func_0200533c(1, 0);
        Func_02006932(120);
        Func_02006946();
        return;
    }

    Func_02006abe(0xf7);
    Func_0200694a();
    Func_02006a76();
    Func_02006a8a();
    {
        s16 *base = (s16 *)0x0200ca1e;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Func_0200695a(30);
    Func_02006ae2(mode + 0x5a);
    Func_02004df4(mode);
    Func_02005380(1, 0);
    Func_02006976(120);

    goto check_transition;
wait_transition:
    Func_020067ee(1);
check_transition:
    if (Func_02006b0a() != 0)
        goto wait_transition;

    Func_02006b0c(0x121);
    Func_02004e1e(5);
    Func_020053aa(2, 0);
    Func_02006b20(0xec);
    Func_020069a6(60);
    Func_020053be(2, 1);
    Func_02006b34(0xec);
    Func_020069ba(60);
    Func_02004e4c(6);
    Func_020053d8(2, 0);
    Func_02006b4e(0xec);
    Func_020069d4(60);
    Func_02004e66(7);
    Func_020053f2(4, 0);
    Func_02006b68(0xed);
    Func_02006b44();
    Func_02006a00();
    Func_020069b6(0x123);
}
