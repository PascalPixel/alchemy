typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_0808a080(s32 actorId);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern void Func_08009098(u8 *object, const void *script);

/*
 * Attach effect 0x11d above one actor and inherit that actor's two-bit
 * presentation field.  The caller's tag is retained at +102 for the installed
 * callback at 0x0200a6f1.  Complete owner: 0x02002750-0x020027d3.
 */
void Func_02002750(s32 actorId, u16 tag)
{
    u8 *actor = Func_0808a080(actorId);
    u8 *effect;
    u8 *actorRecord;
    u8 *effectRecord;

    if (actor == 0)
        return;
    effect = Func_080090c8(0x11d, *(s32 *)(actor + 8),
                           *(s32 *)(actor + 12) + 0x002d0000,
                           *(s32 *)(actor + 16));
    if (effect == 0)
        return;

    effectRecord = *(u8 **)(effect + 80);
    Func_08009098(effect, (const void *)0x0200c15c);
    effect[85] = 0;
    *(u16 *)(effect + 100) = 0;
    *(u16 *)(effect + 102) = tag;
    *(void **)(effect + 108) = (void *)0x0200a6f1;
    effectRecord[38] = 0;

    actorRecord = *(u8 **)(actor + 80);
    effectRecord[9] = (u8)((effectRecord[9] & (u8)~0x0c) |
                           (actorRecord[9] & 0x0c));
    *(u8 **)(effect + 104) = actor;
}
