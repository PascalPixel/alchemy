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

void Func_020054c2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200333e(s32 a, s32 b);
Slot_02002410 *Func_02005540(s32 slot);
Slot_02002410 *Func_02005548(s32 slot);
void Func_020054ee(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
Slot_02002410 *Func_02005564(s32 slot);
Slot_02002410 *Func_0200556c(s32 slot);
void Func_02005512(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02002410(void)
{
    s32 column;
    s32 row;

    Func_020054c2(93, 30, 6, 5, 29, 30);
    Func_0200333e(11, 10);

    column = Func_02005540(10)->column;
    row = Func_02005548(10)->row >> 20;
    Func_020054ee(2, 36, 1, 1, column >> 20, row);

    column = Func_02005564(11)->column;
    row = Func_0200556c(11)->row >> 20;
    Func_02005512(2, 36, 1, 1, column >> 20, row);
}
