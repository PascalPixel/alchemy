#include "types.h"
#include "scene.h"
#include "abi/menu/selection/loop.h"

extern u8 *gIw;
extern u32 gIw2;
extern volatile u32 gIw3;

void WaitFrames(u32 value);

s32 Menu_SelectionLoop(s32 mode)
{
    u8 *state = gIw;

    Menu_Apply(state, 0);
    for (;;) {
        WaitFrames(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (gIw3 & 0x10) {
                Menu_Do(state);
            } else if (gIw3 & 0x20) {
                Menu_Do2(state);
            } else if (gIw2 & 1) {
                return Menu_Do3(state);
            }
        }
        if (mode != 0 && (gIw2 & 2)) {
            return -1;
        }
    }
}
