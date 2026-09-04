#include "types.h"

#define OverlayObject_DecayFields24And28 Func_020008c0

void OverlayObject_DecayFields24And28(u8 *o) {
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}
