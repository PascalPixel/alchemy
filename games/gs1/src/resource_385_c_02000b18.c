#include "types.h"

/* Complete scene-variant seven actor-zero setup wrapper through its pool. */
extern const u8 Data_0200976c[];




extern void Func_02001cce(s32 value);
extern void Func_02001bb0(const void *descriptor, s32 value, s32 actor);
extern void Func_020015b4(s32 x, s32 y, s32 value);
void Func_02000b18(void)
{
    Func_02001cce(158);
    Func_02001bb0(Data_0200976c, 44, 17);
    Func_020015b4(216, 288, 7);
}
