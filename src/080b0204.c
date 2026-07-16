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

s32 Func_08002dd8(s32);
s32 Func_08003f3c(u16);
s32 Func_08004278(s32);
s32 Func_08015140();

void Func_080b0204(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_08004278(0x080B00F5);
    Func_08015140();
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x390));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x392));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x394));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x396));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x398));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x39A));
    Func_08002dd8(0x37);
}
