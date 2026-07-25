typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009a;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Data_0200cbf4[];
extern u8 Data_0200ce88[];
extern u8 Data_0200cedc[];
extern u8 Data_0200cf24[];
extern u8 Data_0200cf54[];
extern u8 Data_0200cf9c[];
extern u8 Data_0200cbe8[];

s32 Func_02000240(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200cbf4;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ce88;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200cedc;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200cf24;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200cf54;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cf9c;
    }
    return (s32)Data_0200cbe8;
}
