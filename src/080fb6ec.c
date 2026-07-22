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

u8 *Func_080fb6ec(s32 arg0, void *arg1) {
    u8 *cursor;
    u8 value;

    cursor = M2C_FIELD(arg1, u8 **, 0x40);
    value = *cursor;
    cursor = (u8 *)arg1 + 0x24;
    *cursor = value;
    cursor = M2C_FIELD(arg1, u8 **, 0x40);
    cursor++;
    M2C_FIELD(arg1, u8 **, 0x40) = cursor;
    return cursor;
}
