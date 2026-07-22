typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

s32 Func_080022ec(s32, s32);

void Func_0809a65c(void *arg0) {
    u8 *a = arg0;
    s32 a44 = *(volatile s32 *)(a + 0x44);
    s32 a4c;

    *(volatile s32 *)(a + 0x08) += a44;
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    a4c = *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x10) += a4c;
    *(volatile s32 *)(a + 0x44) = a44 - Func_080022ec(a44, 0x12);
    *(volatile s32 *)(a + 0x4C) = a4c - a4c / 16;
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}
