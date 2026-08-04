typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02000160, 248 bytes (0x02000160-0x02000257):
 * 230 bytes of code, two alignment bytes at 0x02000246, and the four-word
 * literal pool at 0x02000248.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} / sub sp, #8` at
 * 0x02000160; epilogue `add sp, #8 / pop {r3} / mov r8, r3 /
 * pop {r5, r6, r7} / pop {r0} / bx r0`: r0 holds the popped return address, so
 * the owner is void.  r8 caches the participant record for the whole body and
 * r6 the sub-object at its +80; the eight bytes of outgoing stack carry the
 * fifth and sixth arguments of the two Func_080091c0 calls.
 *
 * All 13 `bl` sites are placed and reach eight distinct callees, matching the
 * inventory row's calls=13.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations - it prints the two identical Func_080000c0 halfwords, and the
 * two identical Func_08009150 halfwords, as different callees.  0x3f50 ->
 * Func_0808a080, 0x3ef0 -> Func_080770c8, 0x40b0 -> Func_080f9010 (twice),
 * 0x3e78 -> Func_08009150 (twice), 0x3da0 -> Func_080000c0 (twice), 0x3e80 ->
 * Func_08009158, 0x3f30 -> Func_0808a010, 0x3f98 -> Func_0808a100, 0x3e88 ->
 * Func_080091c0 (twice).
 *
 * Shape: participant 30 is moved to a first position, then a ten-frame scroll
 * decrements the halfword at +30 of its sub-object by a step that grows by 36
 * each frame; it is moved again and a twenty-two-frame scroll repeats the same
 * pattern starting from 360.  The scroll halfword is then zeroed, the
 * participant is reposed and its record reset, and two lines are drawn.
 *
 * The step schedule is literal: `r7` starts at 0 (then 360), is subtracted
 * from the halfword, and only afterwards advanced by 36, so the first pass of
 * each loop subtracts the initial value.  Both loops are counted with a
 * signed `bge` on a counter that starts at 9 and 21, giving ten and
 * twenty-two passes.
 *
 * Uncertainties: only the record fields at +8, +12, +16, +36, +40, +48, +52
 * and +80 are asserted, together with the halfword at +30 of the sub-object.
 * The constants 0xfff00000 and 0xfff80000 are pool words used directly as the
 * y coordinate of a move and as a record field; they are reproduced as
 * written rather than reinterpreted.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
void Func_080770c8();
void Func_080f9010();
void Func_08009150();
void Func_080000c0();
void Func_08009158();
void Func_0808a010();
void Func_0808a100();
void Func_080091c0();

void Func_02000160(void)
{
    u8 *record;
    u8 *scroll;
    s32 step;
    s32 frames;

    record = Func_0808a080(30);
    scroll = *(u8 **)(record + 80);

    Func_080770c8(816);                         /* 204 << 2 */

    *(s32 *)(record + 52) = 0x1999;
    *(s32 *)(record + 48) = 0x13333;

    Func_080f9010(227);
    Func_08009150(record, 0x2d00000, 0xa0000, 0x2280000);   /* 168<<17, 160<<12, 132<<17 */

    step = 0;
    frames = 9;
    do {
        *(u16 *)(scroll + 30) = (u16)(*(u16 *)(scroll + 30) - step);
        frames = frames - 1;
        Func_080000c0(1);
        step = step + 36;
    } while (frames >= 0);

    Func_08009150(record, 0x2a00000, 0xfff00000, 0x2280000); /* 165 << 17 */

    step = 360;                                 /* 180 << 1 */
    frames = 21;
    do {
        *(u16 *)(scroll + 30) = (u16)(*(u16 *)(scroll + 30) - step);
        frames = frames - 1;
        Func_080000c0(1);
        step = step + 36;
    } while (frames >= 0);

    Func_08009158(record);
    Func_0808a010(2);
    Func_080f9010(240);

    *(u16 *)(scroll + 30) = 0;
    Func_0808a100(30, 4);

    *(s32 *)(record + 8) = 0x2d00000;
    *(s32 *)(record + 12) = 0xfff80000;
    *(s32 *)(record + 16) = 0x2280000;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 36) = 0;

    Func_080091c0(19, 16, 1, 1, 20, 16);
    Func_080091c0(20, 80, 1, 1, 21, 80);
}
