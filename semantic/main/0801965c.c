typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08018038(s32, s32);

s32 Func_0801965c(s32 command, s16 *destination, s32 count)
{
    u8 *state = *(u8 **)0x03001E8C;
    s16 *field;
    u32 index;
    s32 offset;

    offset = 0x12B2;
    field = (s16 *)(state + offset);
    offset = 0;
    *field = offset;
    Func_08018038(command, 1);
    count--;
    for (index = 0; index < (u32)count; index++) {
        destination[index] = *(s16 *)(state + 0xEB0 + index * 2);
        if (destination[index] == 0)
            break;
    }
    destination[index] = 0;
    return index;
}
