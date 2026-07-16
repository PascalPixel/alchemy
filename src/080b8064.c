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

s32 Func_08009080(void *, s32);
s32 Func_08009140(void *);
s32 Func_08009150(void *, s32, s32, s32);
void *Func_080b7dd0();

void Func_080b8064(void) {
    void *temp_r0;
    void *temp_r5;

    temp_r0 = Func_080b7dd0();
    temp_r5 = M2C_FIELD(temp_r0, void **, 0);
    M2C_FIELD(temp_r5, s32 *, 0x34) = 0x20000;
    M2C_FIELD(temp_r5, s32 *, 0x30) = 0x80000;
    M2C_FIELD(temp_r5, s32 *, 0x28) = 0x50000;
    M2C_FIELD(temp_r5, s32 *, 0x48) = 0x7851;
    M2C_FIELD(temp_r5, s32 *, 0x44) = 0;
    M2C_FIELD(temp_r5, s8 *, 0x5A) = 0;
    Func_08009140(temp_r5);
    Func_08009150(temp_r5, M2C_FIELD(temp_r0, s32 *, 0xC) * 3, 0, M2C_FIELD(temp_r0, s32 *, 0x10));
    Func_08009080(temp_r5, 1);
}
