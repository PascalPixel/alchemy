#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/fetch_rectangle_blitters.h"

extern u8 gIw[];

void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output)
{
    if (alternate == 0) {
        u8 *state;
        u32 value;

        Battle_SetRange(alternate = 46, 7, 7, 3, 2);
        state = gIw;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        Battle_SetRange(alternate, 7, 7, 3, 3);
        output[1] = *(u32 *)(state += 188);
    } else {
        u8 *state;
        u32 value;

        Battle_SetRange(alternate = 46, 7, 7, 7, 2);
        state = gIw;
        value = *(u32 *)(state + 184);
        alternate = 47;
        output[0] = value;
        Battle_SetRange(alternate, 7, 7, 7, 3);
        output[1] = *(u32 *)(state += 188);
    }
}
