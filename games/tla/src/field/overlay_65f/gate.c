#include "ov65f.h"

struct EventWork {
    u8 unknown_000[0x154];
    s16 unknown_154;
    u8 unknown_156[0x56];
    s32 unknown_1ac;
};

extern s32 Data_0200632c;

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_020052c4(s32 a);
void Func_02002694(void);
void Func_02003878(void);

static inline void CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters build each -1 on its own; a direct call copies one register */
    Motion_CamBounds(x, y, z, mode);
}

void Func_02000960(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    Func_02005394(123);
    ((struct EventWork *)Ram_HeapSlots->event_work)->unknown_1ac = 521;
    CamMove(-1, -1, -1, 0);
    Flag_Set(0x121);
    Func_020052c4(event->unknown_154);
}

void Func_020009a4(void)
{
    if (!Flag_IsSet(0x91b)) {
        if (!Flag_IsSet(0x91e))
            Func_02002694();
    } else {
        if (!Flag_IsSet(0x91f))
            Func_02003878();
    }
}
