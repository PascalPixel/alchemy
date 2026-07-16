typedef signed int s32;
typedef unsigned int u32;

u32 Func_08079bc4(void)
{
    s32 value;
    value = (*(s32 *)0x020023a8 * 0x41c64e6d) + 0x3039;
    *(s32 *)0x020023a8 = value;
    return (u32)(value << 8) >> 16;
}
