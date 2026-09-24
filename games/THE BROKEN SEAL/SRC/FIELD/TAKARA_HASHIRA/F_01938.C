#include "TYPES.H"
#include "FIELD_EVENT.H"

#define SPRITE_OF(actor) ((struct FieldSprite *)*(s32 *)((u8 *)(actor) + 0x50))

/* Whether actor stands directly in front of front (the mirror of
 * TakaraHashira_Func020019f0); when front draws in a higher sprite priority
 * number, actor takes front's priorities. */
s32 TakaraHashira_Func02001938(struct FieldActor *actor, struct FieldActor *front)
{
    s32 result = 0;

    if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed
        && front->z.fixed == actor->z.fixed) {
        return result;
    }
    if (front->x.fixed - 0x100000 < actor->x.fixed && actor->x.fixed < front->x.fixed + 0x100000
        && front->y.fixed / 0x10000 == actor->y.fixed / 0x10000
        && front->z.fixed > actor->z.fixed && front->z.fixed - 0x200000 < actor->z.fixed) {
        if (SPRITE_OF(actor)->priority < SPRITE_OF(front)->priority) {
            actor->priority_flags &= ~1;
            SPRITE_OF(actor)->priority = SPRITE_OF(front)->priority;
            SPRITE_OF(actor)->second_priority = SPRITE_OF(front)->second_priority;
        }
        result = 1;
    }
    return result;
}
