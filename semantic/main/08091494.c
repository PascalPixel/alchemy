#include "types.h"

struct DmaTransfer_08091494 {
    const void *source;
    void *destination;
    u32 control;
};

struct BlendRegisters_08091494 {
    u16 control;
    u16 alpha;
    u16 brightness;
};

struct State_08091494 {
    u8 padding[0x18];
    s32 object;
};

struct GlobalState_08091494 {
    u8 padding[0x1f4];
    u32 selected_object;
};

extern struct GlobalState_08091494 Data_02000240;
extern struct DmaTransfer_08091494 Data_040000d4;
extern struct BlendRegisters_08091494 Data_04000050;

void *Func_080048f4(s32 kind, s32 size);
void *Func_080048b0(s32 kind, s32 size);
void Func_08003fa4(s32 value, s32 size, void *destination);
void Func_08002dd8(s32 kind);
void Func_080041d8(void (*callback)(void), s32 period);
s32 Func_0808ba1c(u32 object_id);
void Func_080912b8(void);

void Func_08091494(s32 object)
{
    struct State_08091494 *state;
    void *buffer;
    u32 fill;
    u32 *source;
    volatile u16 *blend;
    s32 blend_value;
    s32 period;

    state = Func_080048f4(36, sizeof(*state));
    buffer = Func_080048b0(14, 0x400);

    source = &fill;
    fill = 0x11111111;
    Data_040000d4.source = source;
    Data_040000d4.destination = buffer;
    Data_040000d4.control = 0x85000080;

    Func_08003fa4(94, 0x200, buffer);
    Func_08002dd8(14);
    period = 200;
    period <<= 4;
    Func_080041d8(Func_080912b8, period);

    blend_value = 0x3f9e;
    blend = &Data_04000050.control;
    *blend++ = blend_value;
    blend_value = 0x10;
    *blend++ = blend_value;
    blend_value = 0x1f;
    *blend = blend_value;

    fill = 0;
    Data_040000d4.source = source;
    Data_040000d4.destination = state;
    Data_040000d4.control = 0x85000007;

    if (object == 0) {
        object = Func_0808ba1c(Data_02000240.selected_object);
    }
    state->object = object;
}
