typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_0808a080(s32 actorId);
extern u32 Func_080000f8(void);
extern s32 Func_030003e0(s32 value, s32 divisor);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);

/* Spawn one randomized child effect around the selected actor. */
void Func_020031b8(s32 actorId)
{
    u8 *actor = Func_0808a080(actorId);
    u8 *child;
    u8 *actorRecord;
    u8 *childRecord;
    s32 x;
    s32 y;

    if (actor == 0)
        return;
    x = *(s32 *)(actor + 8) + (Func_030003e0(Func_080000f8(), 20) << 16)
        - 0x000a0000;
    y = *(s32 *)(actor + 12) + ((Func_080000f8() & 15) << 16)
        - 0x00080000;
    child = Func_080090c8(0x11e, x, y, *(s32 *)(actor + 16));
    if (child == 0)
        return;

    child[85] = 0;
    childRecord = *(u8 **)(child + 80);
    *(u16 *)(child + 100) = (u16)(Func_030003e0(Func_080000f8(), 10) + 5);
    *(u16 *)(child + 102) = (u16)(Func_030003e0(Func_080000f8(), 60) + 30);
    *(void **)(child + 108) = (void *)0x0200b145;
    childRecord[38] = 0;
    actorRecord = *(u8 **)(actor + 80);
    childRecord[9] = (u8)((childRecord[9] & (u8)~0x0c) |
                          (actorRecord[9] & 0x0c));
}
