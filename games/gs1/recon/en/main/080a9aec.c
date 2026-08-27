#include "types.h"

extern u8 Value_00000182;

void *Func_08077018(s32 item_id);
void Func_08015080(s32 message, s32 window, s32 x, s32 style);

void Func_080a9aec(s32 window, const u16 *flags)
{
    s32 i;
    const u16 *entry = flags;

    for (i = 14; i >= 0; i--) {
        u16 value = *entry;
        entry++;
        if (value & 0x200) {
            s32 item_id = value & 0x1ff;
            u8 *info = Func_08077018(item_id);
            switch (info[2]) {
            case 2:
                Func_08015080(item_id + (s32)&Value_00000182, window, 8, 56);
                break;
            case 3:
                Func_08015080(item_id + (s32)&Value_00000182, window, 8, 40);
                break;
            case 4:
                Func_08015080(item_id + (s32)&Value_00000182, window, 8, 24);
                break;
            case 1:
                Func_08015080(item_id + (s32)&Value_00000182, window, 8, 8);
                break;
            default:
                break;
            }
        }
    }
}
