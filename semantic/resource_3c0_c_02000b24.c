#include "types.h"









/* Close the actor-selected encounter, transfer the scene-specific flag, and
 * mark the current map actor as returned to its ordinary state. */
extern void Func_02001d9e(s32 flagId);
extern void Func_02001db0(s32 flagId);
extern void Func_02001dc0(s32 flagId);
extern void Func_02001de2(s32 counterId, s32 value);
extern void Func_02001eba(s32 resourceId, s32 variant);
extern void Func_02001ee2(s32 resourceId, s32 variant);
extern void Func_02001ef0(s32 resourceId, s32 variant);
extern void Func_02001e5e(s32 actorId);
extern void Func_02001e90(s32 actorId, s32 x, s32 y);
extern u8 * Func_02001e5c(s32 actorId);
void Func_02000b24(s32 unused, s32 encounterActorId)
{
    u8 *mapState = *(u8 *volatile *)0x03001ebc;
    s16 sceneId;
    s32 mapActorId;
    u8 *mapActor;
    (void)unused;

    if (*(s16 *)(mapState + 0x182) == 99)
        *(u16 *)(mapState + 0x182) = 0;
    Func_02001d9e(0x20f);

    sceneId = *(volatile s16 *)0x02000400;
    if (sceneId == 0xa4)
        Func_02001db0(encounterActorId + 0x2f9);
    else if (sceneId == 0xa5)
        Func_02001dc0(encounterActorId + 0x309);

    Func_02001de2(0x210, 0);
    Func_02001eba(98, 5);
    *(volatile u8 *)0x0200046b = 3;

    if (sceneId == 0xa5) {
        if (encounterActorId == 11) {
            Func_02001ee2(98, 7);
        } else if (encounterActorId == 12) {
            Func_02001ef0(98, 6);
            Func_02001e5e(12);
            Func_02001e90(12, 0, 0);
        }
    }

    mapActor[85] = 3;
    mapActorId = *(volatile s32 *)0x02000434;
    mapActor = Func_02001e5c(mapActorId);
}
