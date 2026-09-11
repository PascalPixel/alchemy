#include "types.h"
#include "scene.h"
#include "abi/object/motion/four_object/clear_slots_and_schedule.h"
#include "four_object_motion.h"

void ScheduleCallback(s32);
extern u8 gRom;

void FourObjectMotion_ClearSlotsAndSchedule(void)
{
    struct FourObjectMotionState *state = gIw;
    s32 index = 0;

    do {
        void *object = state->objects[index];

        if (object != 0) {
            Obj_Do(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    ScheduleCallback((s32)&gRom);
}
