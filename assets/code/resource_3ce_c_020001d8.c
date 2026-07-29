typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern u8 Value_00000da2;
extern s32 Func_02000260();

void Func_020001d8(void) {
    Func_02000260((s32)&Value_00000da2, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
