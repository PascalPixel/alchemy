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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08009140();
void Func_08009150(void *, s32, s32, s32);
void Func_08009158(void *);
void *Func_0808ba1c(s32);

void Func_08092158(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r0;

    temp_r0 = Func_0808ba1c(arg0);
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s8 *, 0x5B) = 0;
        Func_08009140();
        Func_08009150(temp_r0, arg1 << 0x10, M2C_FIELD(temp_r0, s32 *, 0xC), arg2 << 0x10);
        Func_08009158(temp_r0);
    }
}
