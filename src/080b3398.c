#include "types.h"

struct RuntimeState_080b3398 {
    u8 padding0[0x1C0];
    s32 effect;
    u8 padding1C4[4];
    s32 delay;
};

struct Object_080b3398 {
    u8 padding0[0x34];
    u16 saved_x;
    u16 saved_y;
    s16 x;
    u16 y;
};

extern struct RuntimeState_080b3398 *Data_03001ebc;

s32 Func_08077158(s16 *);
void Func_08077230(s32);
struct Object_080b3398 *Func_08077008(s32);
void Func_08077128(s32);
void Func_080030f8(s32);
void Func_0808a368(void);
void Func_0808a370(void);
void Func_080f9010(s32);
void Func_080b04c4(void);
void Func_0808a360(void);

void Func_080b3398(s32 value)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct Object_080b3398 *object;
    struct RuntimeState_080b3398 *state;

    count = Func_08077158(objects);
    Func_08077230(-value);

    for (index = 0; index < count; index++) {
        object = Func_08077008(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Func_08077128(objects[index]);
        }
    }

    state = Data_03001ebc;
    state->effect = 0x209;
    state->delay = 60;
    Func_080030f8(20);
    Func_0808a368();
    Func_0808a370();
    Func_080f9010(86);
    Func_080b04c4();
    Func_080030f8(10);
    Func_0808a360();
    Func_0808a370();
    Func_080030f8(30);
    Data_03001ebc->delay = 16;
}
