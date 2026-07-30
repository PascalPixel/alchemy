typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

void Func_0200469e(void);
void Func_0200469e_b(s32, s32, s32, s32, s32, s32);
s32 Func_02004684(s32);
s32 Func_0200468e(s32);
void Func_0200477e(s32, s32);
void Func_02004798(s32, s32, s32, s32);
void Func_020047a4(void);
void Func_020047ea(s32);
s32 Func_020046d4(s32);
void Func_020046ba(s32, s32, s32, s32, s32, s32);
void Func_020047b4(s32, s32, s32);
void Func_02004712(s32);
void Func_02004708(s32);
s32 Func_02004706(s32);
void Func_02002372(void);
void Func_0200473a(void);

void Func_02002244(void) {
    Func_0200469e();
    if (Func_02004684(0x818) == 0 && Func_0200468e(0x816) == 0) {
        Func_0200477e(0x20000, 0x4000);
        Func_02004798(0x11e0000, -1, 0x920000, 1);
        Func_020047a4();
        Func_020047ea(186);
        { s32 k5 = 4, k6 = 3; Func_0200469e_b(0, 59, 15, 38, k5, k6); }
        if (Func_020046d4(0x817) != 0) {
            Func_020046ba(8, 60, 17, 39, 2, 2);
        }
        Func_020047b4(0, 0, 0);
        Func_02004712(30);
        Func_02004708(0x816);
        if (Func_02004706(0x817) != 0) {
            Func_02002372();
        }
    }
    Func_0200473a();
}
