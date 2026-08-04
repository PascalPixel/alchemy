#include "types.h"

void Func_08015090(s32 image, s32 layer, s32 x, s32 y);
void Func_08015098(s32 image, s32 layer, s32 x, s32 y);
void Func_080a14f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Func_080150b8(s32 color);

void Func_080a153c(void *object, s32 layer)
{
    s16 value;

    Func_08015090(0x080af210, layer, 0, 40);
    Func_08015098(0x080af214, layer, 48, 40);
    value = *(s16 *)((u8 *)object + 52);
    Func_080a14f0(value, layer, 88, 40);
    value = *(s16 *)((u8 *)object + 56);
    if (value < ((s32)(u16)*(s16 *)((u8 *)object + 52) << 16) >> 18) {
        Func_080150b8(4);
    }
    if (value == 0) {
        Func_080150b8(2);
    }
    Func_080a14f0(value, layer, 48, 40);
    Func_080150b8(15);
    Func_08015090(0x080af218, layer, 0, 48);
    Func_08015098(0x080af214, layer, 48, 48);
    value = *(s16 *)((u8 *)object + 58);
    Func_080a14f0(value, layer, 48, 48);
    value = *(s16 *)((u8 *)object + 54);
    Func_080a14f0(value, layer, 88, 48);
}
