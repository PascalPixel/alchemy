typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u8 *Func_08077080(u32);

s32 Func_080a735c(s32 value)
{
    u8 *entry = Func_08077080((u32)(value << 18) >> 18);
    u32 masked;

    if (entry[12] != 0)
        goto zero;
    masked = entry[1] & 0xc0;
    value = 1;
    if (masked != 0xc0)
        goto done;
zero:
    value = 0;
done:
    return value;
}
