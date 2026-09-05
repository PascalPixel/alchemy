#include "types.h"

struct Entry_080bd3c8 {
    u8 unknown_00[9];
    u8 status;
};

void *Ability_GetData();

u32 Ability_CheckStatusOrSpecialId(s32 value) {
    u8 status;

    if (value == 0x7E) {
        return 1U;
    }
    status = ((struct Entry_080bd3c8 *)Ability_GetData())->status;
    return (u32) ((0 - status) | status) >> 0x1F;
}
