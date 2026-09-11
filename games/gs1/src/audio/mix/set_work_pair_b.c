#include "types.h"
#include "scene.h"
#include "abi/audio/mix/set_work_pair_b.h"

extern u16 gOv;
extern u16 gOv2;

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    gOv = primary;
    gOv2 = secondary;
}
