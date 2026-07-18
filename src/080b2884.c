typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 padding[0x3AA];
    s8 mode;
} State;

extern State *Data_03001f2c;
extern u8 Value_00000d24;
extern u8 Value_00000d2e;
extern u8 Value_00000d38;
extern u8 Value_00000d42;

s32 Func_080b2884(s32 value) {
    s8 mode = Data_03001f2c->mode;

    if (mode == 1) {
        value += (u32)&Value_00000d2e - (u32)&Value_00000d24;
    }
    if (mode == 2) {
        value += (u32)&Value_00000d38 - (u32)&Value_00000d24;
    }
    if (mode == 3) {
        value += (u32)&Value_00000d42 - (u32)&Value_00000d24;
    }
    return value;
}
