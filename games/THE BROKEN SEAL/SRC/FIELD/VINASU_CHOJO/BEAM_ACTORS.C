#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "VINASU.H"

/* Once the second actor stops, grow and slide the pair into their shared form. */
void VinasuChojo_UpdateBeamActors(void)
{
    struct FieldActor *first;
    struct FieldActor *second;
    s32 stopped;

    first = Engine_ActorGet(ACTOR_FIRST_OF_PAIR);
    second = Engine_ActorGet(ACTOR_SECOND_OF_PAIR);
    /* FAKEMATCH: the dead first test and the word temporaries below reproduce the
     * reference's leftover loads and stores. */
    stopped = first->target_x == ACTOR_NO_TARGET && first->target_y == first->target_x && first->target_z == first->target_y;
    if (second->target_x == ACTOR_NO_TARGET && second->target_y == second->target_x && second->target_z == second->target_y) {
        stopped = 1;
    } else {
        stopped = 0;
    }
    if (stopped != 0) {
        {
            s32 shown = 0;
        
            first->facing = shown;
        }
        {
            s32 shown = 0;
        
            second->facing = shown;
        }
        if (Engine_GameFlagIsSet(FLAG_PAIR_GROWING) != 0) {
            Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 7);
            Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 7);
            if (first->scale_x >= 0x14000) {
                goto slide;
            }
            first->scale_x += 0x200;
            first->scale_y += 0x200;
            second->scale_x += 0x200;
            second->scale_y += 0x200;
        } else {
            if ((*(s32 *)&gFrameCount & 2) != 0) {
                Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 15);
                Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 0);
            } else {
                Engine_ActorSetChildValue(ACTOR_FIRST_OF_PAIR, 0);
                Engine_ActorSetChildValue(ACTOR_SECOND_OF_PAIR, 15);
            }
        }
        slide:;
        if (Engine_GameFlagIsSet(FLAG_PAIR_SLIDING) != 0) {
            if (first->x.fixed < PIXELS(312)) {
                first->x.fixed += 0x1000;
                second->x.fixed += 0x1000;
            }
            if (first->z.fixed > PIXELS(182)) {
                first->z.fixed += -0x1000;
                second->z.fixed += -0x1000;
            }
        }
    }
}
