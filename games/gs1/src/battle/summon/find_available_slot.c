#include "types.h"

u8 *Runtime_GetObject(s32);

s32 Summon_FindSlot(void) {
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 0x80;
        if (Runtime_GetObject(id)[0x12A] == 0)
            break;
    }
    if (i == 6)
        return -1;
    return id;
}
