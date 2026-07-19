typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct State_08078a60 {
    u8 pad[216];
    u16 values[15];
};

struct State_08078a60 *Func_08077394(u32);

s32 Func_08078a60(u32 object, u32 index) {
    struct State_08078a60 *state = Func_08077394(object);
    if (state->values[index] == 0) {
        return -1;
    }
    state->values[index] &= ~0x400;
    return 0;
}
