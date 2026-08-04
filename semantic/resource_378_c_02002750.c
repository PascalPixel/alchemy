typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_02005d6a(s32 actorId);
extern u8 *Func_02005ce2(s32 kind, s32 x, s32 y, s32 z);
extern void Func_02005ce8(u8 *object, const void *script);

/*
 * Attach effect 0x11d above one actor and inherit that actor's two-bit
 * presentation field.  The caller's tag is retained at +102 for the installed
 * callback at 0x0200a6f1.  Complete owner: 0x02002750-0x020027d3.
 */
void Func_02002750(s32 actorId, s32 tag)
{
    u8 *actor = Func_02005d6a(actorId);
    u8 *effect;
    u8 *actorRecord;
    u8 *effectRecord;
    u8 mixed;

    if (actor == 0)
        return;
    effect = Func_02005ce2(0x11d, *(s32 *)(actor + 8),
                           *(s32 *)(actor + 12) + 0x002d0000,
                           *(s32 *)(actor + 16));
    if (effect == 0)
        return;

    effectRecord = *(u8 **)(effect + 80);
    Func_02005ce8(effect, (const void *)0x0200c15c);
    {
        u8 *cursor = effect + 85;
        *cursor = 0;
        cursor += 15;
        *(u16 *)cursor = 0;
        cursor += 2;
        *(u16 *)cursor = (u16)tag;
    }
    *(void **)(effect + 108) = (void *)0x0200a6f1;
    effectRecord[38] = 0;

    actorRecord = *(u8 **)(actor + 80);
    mixed = (u8)((actorRecord[9] & 0x0c) |
                 (effectRecord[9] & (u8)~0x0c));
    *(u8 **)(effect + 104) = actor;
    effectRecord[9] = mixed;
}
