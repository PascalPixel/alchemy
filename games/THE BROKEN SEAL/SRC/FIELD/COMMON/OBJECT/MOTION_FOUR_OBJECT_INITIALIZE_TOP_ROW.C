#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "FOUR_OBJECT_MOTION.H"
#include "FIELD_SPRITE.H"
#include "ANIMSPR.H"
#include "RESOURCE.H"

#define Link_DrawShiftedTilePairFar ((void (*)(void*))Link_DrawShiftedTilePairFar)
s32 Object_ApplyProjectedPlacementFar(u32 object, u32 *request, u32 *motion, u32 limit);
extern struct FourObjectMotionState *gMenuWork;

void FourObjectMotion_InitializeTopRow(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index;

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

        if (object != NULL) {
            AnimationObjects_SelectAnimationFar(object, 2);
        }
        state->objects[index] = object;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0x20;
    }
    Scheduler_AddOrUpdateCallback((s32)(FourObjectMotion_UpdateAllPositions), 0xc80);
}

void FourObjectMotion_ClearSlotsAndSchedule(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index = 0;

    do {
        struct AnimationObject *object = state->objects[index];

        if (object != 0) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    Scheduler_RemoveCallback((u32)FourObjectMotion_UpdateAllPositions);
}

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

void FourObjectMotion_UpdateAllPositions(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index;
    u32 motion[2];
    u32 request[4];

    index = 0;
    while (1) {
        u32 object = (u32)state->objects[index];

        if (object != 0) {
            u32 vertical = VERTICAL_ORIGIN -
                ((u32)(s32)state->vertical_origins[index] << 16);
            ((struct FieldSprite *)object)->priority = 0;
            motion[0] = FIXED_ONE;
            motion[1] = FIXED_ONE;
            request[0] = (u32)(s32)state->positions_x[index] << 16;
            request[1] = vertical;
            request[2] = ((u32)(s32)state->positions_y[index] << 16) + vertical;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(object, request, motion, 0x4000);
        }
        index += 1;
        if (index > 3)
            break;
    }
}

extern struct FourObjectMotionState *gMenuWork;

s32 Object_ApplyProjectedPlacementFar(u32, u32 *, u32 *, u32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    u32 motion[2];
    u32 request[4];

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        u32 obj = (u32)work->objects[i];

        if (obj != 0) {
            u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;
            /* FAKEMATCH: the row's second read lives in r2, as the native allocates it. */
            register s32 py asm("r2");

            ((struct FieldSprite *)obj)->priority = 0;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + (s32)(0x10000 - phase) / 3;
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
            request[3] = 0;
            py = work->positions_y[i];
            limit = 0x8000;
            if (py >= 0)
                limit = 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}
