#include "types.h"

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(u16, s32);

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

u32 Func_08005868(u16 code)
{
    s32 *param = (s32 *)0x02004C04;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)0x03001F1C;
    value = code & 0xFFFF;
    if ((Func_080072f0(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Func_08006c68(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}
