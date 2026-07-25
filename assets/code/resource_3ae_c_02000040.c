typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 Data_02009738[];
extern u8 Data_020097c8[];
extern u8 Data_02009840[];
extern u8 Data_02009708[];

s32 Func_02000040(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000006b) {
        return (s32)Data_02009738;
    }
    if (v == (s32)&Value_00000070) {
        return (s32)Data_020097c8;
    }
    if (v == (s32)&Value_0000006c) {
        return (s32)Data_02009840;
    }
    return (s32)Data_02009708;
}
