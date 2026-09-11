#include "types.h"
#include "scene.h"

/* shop/price/service.c */
u8 *Runtime_GetObject(void);

s32 Shop_ServicePrice(s32 unused, s32 kind)
{
    u8 value = Runtime_GetObject()[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

/* shop/unit/can_serve.c */
u8 *Runtime_GetObject(s32);

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Runtime_GetObject(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}

/* shop/unit/count.c */
extern u8 *gIw;

s32 Shop_CountUnits(void)
{
    u8 *work = gIw;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Sys_Apply(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}

/* shop/draw/msg_mode.c */
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
