typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern void Func_080000c0(s32 frames);
extern void Func_080770c8(s32 flagId);
extern s32 Func_080770e0(s32 counterId);
extern void Func_080091e0(u8 *actor, s32 mode);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a210(s32 value, s32 direction, s32 distance, s32 enabled);
extern void Func_0808a238(s32 mode, s32 value);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 resourceId);

/* Fade the selected encounter actor and current map actor together for sixty
 * frames, then select the scene-specific presentation result. */
void Func_02000c00(void)
{
    s32 encounterActorId = Func_080770e0(0x218);
    s32 mapActorId = *(volatile s32 *)0x02000434;
    u8 *mapActor = Func_0808a080(mapActorId);
    u8 *encounterActor = Func_0808a080(encounterActorId);
    s32 frame;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080f9010(219);
    Func_080091e0(mapActor, 0);
    encounterActor[85] = 0;
    mapActor[85] = 0;
    *(s32 *)(mapActor + 40) = 0;
    mapActor[97] = 1;
    encounterActor[97] = 1;

    for (frame = 0; frame < 60; frame++) {
        *(s32 *)(mapActor + 40) += 0x3333;
        *(s32 *)(encounterActor + 40) += 0x3333;
        Func_080000c0(1);
    }

    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
    Func_080770c8(0x122);

    if (*(volatile s16 *)0x02000400 == 0xa5 &&
        Func_080770e0(0x218) == 11) {
        Func_0808a238(2, 77);
    } else {
        Func_0808a238(2, 27);
    }
}
