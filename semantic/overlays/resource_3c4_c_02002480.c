/*
 * resource_3c4 @ 0x02002480 (170 bytes).
 *
 * Two six-argument placements (r5 carries the constant 25 into [sp,#0] for
 * both), a byte poke that sets +34 of slot 14's descriptor to 1, and then
 * three identical grid pins for slots 12, 13 and 14: each slot's +8 and +16
 * words are shifted right by 20 (signed `asrs`) and passed as the two stack
 * arguments of that slot's placement service.
 *
 * Every slot is fetched twice through two different accessors; the reference
 * never reuses the first pointer, so the pairs are kept separate here.
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Slot_02002480 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02002480;

void Func_02005532(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005544(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u8 *Func_020055ba(s32 slot);
Slot_02002480 *Func_020055c6(s32 slot);
Slot_02002480 *Func_020055ce(s32 slot);
void Func_02005574(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
Slot_02002480 *Func_020055ea(s32 slot);
Slot_02002480 *Func_020055f2(s32 slot);
void Func_02005598(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
Slot_02002480 *Func_0200560e(s32 slot);
Slot_02002480 *Func_02005616(s32 slot);
void Func_020055bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02002480(void)
{
    u8 *descriptor;
    s32 column;
    s32 row;

    Func_02005532(89, 49, 3, 2, 25, 49);
    Func_02005544(89, 51, 8, 5, 25, 51);

    descriptor = Func_020055ba(14);
    descriptor[34] = 1;

    column = Func_020055c6(12)->column;
    row = Func_020055ce(12)->row >> 20;
    Func_02005574(22, 52, 1, 1, column >> 20, row);

    column = Func_020055ea(13)->column;
    row = Func_020055f2(13)->row >> 20;
    Func_02005598(22, 52, 1, 1, column >> 20, row);

    column = Func_0200560e(14)->column;
    row = Func_02005616(14)->row >> 20;
    Func_020055bc(22, 52, 1, 1, column >> 20, row);
}
