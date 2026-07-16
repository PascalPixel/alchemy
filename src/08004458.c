typedef signed int s32;
typedef unsigned int u32;

u32 Func_08004458(void)
{
    s32 value;
    value = (*(s32 *)0x03001cb4 * 0x41c64e6d) + 0x3039;
    *(s32 *)0x03001cb4 = value;
    return (u32)(value << 8) >> 16;
}
