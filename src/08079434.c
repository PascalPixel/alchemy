typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08079434(s32 arg0, s32 arg1) {
    s32 f = 0xF;
    s32 sh = 4 & arg0;
    s32 mask = f << sh;
    u8 *base = (u8 *) 0x02000040;
    arg0 = (u32) (arg0 << 0x14) >> 0x17;
    base[arg0] = (u8) ((base[arg0] & ~mask) | ((arg1 & f) << sh));
}
