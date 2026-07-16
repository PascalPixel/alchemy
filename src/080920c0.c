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

void *Func_0808ba1c();
s32 Func_08092054(s32);
s32 Func_08093a6c(void *, s32);

void Func_080920c0(s32 arg0, s32 arg1) {
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s32 *, 0x68) = Func_08092054(arg1);
        Func_08093a6c(temp_r0, 0x0809FF40);
    }
}
