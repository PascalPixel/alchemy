typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000c1a;
extern s32 Data_02009684[];

extern s32 Func_020013fc();
extern s32 Func_020014c6();
extern s32 Func_020014ce();
extern s32 Func_020014d6();
extern s32 Func_020014de();
extern s32 Func_0200149a();
extern s32 Func_020014a0();
extern s32 Func_020014a6();
extern s32 Func_020014ac();

void Func_02000244(void) {
    Func_020013fc((s32)&Value_00000c1a, 1);
    Func_020014c6(0, Data_02009684[0]);
    Func_020014ce(1, Data_02009684[0]);
    Func_020014d6(3, Data_02009684[0]);
    Func_020014de(2, Data_02009684[0]);
    Data_02009684[0] += 10;
    Func_0200149a(0);
    Func_020014a0(1);
    Func_020014a6(3);
    Func_020014ac(2);
}
