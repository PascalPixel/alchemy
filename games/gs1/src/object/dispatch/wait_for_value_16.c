#include "types.h"

#define ObjectDispatch_WaitForValue16 Func_0800c4ec

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void WaitFrames(s32);

void ObjectDispatch_WaitForValue16(void *obj) {
    s32 cnt;

    cnt = 0;
    if (*(s32 *)((FIELD(obj, s16 *, 4) * 4) + FIELD(obj, s32 *, 0)) != 0x10) {
        do {
            WaitFrames(1);
            cnt++;
            if (cnt > 0x12B) {
                break;
            }
        } while (*(s32 *)((FIELD(obj, s16 *, 4) * 4) + FIELD(obj, s32 *, 0)) != 0x10);
    }
}
