#include "types.h"
#include "scene.h"

struct Entry_08091c7c {
    u8 unknown_00[10];
    u16 value_0a;
    u8 unknown_0c[2];
    u16 value_0e;
};

struct Runtime_08091c7c {
    u8 unknown_000[0x1f4];
    s32 first_1f4;
    struct Entry_08091c7c *second_1f8;
    struct Entry_08091c7c *third_1fc;
    u8 unknown_200[0xac2];
    s16 value_cc2;
    s16 value_cc4;
};

extern struct Runtime_08091c7c *gWork;
extern volatile s32 gIw;

s16 *Sys_Run(s32);
void WaitFrames(s32);
s32 UiWork_IsCompleteFar(void);

void UiWork_FinalizePending(void);

s32 Inventory_PromptAndSetObjectMode(s32 id, s32 force)
{
    struct Runtime_08091c7c *rt = gWork;
    s32 v = *Sys_Run(rt->first_1f4);
    struct Entry_08091c7c *ent0 = rt->second_1f8;
    struct Entry_08091c7c *ent1 = rt->third_1fc;
    s32 flag = 1;
    s32 ret;

    while (gIw != 0)
        WaitFrames(1);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(3);

    if (force == 0) {
        s32 sum = ent0->value_0e + ent0->value_0a;

        if (ent1 != 0) {
            s32 sum2 = ent1->value_0e + ent1->value_0a;
            if (sum < sum2)
                sum = sum2;
        }

        if (sum > 15)
            flag = 0;
    }

    ret = Sys_SetMode(flag, rt->value_cc2, rt->value_cc4, 0);
    if (ret != 0) {
        Object_SetModeById(id, 4);
        Sys_Do(v);
        UiWork_FinalizePending();
        Sys_Apply2(id, 4);
    } else {
        Object_SetModeById(id, 3);
        Sys_Do(v);
        UiWork_FinalizePending();
        Sys_Apply2(id, 3);
    }

    return ret;
}
