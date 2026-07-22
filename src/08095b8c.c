typedef unsigned char u8;
typedef unsigned int u32;

extern u32 Data_03001800;
extern u32 Data_0809f0a4[];

void Func_08095b8c(u8 *object)
{
    u32 *table = Data_0809f0a4;
    u32 index = (Data_03001800 >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}
