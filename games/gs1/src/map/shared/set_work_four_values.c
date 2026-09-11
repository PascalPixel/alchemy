#include "types.h"
#include "scene.h"
#include "abi/map/shared/set_work_four_values.h"

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern struct State_080935b0 *volatile gCam;

void Map_SetWorkFourValues(s32 first, s32 second, s32 third, s32 fourth)
{
    volatile struct State_080935b0 *work = gCam;

    work->first = first;
    work->second = second;
    work->third = third;
    work->fourth = fourth;
}
