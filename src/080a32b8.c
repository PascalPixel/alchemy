typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

struct State_080a32b8 {
    u8 pad_000[0x2c];
    s32 object;
    u8 pad_030[0x144];
    u16 selection;
    u16 field_176;
    u16 mask;
    u8 pad_17a[0xa0];
    u8 first_target;
    u8 second_target;
    u8 pad_21c[6];
    s16 flag;
    u8 pad_224[0x36];
    s16 value;
};

extern struct State_080a32b8 *Data_03001f2c;
extern s32 Func_080a9e48(u16, u8, u8);
extern s32 Func_080f9010(s32);
extern s32 Func_08015278(s32);
extern s32 Func_080a1d08(s32, s32, s32);
extern s32 Func_080aa448(u32);
extern s32 Func_08077010(u8);

s32 Func_080a32b8(void)
{
    struct State_080a32b8 *state;
    s32 result;

    state = Data_03001f2c;
    result = Func_080a9e48(
        state->selection, state->first_target, state->second_target);

    if (result == -1) {
        Func_080f9010(0x72);
        Func_08015278(state->object);
        Func_080a1d08(state->value + 0xbef, result, result);
        state->flag = 1;
        return result;
    }

    Func_080aa448(state->mask & 0x1ff);
    Func_08077010(state->first_target);
    Func_08077010(state->second_target);
    return 1;
}
