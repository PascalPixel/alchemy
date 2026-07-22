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

void Func_020001cc();
s32 Func_02000bb4(s32, s32);
void Func_02000c8a(s32);
void Func_020010e4(s32, s32, s32, s32, s32, s32);

void Func_02000054(void) {
    Func_02000c8a(*(s32 *)0x020092C0);
    Func_020010e4(0, 0x40, 0x20, 0x20, 0, 0);
    Func_02000bb4(*(s32 *)0x020092C0, 0xFF);
    Func_020001cc();
}
