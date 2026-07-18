typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Entry {
    u16 value;
    u8 rest[6];
};

extern struct Entry Data_080c7420[];

s32 Func_080c2384(s32 index)
{
    if ((u32)index > 171)
        return Data_080c7420[0].value;
    return Data_080c7420[index].value;
}
