typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern u8 Value_00000d77;
extern s32 Func_02000240();

void Func_020001b8(void) {
    Func_02000240((s32)&Value_00000d77, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
