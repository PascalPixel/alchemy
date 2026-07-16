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

s32 Func_08009140();
s32 Func_08009150(void *, s32, s32, s32);
s32 Func_08009158(void *);
void *Func_0808ba1c();

void Func_08092158(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r0;
    register void *first asm("r0");
    register s32 second asm("r1");
    register s32 third asm("r2");
    register s32 fourth asm("r3");

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s8 *, 0x5B) = 0;
        Func_08009140();
        first = temp_r0;
        second = arg1 << 0x10;
        third = M2C_FIELD(temp_r0, s32 *, 0xC);
        fourth = arg2 << 0x10;
        asm volatile("" : "+r" (first), "+r" (second),
                     "+r" (third), "+r" (fourth));
        Func_08009150(first, second, third, fourth);
        Func_08009158(temp_r0);
    }
}
