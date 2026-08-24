#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void WaitFrames(s32);

void Func_0800c4ec(void *object) {
    s32 count;

    count = 0;
    if (*(s32 *)((FIELD(object, s16 *, 4) * 4) + FIELD(object, s32 *, 0)) != 0x10) {
        do {
            WaitFrames(1);
            count++;
            if (count > 0x12B) {
                break;
            }
        } while (*(s32 *)((FIELD(object, s16 *, 4) * 4) + FIELD(object, s32 *, 0)) != 0x10);
    }
}
