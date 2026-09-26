/* Draft, not-yet-c. Score 2026-09-26: 608 of 612 bytes, 256 differing
 * halfwords, 117 aligned edits. Typed actor/stop calls restore all pointers
 * and destination arguments; loaded -0x1000 and fixed-point random angles
 * replace the old container constants and incomplete decompiler expressions.
 * Remaining: facing pointer spills through r4 (8-byte frame instead of 4),
 * first steering-branch shape and reload registers. Separate block-local
 * facings instead allocate a 12-byte frame; that ownership trial is ruled out. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct StopSet;
struct StopRecord;

struct StopWork {
    u8 unknown_000[0x182];
    s16 raised_trigger;
    u8 unknown_184[0x18];
    s16 value_19c;
};

extern struct StopWork *gStopWork;
extern u16 gBlockedFrames;

u8 *SceneData_FindEntryAtPosition(s32 *pos);
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopSet *set, s16 *facing);
s32 SceneActor_CheckTileFreeOfKinds(u8 *pos);
void SceneActor_ApplyScaledBytePairPosition(s32 actor, u8 *pos);
s32 Math_Atan2(s32 z, s32 x);

void KuupuappuHeya_UpdateActorStops(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    struct StopWork *work;
    u8 *entry;
    u8 *dest;
    s32 dx;
    s32 dz;
    s32 blocked;
    s16 angle;
    s16 facing;
    u32 rnd;

    leader = Engine_ActorLookup(0);
    work = gStopWork;
    blocked = 0;
    actor = Engine_ActorLookup(2);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        dx = actor->x.fixed - leader->x.fixed;
        dz = actor->z.fixed - leader->z.fixed;
        facing = leader->facing;
        angle = Math_Atan2(dz, dx);
        dx >>= 16;
        dz >>= 16;
        if (work->value_19c > 0 && dx * dx + dz * dz <= 400
            && (s16)(facing - (u16)angle) > -0x1000
            && (s16)(facing - (u16)angle) < 0x1000) {
            /* Keep the leader's facing within the nearby forward cone. */
        } else if (dx * dx + dz * dz > 64) {
            facing = actor->facing;
        }
        dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
            SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
            Engine_ObjectSetAnimation(actor, 2);
        } else {
            Engine_ObjectSetAnimation(actor, 1);
        }
    }

    actor = Engine_ActorLookup(24);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        rnd = (u32)Engine_RandomNext() * 2 >> 16;
        rnd = (rnd * 0x60000000 - 0x30000000) >> 16;
        facing = actor->facing + rnd;
        dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(24, 2);
                goto move_first;
            }
            Engine_ObjectSetAnimation(actor, 4);
            blocked = 1;
        } else {
move_first:
            SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
            Engine_ObjectSetAnimation(actor, 2);
        }
    }

    actor = Engine_ActorLookup(25);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        rnd = (u32)Engine_RandomNext() * 3 >> 16;
        rnd = (rnd * 0x30000000 - 0x30000000) >> 16;
        facing = actor->facing + rnd;
        dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(25, 2);
                goto move_second;
            }
            Engine_ObjectSetAnimation(actor, 4);
            blocked += 2;
        } else {
move_second:
            SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
            Engine_ObjectSetAnimation(actor, 2);
        }
    }
    if (blocked != 0) {
        if (++gBlockedFrames > 29)
            work->raised_trigger = blocked + 200;
    } else {
        gBlockedFrames = blocked;
    }
}
