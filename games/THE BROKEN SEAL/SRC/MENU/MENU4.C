#include "TYPES.H"
#include "SCENE.H"
#include "INN.H"

extern struct InnState *Data_03001f2c;

/* menu/entry/clear_first_object_row_and_schedule_update.c */
void Scheduler_RemoveCallback(s32);
void Menu_UpdateFirstObjectRowPositions(void);
void ResourceObject_ReleaseFar(void *);

/* menu/update_first_object_row_positions.c */
s32 Object_ApplyProjectedPlacementFar(s32, void *, void *, s32);

struct PlacementOrigin {
    u8 unknown_00[0x0c];
    u16 x;
    u16 y;
};

struct PlacementState {
    u8 unknown_000[0x224];
    void *objects[4];
    s16 x[4];
    s16 y[4];
};

struct RuntimeObject {
    u8 unknown_00[9];
    s8 flags;
    u8 unknown_0a[0x1c];
    s8 field_26;
};

extern struct PlacementState *gMenuWork;
extern const s32 Menu_PartySpriteResourceIds[4];
struct RuntimeObject *ResourceObject_CreateFar(s32);
void AnimationObjects_SelectAnimationFar(struct RuntimeObject *, s32);
void Scheduler_AddOrUpdateCallback(s32, s32);
void Menu_UpdateSecondObjectRowPositions(void);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    u8 *base = (u8 *)Data_03001f2c;
    s32 offset = 138;
    s32 zero;
    s32 count;

    Scheduler_RemoveCallback((s32)Menu_UpdateFirstObjectRowPositions);
    zero = 0;
    offset *= 2;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            ResourceObject_ReleaseFar(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}

void Menu_UpdateFirstObjectRowPositions(void)
{
    u8 *base = (u8 *)Data_03001f2c;
    s16 *offsets = (s16 *)(base + 0x134);
    s32 *entries = (s32 *)(base + 0x114);
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = entries[index];
        if (handle != 0) {
            source[0] = entries[index + 0x10];
            source[1] = entries[index + 0x10];
            request[0] = offsets[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (offsets[index + 8] << 16) + 0x01F40000;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}

void Menu_SpawnFourObjectsAtOrigin(struct PlacementOrigin *origin, s32 x, s32 y)
{
    struct PlacementState *state = gMenuWork;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct RuntimeObject *object = ResourceObject_CreateFar(Menu_PartySpriteResourceIds[i]);

            if (object != 0) {
                AnimationObjects_SelectAnimationFar(object, 2);
                object->field_26 = 0;
                object->flags = (u8)(object->flags & ~0x0c);
            }

            state->objects[i] = object;
            state->x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->y[i] = (origin->y + y) * 8 + 0x10;
        }

        Scheduler_AddOrUpdateCallback((s32)Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    u8 *base = (u8 *)gMenuWork;
    s32 offset = 137;
    s32 zero;
    s32 count;

    Scheduler_RemoveCallback((s32)Menu_UpdateSecondObjectRowPositions);
    zero = 0;
    offset *= 4;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            ResourceObject_ReleaseFar(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}

void Menu_UpdateSecondObjectRowPositions(void)
{
    struct PlacementState *root = gMenuWork;
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
            position[0] = root->x[index] << 16;
            position[1] = bias;
            position[2] = (root->y[index] << 16) + bias;
            position[3] = 0;
            Object_ApplyProjectedPlacementFar(object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}
