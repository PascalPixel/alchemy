#include "types.h"
#include "scene.h"
#include "abi/battle/effects/common/advance_scroll_on_interval.h"

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 gIw;
extern struct Position gIw2;

void BattleFx_AdvanceScrollOnInterval(void)
{
    u8 *base = (u8 *)gIw;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        gIw2.x += *(s32 *)(base + 0x7798);
        gIw2.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}
