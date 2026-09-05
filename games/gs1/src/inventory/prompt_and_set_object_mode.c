#include "types.h"

#define Inventory_PromptAndSetObjectMode Func_08091c7c

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

extern struct Runtime_08091c7c *Data_03001ebc;
extern volatile s32 Data_03001c94;

s16 *Func_0808d394(s32);
void WaitFrames(s32);
s32 UiWork_IsCompleteFar(void);
s32 Func_08015390(s32, s32, s32, s32);
void Func_080924d4(s32, s32);
void Func_08015100(s32);
void UiWork_FinalizePending(void);
void Func_08091c44(s32, s32);

s32 Inventory_PromptAndSetObjectMode(s32 id, s32 force)
{
    struct Runtime_08091c7c *rt = Data_03001ebc;
    s32 v = *Func_0808d394(rt->first_1f4);
    struct Entry_08091c7c *ent0 = rt->second_1f8;
    struct Entry_08091c7c *ent1 = rt->third_1fc;
    s32 flag = 1;
    s32 ret;

    while (Data_03001c94 != 0)
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

    ret = Func_08015390(flag, rt->value_cc2, rt->value_cc4, 0);
    if (ret != 0) {
        Func_080924d4(id, 4);
        Func_08015100(v);
        UiWork_FinalizePending();
        Func_08091c44(id, 4);
    } else {
        Func_080924d4(id, 3);
        Func_08015100(v);
        UiWork_FinalizePending();
        Func_08091c44(id, 3);
    }

    return ret;
}
