typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern s32 Func_020001b6();

void Func_02000130(void) {
    Func_020001b6((s32)&Value_00000c9b, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
