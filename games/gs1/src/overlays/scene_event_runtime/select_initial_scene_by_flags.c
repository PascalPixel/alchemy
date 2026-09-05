#include "types.h"

#define SceneEventRuntime_SelectInitialSceneByFlags Func_02000054
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0200248e(s32);
s32 Func_02002498(s32);
s32 Func_020024a6(s32);
void Func_020025a8(s32);

s32 SceneEventRuntime_SelectInitialSceneByFlags(void) {
    s32 no;

    if (Func_0200248e(0x818) != 0) {
        if (Func_02002498(0x813) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (Func_020024a6(0x812) == 0) {
        no = 4;
apply:
        Func_020025a8(no);
        return 1;
    }
fail:
    return -1;
}
