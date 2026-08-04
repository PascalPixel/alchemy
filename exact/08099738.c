#include "object_lookup.h"
#include "types.h"

struct Controller_08099738 {
    u8 pad_00[5];
    u8 field_05;
};

struct State_08099738 {
    u8 pad_00[0x25];
    u8 field_25;
    u8 field_26;
    u8 pad_27;
    struct Controller_08099738 *controller;
};

struct Object_08099738 {
    u8 pad_00[0x50];
    struct State_08099738 *state;
    u8 pad_54[0x18];
    u32 field_6c;
};

struct Global_08099738 {
    u8 pad_000[0x1F4];
    u32 object_id;
    u8 pad_1f8[0x54];
    u16 field_24c;
};

extern struct Global_08099738 Data_02000240;

void Func_080f9010(s32);
s32 Func_08004278(void (*callback)(void));
void Func_08009080(struct Object_08099738 *, s32);
void Func_080030f8(s32);
void Func_08099678(void);

void Func_08099738(void)
{
    struct Object_08099738 *object;
    struct State_08099738 *state;
    struct Controller_08099738 *controller;
    u32 i;

    object = Func_0808ba1c(Data_02000240.object_id);
    state = object->state;
    controller = state->controller;

    Func_080f9010(154);
    Func_08004278(Func_08099678);
    Func_08009080(object, 0);
    object->field_6c = 0;

    for (i = 0; i < 5; ++i) {
        controller->field_05 = 7;
        state->field_25 = 1;
        state->field_26 = 2;
        Func_080030f8(2);
        state->field_25 = 1;
        state->field_26 = 0;
        Func_080030f8(2);
    }

    for (i = 0; i < 5; ++i) {
        controller->field_05 = 7;
        state->field_25 = 1;
        state->field_26 = 0;
        Func_080030f8(2);
        controller->field_05 = 0;
        state->field_25 = 1;
        Func_080030f8(2);
    }

    state->field_26 = 1;
    Data_02000240.field_24c = 0;
}
