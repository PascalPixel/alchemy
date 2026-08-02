typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080770e8(s32 counterId, s32 value);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a0a8(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a250(s32 resourceId, s32 variant);

/* Close the actor-selected encounter, transfer the scene-specific flag, and
 * mark the current map actor as returned to its ordinary state. */
void Func_02000b24(s32 unused, s32 encounterActorId)
{
    u8 *mapState = *(u8 *volatile *)0x03001ebc;
    s16 sceneId;
    s32 mapActorId;
    u8 *mapActor;
    (void)unused;

    if (*(s16 *)(mapState + 0x182) == 99)
        *(u16 *)(mapState + 0x182) = 0;
    Func_080770d0(0x20f);

    sceneId = *(volatile s16 *)0x02000400;
    if (sceneId == 0xa4)
        Func_080770c8(encounterActorId + 0x2f9);
    else if (sceneId == 0xa5)
        Func_080770c8(encounterActorId + 0x309);

    Func_080770e8(0x210, 0);
    Func_0808a250(98, 5);
    *(volatile u8 *)0x0200046b = 3;

    if (sceneId == 0xa5) {
        if (encounterActorId == 11) {
            Func_0808a250(98, 7);
        } else if (encounterActorId == 12) {
            Func_0808a250(98, 6);
            Func_0808a0a8(12);
            Func_0808a0f0(12, 0, 0);
        }
    }

    mapActorId = *(volatile s32 *)0x02000434;
    mapActor = Func_0808a080(mapActorId);
    mapActor[85] = 3;
}
