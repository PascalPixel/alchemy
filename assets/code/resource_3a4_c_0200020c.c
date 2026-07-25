typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000004d;
extern u8 Value_0000004f;
extern u8 Value_00000051;
extern u8 Value_00000052;
extern u8 Value_00000053;
extern u8 Value_00000054;
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Value_00000057;
extern u8 Data_0200c940[];
extern u8 Data_0200c9a0[];
extern u8 Data_0200ca00[];
extern u8 Data_0200ca60[];
extern u8 Data_0200caa8[];
extern u8 Data_0200cb68[];
extern u8 Data_0200cb98[];
extern u8 Data_0200cc40[];
extern u8 Data_0200ccd0[];
extern u8 Data_0200c928[];

s32 Func_0200020c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c940;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c9a0;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200ca00;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200ca60;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200caa8;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200cb68;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200cb98;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200cc40;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200ccd0;
    }
    return (s32)Data_0200c928;
}
