#include "types.h"

/* menu/selection/loop.c */
extern u8 *gIw;
extern u32 gIw2;
extern volatile u32 gIw3;
void WaitFrames(s32);
void FunctionHead_0801b9ec(void *state, u32 index);
void Menu_StepRight(void *state);
void Menu_StepLeft(void *state);
s32 Menu_ConfirmSelection(void *state);

s32 Menu_SelectionLoop(s32 mode)
{
    u8 *state = gIw;

    FunctionHead_0801b9ec(state, 0);
    for (;;) {
        WaitFrames(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (gIw3 & 0x10) {
                Menu_StepRight(state);
            } else if (gIw3 & 0x20) {
                Menu_StepLeft(state);
            } else if (gIw2 & 1) {
                return Menu_ConfirmSelection(state);
            }
        }
        if (mode != 0 && (gIw2 & 2)) {
            return -1;
        }
    }
}
