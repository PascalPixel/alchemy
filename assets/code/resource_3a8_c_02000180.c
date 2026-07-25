typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009c;
extern u8 Data_0200c768[];
extern u8 Data_0200ca20[];
extern u8 Data_0200ca80[];
extern u8 Data_0200cb58[];
extern u8 Data_0200c750[];

s32 Func_02000180(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c768;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200ca20;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200ca80;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200cb58;
    }
    return (s32)Data_0200c750;
}
