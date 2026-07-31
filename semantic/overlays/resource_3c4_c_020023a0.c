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

void Func_02000f10();
void Func_080091c0();
Slot_020023a0 *Func_0808a080();

void Func_020023a0(void)
{
    s32 column;
    s32 row;

    Func_080091c0(73, 38, 5, 5, 9, 38);
    Func_02000f10(9, 8);

    column = Func_0808a080(8)->column;
    row = Func_0808a080(8)->row >> 20;
    Func_080091c0(2, 36, 1, 1, column >> 20, row);

    column = Func_0808a080(9)->column;
    row = Func_0808a080(9)->row >> 20;
    Func_080091c0(2, 36, 1, 1, column >> 20, row);
}
