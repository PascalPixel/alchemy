#include "types.h"





extern s32 Func_020030b0(s32 value);
extern s32 Func_020030b8(s32 group, s32 value);
extern u8 * Func_020030bc(s32 group);
void Func_0200172c(s32 value, u16 setting)
{
    s32 group = Func_020030b0(value);

    if (group != -1) {
        s32 slot = Func_020030b8(group, value);

        if (slot != -1) {
            u8 *record = Func_020030bc(group);
            *(u16 *)(record + 216 + slot * 2) = setting;
        }
    }
}
