#include "TYPES.H"
#include "FIELD_EVENT.H"

void Object_SetActionById();
s32 RamakanSabaku_EmitSandEffect(u8 *actor);
void BattleFx_SetWeightedResult();

/* Lamakan Desert: face the nearest of actors 9 to 12, then walk the leader
 * towards it with the search emote. */
void RamakanSabaku_FaceNearestActor(void)
{
    u8 *target;
    u8 *actor;
    u8 *record;
    s32 id;
    s32 best;
    s32 min;
    s32 dx;
    s32 dz;

    target = Actor_Get(gGameState.selected_actor);
    best = 9;
    GameFlag_Set(0x200);
    min = 0x100000;
    for (id = 9; id <= 12; id++) {
        u8 *other = Actor_Get(id);

        if (other != 0) {
            dx = (*(s32 *)(target + 8) - *(s32 *)(other + 8)) / 0x10000;
            dz = (*(s32 *)(target + 16) - *(s32 *)(other + 16)) / 0x10000;
            {
                s32 ax = dx;

                if (ax < 0) {
                    ax = -ax;
                }
                if (dz < 0) {
                    dz = -dz;
                }
                if (ax + dz < min) {
                    best = id;
                    min = ax + dz;
                }
            }
        }
    }
    Engine_ActorSetAnimation(0, 1);
    *((u8 *)Object_GetById(0) + 90) &= 254;
    Engine_ActorFaceActor(0, best, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(0, 0x102);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(0, 0x101);
    actor = Actor_Get(0);
    record = Object_GetById(0);
    *(u16 *)(actor + 6) = (*(u16 *)(record + 6) + 0x8000) & -0x1000;
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Actor_SetSpeed(0, 0x1999, 0xccc);
    record = Object_GetById(0);
    *(s32 *)(record + 108) = (s32)RamakanSabaku_EmitSandEffect;
    record = Actor_Get(best);
    if (record != 0) {
        Engine_ActorSetDestination(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_EventWait(60);
    Actor_ShowEmote(best, 0x104, 0);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(0, 0x100);
    *((u8 *)Object_GetById(0) + 90) |= 1;
    record = Object_GetById(0);
    *(s32 *)(record + 108) = 0;
    BattleFx_SetWeightedResult(53, 4);
}
