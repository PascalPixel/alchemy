typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};

extern struct Entry Data_080c7420[];

s32 Func_080c23c0(s32 index)
{
    s32 result;

    if ((u32)index > 171)
        return 0;
    result = 0;
    if ((u32)Data_080c7420[index].flags0 << 31)
        result = 1;
    return result;
}
