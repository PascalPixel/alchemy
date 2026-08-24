#include "types.h"

s32 UiWork_IsIdle(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x16) == 0) {
        if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
            return 1;
        }
    }
    return 0;
}
