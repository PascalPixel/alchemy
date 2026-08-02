typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001e70;
extern u8 *Func_0808a080(s32 id);
extern void Func_080091c0();
extern void Func_02000528(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 tile);

/* Paint both map layers beneath a recognised footprint-class actor. */
s32 Func_02000ba4(s32 actorId)
{
    const s32 *classIds = (const s32 *)0x0200a740;
    const s32 (*bounds)[4] = (const s32 (*)[4])0x0200a758;
    u8 *actor = Func_0808a080(actorId);
    s16 type = *(s16 *)(*(u8 **)(actor + 80) + 40);
    s32 kind;
    s32 x;
    s32 z;
    s32 width;
    s32 height;

    for (kind = 0; kind < 6 && type != classIds[kind]; kind++) {
    }
    if (kind == 6)
        return 0;

    x = (*(s32 *)(actor + 8) + (bounds[kind][0] << 16)) >> 20;
    z = (*(s32 *)(actor + 16) + (bounds[kind][1] << 16)) >> 20;
    width = ((bounds[kind][0] < 0 ? -bounds[kind][0] : bounds[kind][0]) +
             (bounds[kind][2] < 0 ? -bounds[kind][2] : bounds[kind][2])) >> 4;
    height = ((bounds[kind][1] < 0 ? -bounds[kind][1] : bounds[kind][1]) +
              (bounds[kind][3] < 0 ? -bounds[kind][3] : bounds[kind][3])) >> 4;
    Func_080091c0(x, z, width, height,
                   x + (*(s32 *)(Data_03001e70 + 316) >> 20),
                   z + (*(s32 *)(Data_03001e70 + 320) >> 20));
    Func_02000528(0, x, z, width, height, 255);
    Func_02000528(2, x, z, width, height, 255);
    return 1;
}
