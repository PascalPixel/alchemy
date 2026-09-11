#include "types.h"
#include "scene.h"
#include "abi/shop/draw/msg_mode.h"

struct Work_080b2884 {
    u8 padding[0x3AA];
    s8 mode;
};

extern struct Work_080b2884 *gIw;
extern u8 Value_00000d24;
extern u8 Value_00000d2e;
extern u8 Value_00000d38;
extern u8 Value_00000d42;

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gIw->mode;

    if (mode == 1) {
        value += (u32)&Value_00000d2e - (u32)&Value_00000d24;
    }
    if (mode == 2) {
        value += (u32)&Value_00000d38 - (u32)&Value_00000d24;
    }
    if (mode == 3) {
        value += (u32)&Value_00000d42 - (u32)&Value_00000d24;
    }
    return value;
}
