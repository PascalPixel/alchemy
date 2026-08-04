#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000002;

s32 Func_020017e0();

s32 Func_02000384(void)
{
    s32 mode;
    s32 normalized;

    /* The predicate is meaningful only after flag 324 is set. */
    if (Func_020017e0(324) == 0) {
        return 0;
    }
    if (Data_02000240[287] == 2) {
        return 0;
    }

    /* Return 0 for mode 2 and -1 for every other mode. */
    mode = Data_02000240[224] ^ (s32)&Value_00000002;
    normalized = (unsigned int)(mode | -mode) >> 31;
    return -normalized;
}
