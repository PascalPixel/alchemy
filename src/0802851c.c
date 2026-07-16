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

s32 Func_08002dd8(s32);
s32 Func_080030f8(s32);
s32 Func_08003f3c(u16);
s32 Func_08004278(s32);
s32 Func_08016418(s32, s32);

void Func_0802851c(void) {
    s32 temp_r0;
    s32 var_r6;
    register u8 *var_r5 asm("r5");
    register s16 *limit asm("r7");
    void *temp_r5;

    temp_r5 = *(void **)0x03001F38;
    Func_08004278(0x08028195);
    temp_r0 = M2C_FIELD(temp_r5, s32 *, 0x78);
    if (temp_r0 != 0) {
        Func_08016418(temp_r0, 2);
    }
    var_r6 = 0;
    if (var_r6 < (s32) M2C_FIELD(temp_r5, s16 *, 0x8E)) {
        limit = (s16 *) ((u8 *) temp_r5 + 0x8E);
        asm volatile("" : "+r" (limit));
        var_r5 = (u8 *) temp_r5 + 0x12;
        do {
            Func_08003f3c(*(u16 *) var_r5);
            var_r6 += 1;
            var_r5 += 0x14;
        } while (var_r6 < *limit);
    }
    Func_08002dd8(0x3A);
    Func_080030f8(1);
}
