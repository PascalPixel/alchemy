typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

extern s32 Func_0200548e(s32);
extern void Func_0200546e(s32, s32, s32, s32, s32, s32);
extern void Func_0200548a(s32, s32, s32, s32, s32, s32);
extern void Func_02005494(s32, s32, s32, s32, s32, s32);
extern void Func_02005478(void);
extern void Func_02005416(s32);
extern void Func_0200567c(s32);

void Func_020018a4(void) {
    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Func_0200548e(0x915) != 0) {
        s32 k = 2;
        Func_0200546e(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Func_0200548a(8, 11, 2, 1, a, b); }
        Func_02005494(8, 12, 8, 11, k, 1);
        Func_02005478();
        Func_02005416(1);
    }
    if (Data_02000240[225] <= 3) {
        Func_0200567c(170);
    }
}
