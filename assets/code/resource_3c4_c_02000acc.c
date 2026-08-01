typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void Func_02000acc(void *arg0, int unused, u32 arg2, u32 arg3) {
    u8 *base = arg0;
    u32 increment;
    u32 value;
    *(u32 *)(base + 0x18) = arg3 + arg2;
    increment = *(u32 *)(base + 0x34);
    value = *(u32 *)(base + 0x1c);
    *(u32 *)(base + 0x1c) = value + increment;
    *(u16 *)(*(u8 **)(base + 0x50) + 0x1e) += *(u16 *)(base + 0x64);
}
