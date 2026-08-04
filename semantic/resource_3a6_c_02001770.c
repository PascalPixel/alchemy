#include "types.h"

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */

extern void Func_02001938(s32 width, s32 top, s32 bottom);
extern void Func_080091c0(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern s32 Func_080770c0(s32 flagId);

void Func_02001770(void)
{
    Func_080091c0(0, 34, 13, 3, 23, 34);

    if (Func_080770c0(0x301) != 0) {
        Func_02001938(11, 35, 35);
        Func_080091c0(24, 34, 1, 3, 23, 34);
    } else {
        Func_02001938(11, 23, 35);
        Func_080091c0(24, 34, 1, 3, 35, 34);
    }
}
