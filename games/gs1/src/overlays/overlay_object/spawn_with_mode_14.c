#include "types.h"

#define OverlayObject_SpawnWithMode14 Func_02000058

void Func_020000b2(void *, s32);
void *Func_02004e4e(s32, s32, s32, s32);
void Func_02004ed6(void *, s32);

void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *obj;
    u8 *p;
    s32 mask;
    u8 flag;

    obj = Func_02004e4e(arg3, arg0, arg1, arg2);
    if (obj != 0) {
        p = *(u8 **)((u8 *)obj + 0x50);
        mask = 13;
        flag = p[9];
        mask = -mask;
        mask &= flag;
        p[9] = mask;
        Func_020000b2(obj, 0xE);
        Func_02004ed6(obj, 1);
        return obj;
    }
    return 0;
}
