typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_030001d8(s32);

s32 Func_02001638(const u8 *a, const u8 *b)
{
    s32 dx = (*(const s32 *)(a + 0) - *(const s32 *)(b + 0)) >> 16;
    s32 dy = (*(const s32 *)(a + 4) - *(const s32 *)(b + 4)) >> 16;
    s32 dz = (*(const s32 *)(a + 8) - *(const s32 *)(b + 8)) >> 16;
    return Func_030001d8(dx * dx + dy * dy + dz * dz);
}
