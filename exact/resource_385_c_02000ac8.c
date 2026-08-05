#include "types.h"

/* Complete scene-variant five actor-zero setup wrapper through its pool. */
extern const u8 Data_02009740[];




extern void Func_02001c7e(s32 value);
extern void Func_02001b60(const void *descriptor, s32 value, s32 actor);
extern void Func_02001566(s32 x, s32 y, s32 value);
void Func_02000ac8(void)
{
    Func_02001c7e(158);
    Func_02001b60(Data_02009740, 56, 19);
    Func_02001566(408, 320, 5);
}
