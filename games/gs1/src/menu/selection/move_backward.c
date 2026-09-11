#include "types.h"
#include "scene.h"
#include "abi/menu/selection/move_backward.h"

void WaitFrames(u32 value);

void Menu_MoveSelectionBackward(u8 *state)
{
    u32 *selection = (u32 *)(state + 0x39c);

    if (*selection != 0) {
        u32 no;

        Menu_Apply(state, *(u16 *)(state + 0x39e));
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        no = *(u16 *)(state + 0x39e);
        if (no == 1 && *(u16 *)selection != 0) {
            {
                u32 value = 8;

                *(u16 *)(state + 8) = value;
            }
            *(u16 *)selection += 0xffff;
            Menu_Apply2(state, 0);
            if (*(u16 *)selection == 0) {
                *(u16 *)(state + 10) = 0;
            }
            *(u16 *)(state + 62) = no;
        } else {
            *(u16 *)(state + 0x39e) += 0xffff;
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Menu_Apply3(state, *(u16 *)(state + 0x39e));
        }
        WaitFrames(1);
        Menu_Apply4(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        WaitFrames(1);
    }
}
