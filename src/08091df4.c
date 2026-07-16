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

s32 Func_080901c0(s32, s32);

void Func_08091df4(void) {
    void *temp_r5;
    register s16 *address asm("r2");
    register s32 value asm("r3");

    temp_r5 = *(void **)0x03001EBC;
    Func_080901c0(M2C_FIELD(temp_r5, s32 *, 0x1C0), M2C_FIELD(temp_r5, s32 *, 0x1C8));
    address = (s16 *)((u8 *)temp_r5 + 0x1C6);
    asm volatile("mov r3, #0\n\tstrh r3, [r2]"
                 : "+r" (address), "=r" (value) : : "memory");
}
