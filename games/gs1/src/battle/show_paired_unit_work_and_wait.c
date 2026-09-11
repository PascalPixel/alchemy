#include "types.h"
#include "scene.h"
#include "abi/battle/show_paired_unit_work_and_wait.h"

struct Runtime_080931ec {
    u8 unknown_000[0x1d8];
    s16 effect_count;
};

extern struct Runtime_080931ec *gWork;
extern volatile u32 gIw;

s32 ObjectTable_ReadActiveValue(s32);

s32 UiWork_Create(s32, s32, s32, s32);

s32 UiWork_IsCompleteFar(void);

void UiWork_FinalizePending(void);
void WaitFrames(s32);

void Battle_ShowPairedUnitWorkAndWait(
    s32 first, s32 first_x, s32 first_y, s32 first_arg,
    s32 first_extra, s32 second, s32 second_x, s32 second_y,
    s32 second_arg, s32 second_extra)
{
    struct Runtime_080931ec *rt = gWork;
    s32 id0 = ObjectTable_ReadActiveValue(first);
    s32 id1 = ObjectTable_ReadActiveValue(second);
    s32 h0;
    s32 h1;

    h0 = UiWork_Create(
        rt->effect_count++, first_x, first_y,
        Battle_Check(id0) << 16);
    Battle_SetMode(id0, 0, first_arg, first_extra);

    h1 = UiWork_Create(
        rt->effect_count++, second_x, second_y,
        Battle_Check(id1) << 16);
    Battle_SetMode(id1, 0, second_arg, second_extra);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(1);
    while ((gIw & 0x303) == 0)
        WaitFrames(1);

    WaitFrames(1);
    Battle_Do(id0);
    Battle_Do(id1);
    UiWork_FinalizePending();
    WaitFrames(1);

    while (Battle_Check2(h0) == 0)
        WaitFrames(1);
    while (Battle_Check2(h1) == 0)
        WaitFrames(1);

    WaitFrames(1);
}
