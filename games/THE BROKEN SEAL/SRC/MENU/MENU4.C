#include "CHARACTER_MENU.H"
#include "ANIMSPR.H"
#include "RENDER_INPUT.H"
#include "FOUR_OBJECT_MOTION.H"
#include "FIELD_SPRITE.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"

s32 Func_0808a5f0(s8 kind, s8 variant);

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

extern struct PlacementState *Data_03001f2c_a;
struct AnimationObject *GetBattleEffectObject(s32);
s32 Object_InitializeMode(struct AnimationObject *, s32);
void Menu_UpdateFirstObjectRowPositions(void);

/* menu/entry/clear_first_object_row_and_schedule_update.c */
void ResourceObject_ReleaseFar(void *);

/* menu/update_first_object_row_positions.c */
s32 Object_ApplyProjectedPlacementFar(s32, void *, void *, s32);

extern struct FourObjectMotionState *gMenuWork;
extern const s32 Menu_PartySpriteResourceIds[4];
void *ResourceObject_CreateFar(s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
void Menu_UpdateSecondObjectRowPositions(void);

void ObjectPlacement_CreateGroup(struct RenderInput *origin, s32 x, s32 y,
                                 struct PlacementDescriptor *descriptor)
{
    struct PlacementState *state = Data_03001f2c_a;
    s32 i;
    s32 duration;

    for (i = 0; i < 4 && descriptor->kinds[i] != -1; i++) {
        struct AnimationObject *object =
            GetBattleEffectObject(Func_0808a5f0(descriptor->kinds[i],
                                        descriptor->variant));

        if (object != 0) {
            Object_InitializeMode(object, 1);
            object->flags = 0;
            ((struct FieldSprite *)object)->priority = 0;
        }

        state->objects[i] = object;
        state->x[i] = (x + origin->x + i * 3) * 8 + 0x10;
        state->y[i] = (y + origin->y) * 8 + 0x10;
        state->scale[i] = 0x10000;
    }

    duration = 200;
    duration <<= 4;
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    struct PlacementState *state = Data_03001f2c_a;
    s32 index = 0;
    s32 count;

    Scheduler_RemoveCallback((u32)((s32)Menu_UpdateFirstObjectRowPositions));
    count = 3;
    do {
        struct AnimationObject *entry = state->objects[index];

        if (entry != 0) {
            ResourceObject_ReleaseFar(entry);
            state->objects[index] = NULL;
        }
        count--;
        index++;
    } while (count >= 0);
}

void Menu_UpdateFirstObjectRowPositions(void)
{
    struct PlacementState *state = Data_03001f2c_a;
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
                object->flags = 0;
                ((struct FieldSprite *)object)->priority = 0;
            }

            state->objects[i] = object;
            state->positions_x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->positions_y[i] = (origin->y + y) * 8 + 0x10;
        }

        Scheduler_AddOrUpdateCallback((s32)Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index = 0;
    s32 count;

    Scheduler_RemoveCallback((u32)((s32)Menu_UpdateSecondObjectRowPositions));
    count = 3;
    do {
        void *entry = state->objects[index];

        if (entry != 0) {
            ResourceObject_ReleaseFar(entry);
            state->objects[index] = NULL;
        }
        count--;
        index++;
    } while (count >= 0);
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
