typedef unsigned char u8;
typedef unsigned int u32;

struct Entries {
    u8 values[64];
    u32 data[65];
};

u32 Func_080bbabc(u32 value, u32 data)
{
    u8 *base = *(u8 **)0x03001e74;
    struct Entries *entries = (struct Entries *)(base + 1720);
    u32 *count = (u32 *)(base + 2044);
    u32 index = *count;

    entries->values[index] = value;
    entries->data[index] = data;
    *count = index + 1;
    return value;
}
