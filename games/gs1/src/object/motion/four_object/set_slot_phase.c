#include "types.h"
#include "scene.h"
#include "abi/object/motion/four_object/set_slot_phase.h"
#include "four_object_motion.h"

void FourObjectMotion_SetSlotPhase(s32 arg0, s32 arg1)
{
    gIw->phases[arg0] = arg1;
}
