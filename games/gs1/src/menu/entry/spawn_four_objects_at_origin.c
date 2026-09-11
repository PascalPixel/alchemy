#include "types.h"
#include "scene.h"
#include "abi/menu/entry/spawn_four_objects_at_origin.h"

struct PlacementOrigin {
    u8 pad_00[0x0c];
    u16 x;
    u16 y;
};

struct PlacementState {
    u8 pad_000[0x224];
    void *objects[4];
    s16 x[4];
    s16 y[4];
};

struct RuntimeObject {
    u8 pad_00[9];
    s8 flags;
    u8 pad_0a[0x1c];
    s8 field_26;
};

extern struct PlacementState *gIw;
extern const s32 gRom[4];

struct RuntimeObject *Menu_Run(s32);
void Object_InitializeMode(struct RuntimeObject *, s32);
void ScheduleCallbackAfterFrames(s32, s32);

void Menu_SpawnFourObjectsAtOrigin(struct PlacementOrigin *origin, s32 x, s32 y)
{
    struct PlacementState *state = gIw;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct RuntimeObject *object = Menu_Run(gRom[i]);

            if (object != 0) {
                Object_InitializeMode(object, 2);
                object->field_26 = 0;
                object->flags = (u8)(object->flags & ~0x0c);
            }

            state->objects[i] = object;
            state->x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->y[i] = (origin->y + y) * 8 + 0x10;
        }

        ScheduleCallbackAfterFrames((s32)Menu_Run2, 200 << 4);
    }
}
