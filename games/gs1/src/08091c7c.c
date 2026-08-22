#include "types.h"

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
void Func_080030f8(s32);
s32 Func_08015048(void);
s32 Func_08015390(s32, s32, s32, s32);
void Func_080924d4(s32, s32);
void Func_08015100(s32);
void Func_08015140(void);
void Func_08091c44(s32, s32);

s32 Func_08091c7c(s32 object_id, s32 force)
{
    struct Runtime_08091c7c *runtime = Data_03001ebc;
    s32 value = *Func_0808d394(runtime->first_1f4);
    struct Entry_08091c7c *second = runtime->second_1f8;
    struct Entry_08091c7c *third = runtime->third_1fc;
    s32 allow = 1;
    s32 result;

    while (Data_03001c94 != 0)
        Func_080030f8(1);

    while (Func_08015048() == 0)
        Func_080030f8(1);

    Func_080030f8(3);

    if (force == 0) {
        s32 total = second->value_0e + second->value_0a;

        if (third != 0) {
            s32 other = third->value_0e + third->value_0a;
            if (total < other)
                total = other;
        }

        if (total > 15)
            allow = 0;
    }

    result = Func_08015390(allow, runtime->value_cc2, runtime->value_cc4, 0);
    if (result != 0) {
        Func_080924d4(object_id, 4);
        Func_08015100(value);
        Func_08015140();
        Func_08091c44(object_id, 4);
    } else {
        Func_080924d4(object_id, 3);
        Func_08015100(value);
        Func_08015140();
        Func_08091c44(object_id, 3);
    }

    return result;
}
