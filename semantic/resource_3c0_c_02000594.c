#include "types.h"

struct DeferredDisplayWrite_02000594 {
    u32 value;
    u32 address;
    u32 mask;
};

extern void Func_080000c0(s32 frames);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a090(s32 actorId, s32 width, s32 height);
extern void Func_0808a0b8(s32 actorId, s32 x, s32 y);
extern void Func_0808a0c8(s32 actorId, s32 x, s32 y);
extern void Func_0808a0d0(s32 actorId, s32 x, s32 y);
extern void Func_0808a0e8(s32 actorId);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a100(s32 actorId, s32 direction);
extern void Func_0808a110(s32 actorId, s32 direction);
extern void Func_0808a138(s32 actorId, s32 mode);
extern void Func_0808a148(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a170(s32 messageId);
extern void Func_0808a180(s32 actorId, s32 mode);
extern void Func_0808a1b8(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a1e8(s32 actorId, s32 animationId, s32 frames);
extern void Func_080f9010(s32 resourceId);

/* Prepare the selected encounter actor while stepping the display blend, then
 * play the one-time actor-13 approach when its story flags allow it. */
void Func_02000594(s32 encounterActorId)
{
    s32 mapActorId = *(volatile s32 *)0x02000434;
    u8 *mapActor = Func_0808a080(mapActorId);
    s32 mapY = *(s16 *)(mapActor + 18);
    u8 *encounterActor = Func_0808a080(encounterActorId);
    s32 suppressBlendQueue = Func_080770c0(0x340);
    s32 mapX = *(s16 *)(mapActor + 10);
    s32 step;

    Func_0808a018();
    Func_080f9010(244);

    for (step = 1; step <= 16; step++) {
        *(s32 *)(encounterActor + 24) = step << 11;
        *(s32 *)(encounterActor + 28) = step << 12;

        if (suppressBlendQueue == 0) {
            volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
            volatile u16 *count = (volatile u16 *)0x02002090;
            u16 saved = *interruptMaster;

            *interruptMaster = (u16)(u32)interruptMaster;
            *interruptMaster = saved;
            if (*count <= 31) {
                struct DeferredDisplayWrite_02000594 *entry =
                    (struct DeferredDisplayWrite_02000594 *)
                    (0x02002094 + *count * 12);
                entry->mask = 0x02000000;
                (*count)++;
                entry->address = 0x04000052;
                entry->value = ((16 - step) << 8) | step;
            }
        }
        Func_080000c0(1);
    }

    Func_080770c8(encounterActorId + 0x1fe);
    Func_080770c8(0x340);

    if (Func_080770c0(0x9a0) != 0 && Func_080770c0(0x9b6) == 0) {
        u8 *actor;

        Func_080770c8(0x9b6);
        Func_0808a090(13, 0x10000, 0x8000);
        Func_0808a090(0, 0x18000, 0xc000);
        actor = Func_0808a080(0);
        if (actor != 0)
            Func_0808a0f0(13, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
        Func_0808a148(13, 0x4000, 0);

        actor = Func_0808a080(13);
        actor[90] &= 0xfe;
        Func_0808a0c8(13, mapX, mapY - 16);
        actor = Func_0808a080(0);
        actor[90] &= 0xfe;
        Func_0808a0d0(0, mapX + 8, mapY - 40);
        Func_0808a010(1);
        actor = Func_0808a080(0);
        actor[90] |= 1;

        Func_0808a100(13, 1);
        Func_0808a1b8(0, 0x4000, 0);
        Func_0808a170(0x262e);
        Func_0808a1e8(13, 0x106, 60);
        Func_0808a180(13, 0);
        Func_0808a1e8(13, 0x102, 60);
        Func_0808a180(13, 0);
        Func_0808a1b8(13, 0x2000, 0);
        Func_0808a1e8(13, 0x101, 60);
        Func_0808a180(13, 0);
        Func_0808a010(10);
        Func_0808a1b8(13, 0xc000, 30);
        Func_0808a138(13, 2);
        Func_0808a180(13, 0);
        Func_0808a110(13, 3);
        Func_0808a180(13, 0);
        Func_0808a090(13, 0x10000, 0x8000);
        Func_0808a100(13, 2);

        actor = Func_0808a080(0);
        if (actor != 0)
            Func_0808a0b8(13, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
        Func_0808a0e8(13);
        Func_0808a0f0(13, 0, 0);
    }
    Func_0808a020();
}
