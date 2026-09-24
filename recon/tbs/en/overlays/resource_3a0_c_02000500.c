/* NONMATCHING: undefined of undefined bytes, undefined halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: every instruction and the pool match (154 bytes); the owner's 156-byte extent ends in a 2-byte alignment gap, and alchemy overlay adopt reports exact in 1 of 30 compiles, so it refuses; register the tail as a compiler gap (or diagnose the layout split) and adopt this as is. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneActor_UpdateTracking(struct FieldActor *actor, struct FieldActor *leader, s32 range, s32 near);

/* Counts the frames the leader stands at the gate beside this actor and raises trigger 200 after two seconds. */
s32 Local_02000500(struct FieldActor *self)
{
    struct FieldActor *leader;
    s32 near = 0;

    if (self->target_x == ACTOR_NO_TARGET && self->target_z == ACTOR_NO_TARGET)
        return 0;
    leader = Engine_ActorGet(0);
    if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 && self->x.fixed >> 20 <= 19
        && self->velocity_x <= 0) {
        if (leader->x.fixed <= self->x.fixed) {
            self->rise_counter++;
            near = 1;
        }
    } else {
        self->rise_counter = 0;
    }
    if (near && self->rise_counter > 119) {
        gEventWork->raised_trigger = 200;
        self->rise_counter = 0;
    }
    SceneActor_UpdateTracking(self, leader, 18, near);
    return 0;
}
