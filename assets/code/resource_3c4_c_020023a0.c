/*
 * resource_3c4 @ 0x020023a0 (112 bytes).
 *
 * One six-argument placement, then two identical follow-ups that pin an
 * overlay at the 12.20 grid cell of slots 8 and 9: the slot's +8 and +16
 * words are each shifted right by 20 (signed `asrs`) and passed as the two
 * stack arguments of the placement call.
 *
 * Exact-lane transcription: each `bl` site keeps its own raw overlay_show
 * target name (HANDOVER section 2), even though overlay_call_targets.ts
 * resolves all eight sites to the same three real veneers
 * (Func_080091c0 x3, Func_0808a080 x4, Func_02000f10 x1) -- the raw address
 * IS the assembler's encoding, verbatim.
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

void Func_02005452();
void Func_020032ce();
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
void Func_0200547e();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();
void Func_020054a2();

void Func_020023a0(void)
{
    s32 row;

    {
        s32 k5 = 9, k6 = 38;
        Func_02005452(73, 38, 5, 5, k5, k6);
    }
    Func_020032ce(9, 8);

    {
        s32 col20 = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Func_0200547e(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Func_020054a2(2, 36, 1, 1, col20, row);
    }
}
