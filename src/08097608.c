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
s32 Func_08004278(s32);
s32 Func_0808e0b0(s32, s32);
s32 Func_08092054(u16);
s32 Func_0809748c();
s32 Func_08097adc();

void Func_08097608(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001EA8;
    Func_08097adc();
    Func_08004278(0x08097645);
    Func_0808e0b0(Func_08092054(M2C_FIELD(temp_r5, u16 *, 0x290)), 1);
    Func_0809748c();
    Func_08002dd8(0x16);
}
