#include "types.h"

void Func_080030f8(s32);
extern u8 Value_00000043;
extern u8 Value_00000045;
extern u8 Value_00000058;

s32 Func_080b60a0(void)
{
    u8 *state;
    u16 *other;
    u16 *request;
    s32 counter;
    s32 tries;
    s32 selector;
    s32 selected;
    s16 value;
    s16 value2;

    state = *(u8 **)0x03001e74;
    counter = 0;
    if (state[0x44] != 0) {
        selected = state[0x50];
        selector = 1;
        selector ^= selected;
        other = (u16 *)(0x02002024 + selector * 0x18);
        request = (u16 *)0x02002224;
        if (state[0x52] == 0) {
            value = (s32)&Value_00000045;
            value2 = (s32)&Value_00000058;
            request[0] = value;
            request[2] = value;
            value = (s32)&Value_00000043;
            request[1] = value2;
            request[3] = value;
            tries = 0;
loop:
            if ((*(u16 *)0x03001f64 & 3) != 3) {
                counter++;
                if (counter > 0x18) {
                    goto fail;
                }
                goto wait;
            }
            counter = 0;
            if (request[2] == other[2] && request[3] == other[3]) {
                goto success;
            }
wait:
            Func_080030f8(1);
            tries++;
            if (tries <= 0x1d) {
                goto loop;
            }
        }
fail:
        return -1;
    }
success:
    return 0;
}
