#include "types.h"

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

extern struct PlacementState *Data_03001f2c;
extern const s32 Data_08073854[4];

struct RuntimeObject *Func_08009030(s32);
void Object_InitializeMode(struct RuntimeObject *, s32);
void ScheduleCallbackAfterFrames(s32, s32);
void Func_080200cc(void);

void Menu_SpawnFourObjectsAtOrigin(struct PlacementOrigin *origin, s32 x, s32 y)
{
    struct PlacementState *state = Data_03001f2c;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct RuntimeObject *object = Func_08009030(Data_08073854[i]);

            if (object != 0) {
                Object_InitializeMode(object, 2);
                object->field_26 = 0;
                object->flags = (u8)(object->flags & ~0x0c);
            }

            state->objects[i] = object;
            state->x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->y[i] = (origin->y + y) * 8 + 0x10;
        }

        ScheduleCallbackAfterFrames((s32)Func_080200cc, 200 << 4);
    }
}


void ScheduleCallback(s32);
void Func_08009038(void *);

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    u8 *base = (u8 *)Data_03001f2c;
    s32 offset = 137;
    s32 zero;
    s32 count;

    ScheduleCallback((s32)Func_080200cc);
    zero = 0;
    offset *= 4;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            Func_08009038(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}



void Func_08009008(void *object, s32 *position, s32 *scale, s32 flags);

void Menu_UpdateSecondObjectRowPositions(void)
{
    struct PlacementState *root = Data_03001f2c;
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
            Func_08009008(object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}
