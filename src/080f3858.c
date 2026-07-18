typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_080f2ebc(s16 *, s16 *, s16 *, s32);

typedef struct {
    u8 padding[0x3001];
    s8 value;
    s8 zero;
} State;

void Func_080f3858(s32 arg0) {
    State *state = *(State **)0x03001ED0;

    if (state != 0) {
        state->value = arg0;
        state->zero = 0;
        Func_080f2ebc((s16 *)((u8 *)state + 0x400),
            (s16 *)((u8 *)state + 0x1000),
            (s16 *)((u8 *)state + 0x1C00), arg0);
    }
}
