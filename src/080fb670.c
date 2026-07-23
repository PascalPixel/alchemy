typedef unsigned char u8;
typedef signed int s32;

void Func_080072ec(s32 arg0, s32 arg1, s32 arg2);

void Func_080fb670(s32 arg0, void *arg1)
{
    u8 *cursor;
    s32 value;
    s32 address;

    cursor = *(u8 **)((u8 *)arg1 + 0x40);
    value = *cursor;
    *(u8 **)((u8 *)arg1 + 0x40) = cursor + 1;
    address = 0x080fba48 + (value << 2);
    Func_080072ec(arg0, (s32)arg1, *(s32 *)address);
}
