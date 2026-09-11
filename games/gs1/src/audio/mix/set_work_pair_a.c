#include "types.h"
#include "scene.h"
#include "abi/audio/mix/set_work_pair_a.h"

extern u16 gOv;
extern u16 gOv2;

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    gOv = primary;
    gOv2 = secondary;
}
