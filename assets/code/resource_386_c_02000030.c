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

void Func_02000676(s32);
void Func_02000688(s32);
void *Func_0200069a(s32);
void *Func_020006a2(s32);

void Func_02000030(void) {
    s32 temp_r3;
    s32 temp_r5;

    temp_r5 = M2C_FIELD(Func_0200069a(0), s32 *, 8);
    temp_r3 = (s32) M2C_FIELD(Func_020006a2(0), s32 *, 0x10) >> 0x14;
    if (((u32) ((temp_r5 >> 0x14) - 0x22) <= 1U) && (temp_r3 > 0x28) && (temp_r3 <= 0x2A)) {
        Func_02000676(0x250);
        return;
    }
    Func_02000688(0x250);
}
