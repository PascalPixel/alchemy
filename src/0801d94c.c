typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

extern u8 *Data_03001ea0;
void Func_080b0020(void *);
void Func_080217a4(void *);

void Func_0801d94c(void)
{
    u8 *base = Data_03001ea0;
    u32 index;

    Func_080b0020(base + 0x5a4);
    index = *(u16 *)(base + 0x574);
    index *= 4;
    index += 0x610;
    Func_080217a4(*(void **)(base + index));
}
