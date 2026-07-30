typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009b;
extern u8 Value_0000009c;

extern void Func_0200522e(s32);
extern void Func_02002d24(void);
extern void Func_02002e5c(void);
extern void Func_02002ed8(void);
extern void Func_02002f30(void);
extern void Func_02002fc8(void);

s32 Func_0200164c(void) {
    s32 v;

    Func_0200522e(0x87a);
    v = Data_02000240[224];
    if (v == (s32)&Value_00000063) {
        Func_02002d24();
    } else if (v == (s32)&Value_00000066) {
        Func_02002e5c();
    } else if (v == (s32)&Value_00000099) {
        Func_02002ed8();
    } else if (v == (s32)&Value_0000009b) {
        Func_02002f30();
    } else if (v == (s32)&Value_0000009c) {
        Func_02002fc8();
    }
    return 0;
}
