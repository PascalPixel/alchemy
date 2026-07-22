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

s16 Func_02003588(s32, s32);
void *Func_0200364e(s16);

s32 Func_02000030(void *arg0) {
    void *temp_r0;

    temp_r0 = Func_0200364e(M2C_FIELD(arg0, s16 *, 0x64));
    M2C_FIELD(arg0, s16 *, 6) = Func_02003588(M2C_FIELD(temp_r0, s32 *, 0x10) - M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(temp_r0, s32 *, 8) - M2C_FIELD(arg0, s32 *, 8));
    return 0;
}
