#include "types.h"

void Func_08019ee4(s32 value, s32 mode, s32 *output, s32 *result, s32 enabled);
void Func_08019fcc(s32 value, s32 mode, s32 *output, s32 *result, s32 enabled);
void Func_0801a3d0(s32 value, s32 mode, s32 *output, s32 *result, s32 enabled);
extern u8 Data_0000001f;
extern u8 Data_00000182;
extern u8 Data_00000333;

struct Config_0801bd98 {
    u8 pad0[5];
    unsigned int keep5_0 : 2;
    unsigned int clear5_2 : 2;
    unsigned int clear5_4 : 1;
    unsigned int clear5_5 : 1;
    unsigned int clear5_6 : 2;
    u8 pad6;
    unsigned int keep7_0 : 6;
    unsigned int set7_6 : 2;
    unsigned int value8 : 10;
    unsigned int clear8_10 : 2;
    unsigned int clear8_12 : 4;
};

struct State_0801bd98 {
    u8 pad0[8];
    u16 value;
    u16 operation;
    u16 output;
    u16 result;
    u8 pad16[16];
    u16 offset;
    u16 scale_x;
    u8 pad36[2];
    u16 scale_y;
    struct Config_0801bd98 config;
};

void Func_0801bd98(u32 operation, s32 value, struct State_0801bd98 *state, s32 enabled)
{
    s32 result;
    s32 output;
    s32 offset;

    switch (operation) {
    case 1:
    case 6:
        if (enabled != 0) {
            output = state->output;
        }
        Func_08019ee4(value, 0, &output, &result, enabled);
        offset = (s32)&Data_0000001f;
        goto set_offset;
    case 2:
        if (enabled != 0) {
            output = state->output;
        }
        Func_08019fcc(value, 1, &output, &result, enabled);
        offset = (s32)&Data_00000182;
        goto set_offset;
    case 4:
        if (enabled != 0) {
            output = state->output;
        }
        Func_0801a3d0(value, 1, &output, &result, enabled);
        offset = (s32)&Data_00000333;
        goto set_offset;
    default:
        goto finish;
    }

set_offset:
    state->offset = value + offset;

finish:
    state->value = value;
    state->output = output;
    state->result = result;
    state->operation = operation;
    state->scale_x = 0x100;
    state->scale_y = 0x100;

    {
        struct Config_0801bd98 *config = &state->config;
        config->clear5_2 = 0;
        config->clear5_5 = 0;
        config->clear5_4 = 0;
        config->clear5_6 = 0;
        config->set7_6 = 1;
        config->clear8_12 = 0;
        config->value8 = result;
        config->clear8_10 = 0;
    }
}
