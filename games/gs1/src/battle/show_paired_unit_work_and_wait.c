#include "types.h"

struct Runtime_080931ec {
    u8 unknown_000[0x1d8];
    s16 effect_count;
};

extern struct Runtime_080931ec *Data_03001ebc;
extern volatile u32 Data_03001c94;

s32 ObjectTable_ReadActiveValue(s32);
s32 Func_080915ac(u32);
s32 UiWork_Create(s32, s32, s32, s32);
s32 Func_080150f8(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
s32 Func_08015050(s32);
void Func_08015100(s32);
void UiWork_FinalizePending(void);
void WaitFrames(s32);

void Battle_ShowPairedUnitWorkAndWait(
    s32 first, s32 first_x, s32 first_y, s32 first_arg,
    s32 first_extra, s32 second, s32 second_x, s32 second_y,
    s32 second_arg, s32 second_extra)
{
    struct Runtime_080931ec *rt = Data_03001ebc;
    s32 id0 = ObjectTable_ReadActiveValue(first);
    s32 id1 = ObjectTable_ReadActiveValue(second);
    s32 h0;
    s32 h1;

    h0 = UiWork_Create(
        rt->effect_count++, first_x, first_y,
        Func_080915ac(id0) << 16);
    Func_080150f8(id0, 0, first_arg, first_extra);

    h1 = UiWork_Create(
        rt->effect_count++, second_x, second_y,
        Func_080915ac(id1) << 16);
    Func_080150f8(id1, 0, second_arg, second_extra);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(1);
    while ((Data_03001c94 & 0x303) == 0)
        WaitFrames(1);

    WaitFrames(1);
    Func_08015100(id0);
    Func_08015100(id1);
    UiWork_FinalizePending();
    WaitFrames(1);

    while (Func_08015050(h0) == 0)
        WaitFrames(1);
    while (Func_08015050(h1) == 0)
        WaitFrames(1);

    WaitFrames(1);
}
