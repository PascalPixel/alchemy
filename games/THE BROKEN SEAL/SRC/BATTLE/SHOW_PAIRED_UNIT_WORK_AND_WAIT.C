#include "TYPES.H"

struct Runtime_080931ec {
    u8 unknown_000[0x1d8];
    s16 effect_count;
};

extern struct Runtime_080931ec *Data_03001ebc;
extern volatile u32 Data_03001c94;

s32 ObjectTable_ReadActiveValue(s32);
s32 BattleFx_GetResourceId(u32);
s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWindow_CreateWithSideObjectFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
s32 UiWork_IsIdleFar(s32);
void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32);
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
        BattleFx_GetResourceId(id0) << 16);
    UiWindow_CreateWithSideObjectFar(id0, 0, first_arg, first_extra);

    h1 = UiWork_Create(
        rt->effect_count++, second_x, second_y,
        BattleFx_GetResourceId(id1) << 16);
    UiWindow_CreateWithSideObjectFar(id1, 0, second_arg, second_extra);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(1);
    while ((Data_03001c94 & 0x303) == 0)
        WaitFrames(1);

    WaitFrames(1);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(id0);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(id1);
    UiWork_FinalizePending();
    WaitFrames(1);

    while (UiWork_IsIdleFar(h0) == 0)
        WaitFrames(1);
    while (UiWork_IsIdleFar(h1) == 0)
        WaitFrames(1);

    WaitFrames(1);
}
