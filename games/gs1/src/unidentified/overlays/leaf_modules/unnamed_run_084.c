/* Contiguous unnamed leaf-owner run for resource_3b8. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200ca1c[];
extern u8 Data_0200c614[];

s32 Func_02000030(void) {
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}

#include "types.h"

extern u8 Data_0200ca7c[];

s32 Func_02000060(void)
{
    return (s32)Data_0200ca7c;
}

#include "types.h"

extern u8 Data_0200ca8c[];

s32 Func_02000068(void)
{
    return (s32)Data_0200ca8c;
}

