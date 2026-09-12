#include "types.h"
#include "scene.h"
#include "four_object_motion.h"

extern u8 RomBytes_080ad35d[];
extern u8 RomBytes_080ad40d[];

/* object/motion/four_object/initialize_top_row.c */
extern s32 RomBytes_080af304[];

void *Obj_Run(s32);
void Object_InitializeMode(void *, s32);

s32 ScheduleCallbackAfterFrames(void (*)(void), s32);
void FunctionHead_080ad35c(void);
void FunctionHead_080ad40c(void);

void FourObjectMotion_InitializeTopRow(void)
{
    struct FourObjectMotionState *state = gIw;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            Obj_Do(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        void *object = Obj_Run(RomBytes_080af304[index]);

        if (object != NULL) {
            Object_InitializeMode(object, 2);
        }
        state->objects[index] = object;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0x20;
    }
    ScheduleCallbackAfterFrames(FunctionHead_080ad35c, 0xc80);
}

/* object/motion/four_object/clear_slots_and_schedule.c */
void ScheduleCallback(s32);

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
    ScheduleCallback((s32)&RomBytes_080ad35d);
}

/* object/motion/four_object/update_all_positions.c */
#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

struct ObjectFlags_080ad35c { s8 flags; };

void FourObjectMotion_UpdateAllPositions(void)
{
    struct FourObjectMotionState *state = gIw;
    s32 index;
    u32 motion[2];
    u32 request[4];

    index = 0;
    while (1) {
        u32 object = (u32)state->objects[index];

        if (object != 0) {
            u32 vertical = VERTICAL_ORIGIN -
                ((u32)(s32)state->vertical_origins[index] << 16);
            ((struct ObjectFlags_080ad35c *)(object + 9))->flags &= -13;
            motion[0] = FIXED_ONE;
            motion[1] = FIXED_ONE;
            request[0] = (u32)(s32)state->positions_x[index] << 16;
            request[1] = vertical;
            request[2] = ((u32)(s32)state->positions_y[index] << 16) + vertical;
            request[3] = 0;
            Obj_SetMode(object, request, motion, 0x4000);
        }
        index += 1;
        if (index > 3)
            break;
    }
}

/* object/motion/four_object/initialize_bottom_row.c */


s32 ScheduleCallbackAfterFrames(void (*)(void), s32);

void FourObjectMotion_InitializeBottomRow(void)
{
    struct FourObjectMotionState *state = gIw;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            Obj_Do(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        void *object = Obj_Run(RomBytes_080ad40d[index]);

        if (object != NULL) {
            Object_InitializeMode(object, 2);
        }
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    ScheduleCallbackAfterFrames(FunctionHead_080ad40c, 0xc80);
}

/* object/motion/four_object/set_slot_position.c */
#define NEGATIVE_POSITION_MASK ((s32)0xffff8000)
#define POSITIONS_Y_OFFSET 0x23c

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 position_x, s32 position_y, s32 negative)
{
    struct FourObjectMotionState *state = gIw;

    if (state->objects[index] != 0) {
        state->positions_x[index] = position_x;
        state->positions_y[index] =
            negative != 0 ? position_y | NEGATIVE_POSITION_MASK : position_y;
        return POSITIONS_Y_OFFSET + index * 2;
    }
    return index;
}

/* object/motion/four_object/set_slot_phase.c */
void FourObjectMotion_SetSlotPhase(s32 arg0, s32 arg1)
{
    gIw->phases[arg0] = arg1;
}

/* object/motion/four_object/replace_slot.c */


s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
    struct FourObjectMotionState *state = gIw;

    if (state->objects[index] != 0) {
        Obj_Do(state->objects[index]);
        state->objects[index] = 0;
    }
    {
        void *object = Obj_Run(RomBytes_080ad40d[kind]);

        if (object != 0) {
            Object_InitializeMode(object, value);
        }
        state->objects[index] = object;
    }
    return 1;
}

/* object/motion/four_object/clear_slots_and_schedule_alt.c */

void FourObjectMotion_ClearSlotsAndScheduleAlt(void)
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
    ScheduleCallback((s32)&RomBytes_080ad40d);
}
