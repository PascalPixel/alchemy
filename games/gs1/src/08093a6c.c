#include "types.h"

extern s16 Data_02000240[];
extern s32 Object_GetById(u32);
extern void Object_SetCallback(void *, void *);
extern const u8 Data_0809fe00[];
extern const u8 Data_0809fd44[];
extern const u8 Data_0809fe10[];
extern const u8 Data_0809fecc[];
extern const u8 Data_0809ff18[];
extern const u8 Data_0809ff2c[];
extern const u8 Data_0809fe04[];

void Func_08093a6c(s8 *object, s32 kind)
{
    s32 index;
    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)Data_0809fe00;
        break;
    case 1:
        kind = (s32)Data_0809fd44;
        break;
    case 2:
        kind = (s32)Data_0809fe10;
        break;
    case 3:
        kind = (s32)Data_0809fecc;
        break;
    case 4:
        kind = (s32)Data_0809ff18;
        break;
    case 5:
        index = 250;
        *(s32 *)(object + 0x68) = Object_GetById(*(u32 *)&Data_02000240[index]);
        kind = (s32)Data_0809ff2c;
        break;
    case 6:
        kind = (s32)Data_0809fe04;
        break;
    default:
        break;
    }
    Object_SetCallback(object, (void *)kind);
}
