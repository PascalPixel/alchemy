#include "types.h"

#define OverlayObject_Add160ToFields18And1c Func_020000a4

void OverlayObject_Add160ToFields18And1c(u8 *o) {
    if (*(s32 *)(o + 24) < 0x10000) {
        *(s32 *)(o + 24) += 160;
        *(s32 *)(o + 28) += 160;
    }
}
