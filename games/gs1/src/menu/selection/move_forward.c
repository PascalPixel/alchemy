#include "types.h"
#include "scene.h"
#include "abi/menu/selection/move_forward.h"

struct State_0801b9a8;
struct State_0801b9ec;

void WaitFrames(u32 value);

void Menu_MoveSelectionForward(u8 *state)
{
    u16 *selection = (u16 *)(state + 0x39c);
    u16 *index = (u16 *)(state + 0x39e);
    u16 *count;
    u32 end = *selection + *index + 1;

    count = (u16 *)(state + 0x394);
    if (end != *count) {
        Menu_Apply(state, *index);
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        WaitFrames(1);
        *index += 1;
        if (*index == 4 && end + 1 < *count) {
            *index += 0xffff;
            {
                u32 value = 8;

                *(u16 *)(state + 60) = value;
            }
            *selection += 1;
            Menu_Apply2(state, 1);
            if (*selection + *index + 2 == *count) {
                u32 value = 0;

                *(u16 *)(state + 62) = value;
            }
            {
                u32 value = 1;

                *(u16 *)(state + 10) = value;
            }
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
