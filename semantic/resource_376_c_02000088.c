typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/* Complete 68-byte leaf owner through alignment and its 0x666 pool word. */
s32 Func_02000088(u8 *actor)
{
    *(s32 *)(actor + 8) += (s32)*(s16 *)(actor + 100) << 8;
    *(s32 *)(actor + 12) += (s32)*(s16 *)(actor + 102) << 8;
    *(s32 *)(actor + 24) += 0x666;
    *(s32 *)(actor + 28) += 0x666;
    *(u16 *)(actor + 100) += 5;
    *(u16 *)(actor + 102) -= 1;
    return 0;
}
