/*
 * resource_3c4 @ 0x020023a0 (112 bytes).
 *
 * One six-argument placement, then two identical follow-ups that pin an
 * overlay at the 12.20 grid cell of slots 8 and 9: the slot's +8 and +16 words
 * are each shifted right by 20 (signed `asrs`) and passed as the two stack
 * arguments of the placement call.
 *
 * Each slot is fetched twice through two different accessors; the reference
 * does not reuse the first pointer, so the calls are kept separate here.
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

void Func_02005452(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020032ce(s32 a, s32 b);
Slot_020023a0 *Func_020054d0(s32 slot);
Slot_020023a0 *Func_020054d8(s32 slot);
void Func_0200547e(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
Slot_020023a0 *Func_020054f4(s32 slot);
Slot_020023a0 *Func_020054fc(s32 slot);
void Func_020054a2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_020023a0(void)
{
    s32 column;
    s32 row;

    Func_02005452(73, 38, 5, 5, 9, 38);
    Func_020032ce(9, 8);

    column = Func_020054d0(8)->column;
    row = Func_020054d8(8)->row >> 20;
    Func_0200547e(2, 36, 1, 1, column >> 20, row);

    column = Func_020054f4(9)->column;
    row = Func_020054fc(9)->row >> 20;
    Func_020054a2(2, 36, 1, 1, column >> 20, row);
}
