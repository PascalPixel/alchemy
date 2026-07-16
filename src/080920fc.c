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

s32 Func_08009148(void *);
void *Func_0808ba1c();
s32 Func_08093a6c(void *, s32);

void Func_080920fc(s32 arg0, s32 arg1) {
    register u8 current asm("r2");
    register u8 result asm("r3");
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        current = M2C_FIELD(temp_r0, u8 *, 0x5A);
        asm volatile("" : "+r" (current));
        result = 1;
        asm volatile("" : "+r" (result) : "r" (current));
        result |= current;
        M2C_FIELD(temp_r0, u8 *, 0x5A) = result;
        Func_08093a6c(temp_r0, arg1);
        Func_08009148(temp_r0);
    }
}
