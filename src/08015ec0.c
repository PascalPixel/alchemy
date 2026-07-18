typedef unsigned char u8;
typedef unsigned int u32;

extern u8 *Data_03001e8c;

void Func_08015ec0(u32 arg0)
{
    u8 *base = Data_03001e8c;
    if (arg0 >= (u32)(base + 0x698) && arg0 < (u32)(base + 0xd98)) {
        u32 old = *(u32 *)(base + 0xd9c);
        *(u32 *)(base + 0xd9c) = arg0;
        *(u32 *)old = arg0;
        *(u32 *)arg0 = 0;
    }
}
