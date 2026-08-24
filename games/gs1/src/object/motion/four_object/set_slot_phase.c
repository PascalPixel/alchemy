#include "types.h"
#include "four_object_motion.h"

void FourObjectMotion_SetSlotPhase(s32 arg0, s32 arg1)
{
    Data_03001f2c->phases[arg0] = arg1;
}
