#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *other);
void TakaraHashira_SyncPriorityIfAhead(struct FieldActor *actor, struct FieldActor *other);

/* The sprite read through an s32 view of actor->sprite, so the pointer reloads
 * after each bitfield store. */
#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

s32 TakaraHashira_UpdateActorPriority(struct FieldActor *actor)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    s32 hit;
    u32 id;

    if (((s8 *)gEventWork)[0xcc7] == 1) {
        actor->sprite->priority = leader->sprite->priority;
        actor->collision_flags |= 1;
        return 0;
    }
    hit = TakaraHashira_SyncPriorityIfBehind(actor, leader);
    for (id = 8; id <= 11; id++) {
        hit += TakaraHashira_SyncPriorityIfBehind(actor, Engine_ActorGet(id));
    }
    if (hit != 0) {
        for (id = 8; id <= 11; id++) {
            TakaraHashira_SyncPriorityIfAhead(actor, Engine_ActorGet(id));
        }
    }
    if (actor->y.fixed < leader->y.fixed) {
        actor->priority_flags |= 2;
        actor->collision_flags &= 254;
        if (actor->sprite->priority < leader->sprite->priority) {
            actor->priority_flags &= 254;
            SPRITE_OF(actor)->priority = SPRITE_OF(leader)->priority;
            SPRITE_OF(actor)->second_priority = SPRITE_OF(leader)->second_priority;
            hit = 1;
        }
    } else {
        actor->priority_flags &= 253;
        actor->collision_flags |= 1;
    }
    if (hit == 0) {
        actor->priority_flags |= 1;
    }
    return 0;
}
