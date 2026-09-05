#include "types.h"

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern struct State_080935b0 * volatile Data_03001e70;

void Map_SetWorkFourValues(s32 first, s32 second, s32 third, s32 fourth)
{
    volatile struct State_080935b0 *work = Data_03001e70;

    work->first = first;
    work->second = second;
    work->third = third;
    work->fourth = fourth;
}
