/* Draft, not-yet-c. Score 2026-09-26: 608 of 612 bytes, 256 differing
 * halfwords, 117 aligned edits. Typed actor/stop calls restore all pointers
 * and destination arguments; loaded -0x1000 and fixed-point random angles
 * replace the old container constants and incomplete decompiler expressions.
 * Remaining: facing pointer spills through r4 (8-byte frame instead of 4),
 * first steering-branch shape and reload registers. Separate block-local
 * facings instead allocate a 12-byte frame; that ownership trial is ruled out.
 * H1 (2026-09-27): one scene record owns its byte position and three stops;
 * one halfword direction result is reused by every snap. NEAREST_STOP.C and
 * PROMPT.C establish the 16-byte record and in/out direction contract.
 * Admission: 4-byte frame, first direction address retained across Atan2
 * without a spill; whole 612-byte owner and pool must be exact to adopt.
 * Baseline 608/612, 256 halfwords, 117 aligned edits.
 * H1 result: 604/612, 269 halfwords, 127 aligned edits. The 8-byte frame
 * and r4 direction-address spill survive; member storage also changes the
 * initial signed facing loads to ldrh. Thus the record layout is supported,
 * but a direction aggregate does not recover the missing lifetime boundary.
 * Exact siblings were read only; no adoption credit.
 * H2: restore scalar facing and expose the snap's in/out pointer across
 * steering, with a common fallback for failed leader-cone guards. H1's
 * allocator gives its generated address pseudo 166 r4: five uses over 32
 * instructions and one call. Test an explicit direction lifetime instead;
 * preserve the 4-byte/no-spill admission check and complete-owner gate.
 * H2 result: 600/612, 273 halfwords, 126 aligned edits, still 8-byte frame
 * with the direction address spilled across Atan2. Assigning the angle
 * difference before the guard folds the two signed bounds into one unsigned
 * interval, adding 0x0fff0000/0x1ffe0000 pools absent from ROM. Not admitted.
 * Stop this record/direction axis; no new DONE bytes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Stop {
    u8 id;
    u8 pos;
    u8 unknown_02[2];
};

struct StopRecord {
    u8 x;
    u8 z;
    u8 unknown_02[2];
    struct Stop stops[3];
};

struct StopWork {
    u8 unknown_000[0x182];
    s16 raised_trigger;
    u8 unknown_184[0x18];
    s16 value_19c;
};

extern struct StopWork *gStopWork;
extern u16 gBlockedFrames;

struct StopRecord *SceneData_FindEntryAtPosition(s32 *pos);
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopRecord *set, s16 *facing);
s32 SceneActor_CheckTileFreeOfKinds(struct StopRecord *pos);
void SceneActor_ApplyScaledBytePairPosition(struct FieldActor *actor, struct StopRecord *pos);
s32 Math_Atan2(s32 z, s32 x);

void KuupuappuHeya_UpdateActorStops(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    struct StopWork *work;
    struct StopRecord *entry;
    struct StopRecord *dest;
    s16 *direction;
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
        direction = &facing;
        *direction = leader->facing;
        angle = Math_Atan2(dz, dx);
        dx >>= 16;
        dz >>= 16;
        /* FAKEMATCH: failed cone guards share the actor-facing fallback. */
        if (work->value_19c > 0) {
            if (dx * dx + dz * dz > 400)
                goto actor_facing;
            angle = (u16)*direction - (u16)angle;
            if (angle <= -0x1000 || angle >= 0x1000)
                goto actor_facing;
        } else {
actor_facing:
            if (dx * dx + dz * dz > 64)
                *direction = actor->facing;
        }
        dest = KuupuappuHeya_SnapToNearestStop(entry, direction);
        if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
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
        dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(24, 2);
                goto move_first;
            }
            Engine_ObjectSetAnimation(actor, 4);
            blocked = 1;
        } else {
move_first:
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
            Engine_ObjectSetAnimation(actor, 2);
        }
    }

    actor = Engine_ActorLookup(25);
    entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
    if (entry != NULL && actor->target_x == ACTOR_NO_TARGET) {
        rnd = (u32)Engine_RandomNext() * 3 >> 16;
        rnd = (rnd * 0x30000000 - 0x30000000) >> 16;
        facing = actor->facing + rnd;
        dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
        if (SceneActor_CheckTileFreeOfKinds(dest) != 0) {
            facing = actor->facing + 0x8000;
            dest = KuupuappuHeya_SnapToNearestStop(entry, &facing);
            if (SceneActor_CheckTileFreeOfKinds(dest) == 0) {
                Engine_ActorSetAttachedEffect(25, 2);
                goto move_second;
            }
            Engine_ObjectSetAnimation(actor, 4);
            blocked += 2;
        } else {
move_second:
            SceneActor_ApplyScaledBytePairPosition(actor, dest);
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
