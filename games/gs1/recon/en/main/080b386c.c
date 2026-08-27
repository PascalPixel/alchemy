#include "types.h"

extern u8 Value_00000182;
extern u8 Value_00000c94;
extern u8 Value_00000c95;
extern u8 Value_00000c8d;

void *Func_08077008(s32);
void Func_08015270(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_080772a8(s32, s32);
s32 Func_080b19cc(s32);
void Func_080150b0(s32, s32, s32, s32, s32);

void Func_080b386c(s32 window, s32 unit_id, s32 item_id)
{
    u8 *unit = Func_08077008(unit_id);
    s32 slot_offset = item_id * 2 + 216;
    s32 masked = *(u16 *)(unit + slot_offset) & 0x1ff;
    s32 mult = (*(u16 *)(unit + slot_offset) >> 11) + 1;

    if (window != 0) {
        s32 result;

        Func_08015270(window);
        Func_08015080(masked + (s32)&Value_00000182, window, 0, 0);

        result = Func_080772a8(unit_id, item_id);
        if (result == -4) {
            Func_08015080((s32)&Value_00000c94, window, 0, 8);
        } else if (result == -3) {
            Func_08015080((s32)&Value_00000c95, window, 0, 8);
        } else {
            s32 qty;
            s32 total;

            qty = Func_080b19cc(*(u16 *)(unit + slot_offset));
            total = mult * qty;

            Func_08015080((s32)&Value_00000c8d, window, 8, 8);
            Func_080150b0(total, 5, window, 40, 8);
            Func_08015080((s32)&Value_00000c8d - 5, window, 80, 8);
        }
    }
}
