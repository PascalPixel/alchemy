#include "types.h"

struct Runtime_080931ec {
    u8 unknown_000[0x1d8];
    s16 effect_count;
};

extern struct Runtime_080931ec *Data_03001ebc;
extern volatile u32 Data_03001c94;

s32 Func_08092ba8(s32);
s32 Func_080915ac(u32);
s32 Func_08015038(s32, s32, s32, s32);
s32 Func_080150f8(s32, s32, s32, s32);
s32 Func_08015048(void);
s32 Func_08015050(s32);
void Func_08015100(s32);
void Func_08015140(void);
void Func_080030f8(s32);

void Func_080931ec(
    s32 first, s32 first_x, s32 first_y, s32 first_arg,
    s32 first_extra, s32 second, s32 second_x, s32 second_y,
    s32 second_arg, s32 second_extra)
{
    struct Runtime_080931ec *runtime = Data_03001ebc;
    s32 first_id = Func_08092ba8(first);
    s32 second_id = Func_08092ba8(second);
    s32 first_handle;
    s32 second_handle;

    first_handle = Func_08015038(
        runtime->effect_count++, first_x, first_y,
        Func_080915ac(first_id) << 16);
    Func_080150f8(first_id, 0, first_arg, first_extra);

    second_handle = Func_08015038(
        runtime->effect_count++, second_x, second_y,
        Func_080915ac(second_id) << 16);
    Func_080150f8(second_id, 0, second_arg, second_extra);

    while (Func_08015048() == 0)
        Func_080030f8(1);

    Func_080030f8(1);
    while ((Data_03001c94 & 0x303) == 0)
        Func_080030f8(1);

    Func_080030f8(1);
    Func_08015100(first_id);
    Func_08015100(second_id);
    Func_08015140();
    Func_080030f8(1);

    while (Func_08015050(first_handle) == 0)
        Func_080030f8(1);
    while (Func_08015050(second_handle) == 0)
        Func_080030f8(1);

    Func_080030f8(1);
}
