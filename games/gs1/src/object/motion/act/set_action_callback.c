#include "types.h"
#include "scene.h"
#include "abi/object/motion/act/set_action_callback.h"

extern s16 gCell[];
extern s32 Object_GetById(u32);
extern void Object_SetCallback(void *, void *);
extern const u8 gRom[];
extern const u8 gRom2[];
extern const u8 gRom3[];
extern const u8 gRom4[];
extern const u8 gRom5[];
extern const u8 gRom6[];
extern const u8 gRom7[];

void Motion_SetActionCallback(s8 *object, s32 kind)
{
    s32 index;
    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)gRom;
        break;
    case 1:
        kind = (s32)gRom2;
        break;
    case 2:
        kind = (s32)gRom3;
        break;
    case 3:
        kind = (s32)gRom4;
        break;
    case 4:
        kind = (s32)gRom5;
        break;
    case 5:
        index = 250;
        *(s32 *)(object + 0x68) = Object_GetById(*(u32 *)&gCell[index]);
        kind = (s32)gRom6;
        break;
    case 6:
        kind = (s32)gRom7;
        break;
    default:
        break;
    }
    Object_SetCallback(object, (void *)kind);
}
