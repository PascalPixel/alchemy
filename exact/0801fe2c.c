#include "types.h"

struct PlacementOrigin {
    u8 pad_00[0x0c];
    u16 x;
    u16 y;
};

struct PlacementDescriptor {
    u8 pad_00[0x2c];
    s8 kinds[4];
    u8 pad_30[3];
    s8 variant;
};

struct PlacementState {
    u8 pad_000[0x114];
    void *objects[4];
    u8 pad_124[0x10];
    s16 x[4];
    u8 pad_13c[8];
    s16 y[4];
    u8 pad_14c[8];
    s32 scale[4];
};

struct RuntimeObject {
    u8 pad_00[9];
    s8 flags;
    u8 pad_0a[0x1c];
    s8 field_26;
};

extern struct PlacementState *Data_03001f2c;

s32 Func_0808a5f0(s8 kind, s8 variant);
struct RuntimeObject *Func_08009030(s32);
void Func_08009020(struct RuntimeObject *, s32);
void Func_080041d8(s32, s32);

void Func_0801fe2c(struct PlacementOrigin *origin, s32 x, s32 y,
                   struct PlacementDescriptor *descriptor)
{
    struct PlacementState *state = Data_03001f2c;
    s32 i;
    s32 duration;

    for (i = 0; i < 4 && descriptor->kinds[i] != -1; i++) {
        struct RuntimeObject *object =
            Func_08009030(Func_0808a5f0(descriptor->kinds[i],
                                        descriptor->variant));

        if (object != 0) {
            Func_08009020(object, 1);
            object->field_26 = 0;
            object->flags = (u8)(object->flags & ~0x0c);
        }

        state->objects[i] = object;
        state->x[i] = (x + origin->x + i * 3) * 8 + 0x10;
        state->y[i] = (y + origin->y) * 8 + 0x10;
        state->scale[i] = 0x10000;
    }

    duration = 200;
    duration <<= 4;
    Func_080041d8(0x0801ff59, duration);
}
