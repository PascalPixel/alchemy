#include "TYPES.H"
#include "SCENE.H"

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern struct State_080935b0 *gCam;

void Map_SetWorkFourValues(s32 first, s32 second, s32 third, s32 fourth)
{
    struct State_080935b0 *work = gCam;

    work->first = first;
    work->second = second;
    work->third = third;
    work->fourth = fourth;
}
