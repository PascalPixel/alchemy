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

s32 Func_0801a404(u8, s32, s32, s32, s32);
void *Func_08077080();

void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Func_0801a404(M2C_FIELD(Func_08077080(), u8 *, 4), arg1, arg2, arg3, arg4);
}
