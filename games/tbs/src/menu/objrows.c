#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "character_menu.h"
#include "animspr.h"
#include "render_input.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "resource.h"

#if EDITION_INTERNATIONAL
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "global_cells.h"


void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = (u16 *)gWindowWork[0];
    s32 sum;

    sum = x + window->x;
    x = sum + 1;
    sum = y + ((struct UiWindow *)window)->y;
    y = sum + 1;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        s32 y_ofs = y << 6;
        s32 x_ofs = x << 1;
        s32 offset = y_ofs + x_ofs;
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((struct UiRenderWork *)map)->dirty = 1;
    }
}

#endif

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

struct PlacementDescriptor {
    u8 pad_00[0x2c];
    s8 kinds[4];
    u8 pad_30[3];
    s8 variant;
};

struct PlacementState {
    u8 pad_000[0x114];
    struct AnimationObject *objects[4];
    u8 pad_124[0x10];
    s16 x[4];
    u8 pad_13c[8];
    s16 y[4];
    u8 pad_14c[8];
    s32 scale[4];
};

s32 Object_ApplyProjectedPlacementFar(s32, void *, void *, s32);

extern struct FourObjectMotionState *gMenuWork;

void ObjectPlacement_CreateGroup(struct RenderInput *origin, s32 x, s32 y,
                                 struct PlacementDescriptor *descriptor)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 i;
    s32 duration;

    for (i = 0; i < 4 && descriptor->kinds[i] != -1; i++) {
        struct AnimationObject *object =
            ResourceObject_CreateFar(Party_RemapCharacterIdIfEnabledFar(descriptor->kinds[i],
                                        descriptor->variant));

        if (object != 0) {
            AnimationObjects_SelectAnimationFar(object, 1);
            ((struct FieldSprite *)object)->flags = 0;
            ((struct FieldSprite *)object)->priority = 0;
        }

        state->objects[i] = object;
        state->x[i] = (x + origin->x + i * 3) * 8 + 0x10;
        state->y[i] = (y + origin->y) * 8 + 0x10;
        state->scale[i] = 0x10000;
    }

    duration = 200;
    duration <<= 4;
    Scheduler_AddOrUpdateCallback(Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 i;

    Scheduler_RemoveCallback(Menu_UpdateFirstObjectRowPositions);
    for (i = 0; i < 4; i++) {
        if (state->objects[i] != 0) {
            ResourceObject_ReleaseFar(state->objects[i]);
            state->objects[i] = 0;
        }
    }
}

void Menu_UpdateFirstObjectRowPositions(void)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = (s32)state->objects[index];
        if (handle != 0) {
            source[0] = state->scale[index];
            source[1] = state->scale[index];
            request[0] = state->x[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (state->y[index] << 16) + 0x01F40000;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}

void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct AnimationObject *object = (struct AnimationObject *)ResourceObject_CreateFar(Menu_PartySpriteResourceIds[i]);

            if (object != 0) {
                AnimationObjects_SelectAnimationFar(object, 2);
                ((struct FieldSprite *)object)->flags = 0;
                ((struct FieldSprite *)object)->priority = 0;
            }

            state->objects[i] = object;
            state->positions_x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->positions_y[i] = (origin->y + y) * 8 + 0x10;
        }

        Scheduler_AddOrUpdateCallback(Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 i;

    Scheduler_RemoveCallback(Menu_UpdateSecondObjectRowPositions);
    for (i = 0; i < 4; i++) {
        if (state->objects[i] != 0) {
            ResourceObject_ReleaseFar(state->objects[i]);
            state->objects[i] = 0;
        }
    }
}

void Menu_UpdateSecondObjectRowPositions(void)
{
    struct FourObjectMotionState *root = gMenuWork;
    s32 scale[2];
    s32 position[4];
    s32 index;

    index = 0;
    do {
        void *object = root->objects[index];

        if (object != 0) {
            s32 unit = 0x10000;
            s32 bias = 0x1f40000;

            scale[0] = unit;
            scale[1] = unit;
            position[0] = root->positions_x[index] << 16;
            position[1] = bias;
            position[2] = (root->positions_y[index] << 16) + bias;
            position[3] = 0;
            Object_ApplyProjectedPlacementFar((s32)object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}
