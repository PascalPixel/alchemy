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

s32 Func_080cd594(s32);
s32 Func_080cdbc0();
s32 Func_080de2f8(void *, s32, s32, s32);

void Func_080e6948(void *arg0) {
    void *temp_r5;
    register void *saved asm("r6");
    register void *first asm("r0");
    register s32 second asm("r1");
    register s32 third asm("r2");
    register s32 fourth asm("r3");

    temp_r5 = *(void **)0x03001EEC;
    saved = arg0;
    M2C_FIELD(temp_r5, void **, 0x7828) = saved;
    first = 0;
    asm volatile("sub sp, #16" : "+r" (first) : : "memory");
    Func_080cd594(first);
    third = M2C_FIELD(M2C_FIELD(temp_r5, void **, 0x7828), s32 *, 4) ^ 1;
    asm volatile(
        "add r3, sp, #12\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #8\n\t"
        "str r3, [sp, #4]"
        : "+r" (third) : : "r0", "r1", "r3", "memory");
    first = saved;
    second = 7;
    fourth = 0;
    asm volatile("" : "+r" (first), "+r" (second),
                 "+r" (third), "+r" (fourth));
    Func_080de2f8(first, second, third, fourth);
    Func_080cdbc0();
    asm volatile("add sp, #16" : : : "memory");
}
