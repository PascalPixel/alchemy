#include "types.h"

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

extern struct PlacementState *Data_03001f2c;
extern const s32 Data_08073854[4];

struct RuntimeObject *Func_08009030(s32);
void Func_08009020(struct RuntimeObject *, s32);
void Func_080041d8(s32, s32);

void Func_0801ffd8(struct PlacementOrigin *origin, s32 x, s32 y)
{
    struct PlacementState *state = Data_03001f2c;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct RuntimeObject *object = Func_08009030(Data_08073854[i]);

            if (object != 0) {
                Func_08009020(object, 2);
                object->field_26 = 0;
                object->flags = (u8)(object->flags & ~0x0c);
            }

            state->objects[i] = object;
            state->x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->y[i] = (origin->y + y) * 8 + 0x10;
        }

        Func_080041d8(0x080200cd, 200 << 4);
    }
}
