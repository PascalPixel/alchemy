typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001ea8;

extern void Func_08097948(s32 angle, s32 *out1, s32 *out2, s32 *out3);

void Func_080978c4(void)
{
    u8 *base = Data_03001ea8;
    s32 out1 = 0;
    s32 out2 = 0;
    s32 out3 = 0;
    s32 offset;
    u8 *p0;
    u8 *p1;
    u8 *p2;

    Func_08097948((s32)(*(u16 *)(base + 0x28E)) << 16, &out1, &out2, &out3);

    offset = 0x28B;
    p0 = base + offset;
    *p0 = (u8)((out1 >> 18) + 4);
    offset += 1;
    p1 = base + offset;
    *p1 = (u8)((out2 >> 18) + 4);
    offset += 1;
    p2 = base + offset;
    *p2 = (u8)((out3 >> 18) + 4);

    *(u16 *)(base + 0x28E) += 4;

    *p0 &= 0x1F;
    *p1 &= 0x1F;
    *p2 &= 0x1F;

    if (*(u16 *)(base + 0x28E) >= 360) {
        *(u16 *)(base + 0x28E) = 0;
    }
}
