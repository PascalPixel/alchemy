/* shared family owner 0x020008c0-0x020009db: kind-selected rectangle redraw. */
typedef int s32; typedef short s16; typedef unsigned char u8;
u8 *Func_0808a080(s32);
void Func_080091c0(s32,s32,s32,s32,s32,s32);
s32 Func_02000244(s32,s32,s32,s32,s32,s32);
s32 Func_020008c0(s32 slot)
{
    u8 *runtime = *(u8 **)0x03001e70;
    u8 *object = Func_0808a080(slot);
    s16 kind = **(s16 **)(*(u8 **)(object + 80) + 40);
    s32 *keys = (s32 *)0x0200b230, *delta;
    s32 index, a0, a1, a2, a3, width, depth, tileX, tileZ, globalX, globalZ;
    for (index = 0; index < 6; index++) if (kind == keys[index]) break;
    if (index == 6) return 0;
    delta = (s32 *)0x0200b248 + index * 4;
    a0 = delta[0] < 0 ? -delta[0] : delta[0];
    a1 = delta[1] < 0 ? -delta[1] : delta[1];
    a2 = delta[2] < 0 ? -delta[2] : delta[2];
    a3 = delta[3] < 0 ? -delta[3] : delta[3];
    width = (a0 + a2) >> 4; depth = (a1 + a3) >> 4;
    tileX = (*(s32 *)(object + 8) + (delta[0] << 16)) >> 20;
    tileZ = (*(s32 *)(object + 16) + (delta[1] << 16)) >> 20;
    globalX = (*(s32 *)(runtime + 316) >> 20) + tileX;
    globalZ = (*(s32 *)(runtime + 320) >> 20) + tileZ;
    Func_080091c0(tileX, tileZ, width, depth, globalX, globalZ);
    Func_02000244(0, tileX, tileZ, width, depth, 255);
    Func_02000244(2, tileX, tileZ, width, depth, 255);
    return 1;
}
