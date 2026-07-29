typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_08006a00(s32);
void Func_08006a78(void);
s32 Func_080072e8(s32, s32);

s32 Func_08006af8(u8 value, s32 argument, u8 expected) {
    u32 packed;
    s32 result;

    packed = value;
    result = 0;
    Func_08006a00(value);
    packed = (packed | 0xC000) << 16;

    goto loop;

failure:
    if (*(u16 *)(*(u8 **)0x02004C08 + 20) == 0x1CC2)
        *(u8 *)0x0E005555 = 0xF0;
    result = packed >> 16;
    goto done;

loop:
    if ((u8)Func_080072e8(argument, *(s32 *)0x02004C1C) == expected)
        goto done;
    if (*(u8 *)0x02004C24 == 0)
        goto loop;
    if ((u8)Func_080072e8(argument, *(s32 *)0x02004C1C) != expected)
        goto failure;

done:
    Func_08006a78();
    return result;
}
