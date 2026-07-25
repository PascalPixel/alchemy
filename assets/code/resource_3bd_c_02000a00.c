typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000093;
extern u8 Value_00000095;
extern u8 Value_00000097;
extern u8 Data_0200c1b0[];
extern u8 Data_0200c270[];
extern u8 Data_0200c318[];
extern u8 Data_0200c198[];

s32 Func_02000a00(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c1b0;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c270;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c318;
    }
    return (s32)Data_0200c198;
}
