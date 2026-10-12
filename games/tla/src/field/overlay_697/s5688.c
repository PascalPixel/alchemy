#include "ov697.h"

struct Ov697Control {
    s16 state;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s16 field_08;
    s16 field_0a;
};

extern u8 gSceneState[];

void Func_02004ddc(void);

void Func_02005688(s32 a)
{
    u8 *work = Ram_HeapSlots->unknown_ec[0];
    struct Ov697Control *control = (struct Ov697Control *)gSceneState;

    if (GameFlag_Test(0x109) == 0) {
        /* FAKEMATCH: a temporary reads the work value before the three stores (differ 4 without it). */
        s32 value = *(u16 *)(work + 228);

        control->field_02 = a;
        control->field_04 = value;
        control->field_08 = 0;
    }
    {
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load (differ 2 without it). */
        s32 priority = 0x485;

        Scheduler_AddOrUpdateCallback(Func_02004ddc, priority);
    }
}

void Func_020056c4(s32 a)
{
    u8 *work = Ram_HeapSlots->unknown_ec[0];
    struct Ov697Control *control = (struct Ov697Control *)gSceneState;
    /* FAKEMATCH: a word temporary builds 0 with movs for both halfword stores, where a halfword constant loads from the pool. */
    s32 zero = 0;

    *(u16 *)(work + 234) = zero;
    control->field_0a = a;
    control->field_06 = zero;
    control->state = 1;
    Func_02004ddc();
}

s32 Func_020056e8(void)
{
    s32 state = *(s16 *)gSceneState;

    return IsZero(state ^ 9);
}

void Func_02005704(void)
{
}
