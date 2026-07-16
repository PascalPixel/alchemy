typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32, s32);
s32 Func_080090e0(void *, s32);

void Func_08093570(void *arg0, s32 arg1) {
    void *temp_r5;

    temp_r5 = M2C_FIELD(Func_080048f4(0x1B, 0xCCC), void **, 0x1E0);
    if (arg0 != NULL) {
        Func_080090e0(temp_r5, 0);
        M2C_FIELD(temp_r5, void **, 0x68) = arg0;
        if (arg1 == 0) {
            M2C_FIELD(temp_r5, s32 *, 8) = (s32) M2C_FIELD(arg0, s32 *, 8);
            M2C_FIELD(temp_r5, s32 *, 0xC) = (s32) M2C_FIELD(arg0, s32 *, 0xC);
            M2C_FIELD(temp_r5, s32 *, 0x10) = (s32) M2C_FIELD(arg0, s32 *, 0x10);
        }
    }
}
