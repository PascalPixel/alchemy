#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern s32 Func_02007670(void);
extern void Func_02007684(s32 first, s32 second, s32 *position);
extern s32 Func_02007898(void *object);
extern s32 Func_020078b0(void *object);
extern void Func_020078da(void *object);
extern void Func_02007904(s32 value);

void OverlayObject_UpdateThreeStateMotion(void *obj)
{
    s32 position[3];
    s32 x;
    s32 z;
    u8 *p;
    s32 state;

    p = (u8 *)obj + 0x40;
    state = *(s8 *)p;
    if (state == 0) {
        z = FIELD(obj, s32, 0x18);
        x = FIELD(obj, s32, 0x14);
        FIELD(obj, s32, 8) = z;
        position[2] = z;
        FIELD(obj, s32, 4) = x;
        position[0] = x;
        Func_02007684(0x780000, Func_02007670(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Func_02007904(0x86);
    } else if (state == 1) {
        if (Func_02007898(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (Func_020078b0(obj) == 0)
            Func_020078da(obj);
    }
}
