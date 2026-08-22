#include "types.h"
extern u8 *Data_03001ebc;







/* Complete 76-byte shared numbered-scene owner through its two-word pool. */
extern s32 Func_02001448(s32 flagId);
extern void Func_02001588(void);
extern void Func_020015c6(s32 soundId);
extern void Func_020015c8(void);
extern void Func_020015d4(void);
extern void Func_020015a2(s32 value);
void Func_020002bc(s32 value)
{
    if (Func_02001448(0x834) != 0)
        Func_02001588();
    Func_020015c6(123);
    *(s32 *)(Data_03001ebc + 448) = 521;
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_020015c8();
    Func_020015d4();
    Func_020015a2(value);
}
