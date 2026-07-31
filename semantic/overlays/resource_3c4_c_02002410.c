/*
 * resource_3c4 @ 0x02002410 (112 bytes).
 *
 * Sibling of 0x020023a0 for slots 10 and 11 with its own service set.
 * Same shape: one six-argument placement, then two grid-cell pins built from
 * each slot's +8 and +16 words shifted right by 20 (signed).
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

typedef struct Slot_02002410 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002410;

void Func_02000f10();
void Func_080091c0();
Slot_02002410 *Func_0808a080();

void Func_02002410(void)
{
    s32 column;
    s32 row;

    Func_080091c0(93, 30, 6, 5, 29, 30);
    Func_02000f10(11, 10);

    column = Func_0808a080(10)->column;
    row = Func_0808a080(10)->row >> 20;
    Func_080091c0(2, 36, 1, 1, column >> 20, row);

    column = Func_0808a080(11)->column;
    row = Func_0808a080(11)->row >> 20;
    Func_080091c0(2, 36, 1, 1, column >> 20, row);
}
