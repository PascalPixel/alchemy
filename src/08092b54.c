typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 Func_08092054(u32);

void Func_08092b54(u32 arg0, u32 arg1)
{
    void *p;
    u8 flags;
    u32 shape;
    u32 dstAttr;
    u32 merged;

    p = (void *)Func_08092054(arg1);
    p = *(void **)((u8 *)p + 0x50);
    flags = *(u8 *)((u8 *)p + 0x1C);
    shape = *(u16 *)((u8 *)p + 0x8);

    p = (void *)Func_08092054(arg0);
    p = *(void **)((u8 *)p + 0x50);
    dstAttr = *(u16 *)((u8 *)p + 0x8);
    *(u8 *)((u8 *)p + 0x1C) = flags;
    shape <<= 22;
    shape >>= 22;
    merged = 0xfffffc00;
    merged &= dstAttr;
    merged |= shape;
    *(u16 *)((u8 *)p + 0x8) = merged;
}

void Func_08092b90(void)
{
}
