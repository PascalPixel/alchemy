typedef signed short s16;
typedef signed int s32;

struct Table {
    s16 header[32];
    s16 entries[1][33];
};

extern struct Table Data_080b41ac;

s32 Func_080b2764(s32 index)
{
    return Data_080b41ac.entries[index][0];
}
