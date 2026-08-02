typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770e8(s32 counterId, s32 value);
extern void Func_080000d0(u8 *address, s32 size);
extern void Func_08009150(u8 *actor, s32 x, s32 height, s32 y);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a0b8(s32 actorId, s32 x, s32 y);
extern void Func_0808a0e8(s32 actorId);
extern void Func_0808a100(s32 actorId, s32 direction);
extern void Func_0808a1f0(s32 actorId, s32 animationId);
extern void Func_080f9010(s32 resourceId);

/* Activate one encounter actor relative to the current map actor, initialize
 * its vertical motion, and publish the encounter state exactly once. */
void Func_02000464(s32 encounterActorId)
{
    s32 mapActorId = *(volatile s32 *)0x02000434;
    u8 *mapActor = Func_0808a080(mapActorId);
    u8 *encounterActor = Func_0808a080(encounterActorId);

    if (Func_080770c0(0x20f) != 0)
        return;

    Func_0808a018();
    Func_0808a1f0(mapActorId, 0x101);
    Func_0808a100(mapActorId, 9);

    encounterActor = Func_0808a080(encounterActorId);
    if (encounterActor != 0) {
        Func_0808a0b8(mapActorId,
            *(s16 *)(encounterActor + 10), *(s16 *)(encounterActor + 18));
    }
    Func_0808a0e8(mapActorId);
    Func_080f9010(244);
    Func_080000d0((u8 *)0x02008401, 3200);

    mapActor[85] = 0;
    Func_08009150(mapActor,
        *(s32 *)(mapActor + 8), *(s32 *)(mapActor + 12) + 0x200000,
        *(s32 *)(mapActor + 16));
    Func_0808a0e8(mapActorId);
    *(s32 *)(mapActor + 40) = 0;
    mapActor[85] = 4;
    *(volatile u8 *)0x02000432 = 2;

    Func_080770c8(0x20f);
    Func_080770e8(0x218, encounterActorId);
    Func_080770e8(0x210, 180);
    Func_0808a020();
    *(volatile u16 *)(*(u8 *volatile *)0x03001ebc + 0x17c) = 0;
}
