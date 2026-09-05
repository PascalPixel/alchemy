#include "types.h"

#define UiWork_WaitUntilField1aClear Func_080163ec

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);

void UiWork_WaitUntilField1aClear(void *work) {
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & FIELD_AT_OFFSET(work, u16 *, 0x16)) && (FIELD_AT_OFFSET(work, s16 *, 0x1A) != 0)) {
        do {
            WaitFrames(1);
        } while (FIELD_AT_OFFSET(work, s16 *, 0x1A) != 0);
    }
}
