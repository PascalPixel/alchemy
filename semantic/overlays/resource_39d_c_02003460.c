typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_08000120(s32 angle);
extern s32 Func_08000118(s32 angle);

/* Advance an orbiting effect around the object published at +104. */
void Func_02003460(u8 *object)
{
    u8 *anchor = *(u8 **)(object + 104);
    u16 angle = *(u16 *)(object + 100);
    s32 radius = *(s32 *)(object + 48) + 28;
    s32 x = *(s32 *)(anchor + 8) + radius * Func_08000120(angle);
    s32 z = 0x00900000 + (Func_08000118(angle) << 4);

    *(s32 *)(object + 8) = x;
    *(s32 *)(object + 16) = z;
    *(s32 *)(object + 56) = x;
    *(s32 *)(object + 64) = z;
    *(u16 *)(object + 100) = angle - 0x200;
}
