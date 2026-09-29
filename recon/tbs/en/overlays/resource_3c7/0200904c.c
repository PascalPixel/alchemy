/* Draft of resource_3c7 0x0200904c..0x020091dc (400 bytes with pool),
 * Scene_Initialize, the overlay's first entry; the listing keeps the rows.
 * Remaining difference: the reference compares the scene with the sanctum's
 * and the house's ids 0xb3 and 0xb4 loaded from its literal pool, link-time
 * values; integer scenes are immediates (388 bytes, 268 differ from +0xe). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.H"

enum {
    ENTRANCE_SANCTUM_RETURN = 12,
    ENTRANCE_HOUSE_AFTER_REPORT = 21,
    ENTRANCE_FROM_AERIE = 90,
    ENTRANCE_HOUSE_REPORT = 99
};

enum {
    ACTOR_HOUSE_REPORT_SECOND = 11,
    ACTOR_HOUSE_REPORT_FIRST = 12,
    ACTOR_HOUSE_RESIDENT = 13,
    ACTOR_SANCTUM_SECOND = 18,
    ACTOR_SANCTUM_THIRD = 19,
    ACTOR_SANCTUM_FIRST = 20
};

enum {
    FLAG_AERIE_EVENTS_DONE = 0x9a7
};

/* The sanctum and the house, fixed when the overlay is linked. */

/* The action table actor 14 takes while flag 0x300 is set. */
extern const u8 gRariberoPoseAction[];

/*
 * Opens both Lalivero interiors. Arriving from the aerie by entrance 90 sets
 * flag 0x9a7. In the sanctum three actors take collision flag 4 and sprite
 * priority 2, and returning by entrance 12 records the sanctum as the scene
 * to come back to. In the house the resident is set up the same way, actor
 * 14 takes its action table while flag 0x300 is set, and arriving by
 * entrance 99 plays the report before the entrance becomes 21.
 */
s32 Scene_Initialize(void)
{
    s32 scene;
    s16 entrance;
    struct FieldActor *actor;

    if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.scene;
    if (scene == 0xb3) {
        actor = Actor_Get(ACTOR_SANCTUM_FIRST);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Actor_Get(ACTOR_SANCTUM_SECOND);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Actor_Get(ACTOR_SANCTUM_THIRD);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        Actor_SetAnimation(15, 6);
        entrance = gGameState.entrance;
        if (entrance == ENTRANCE_SANCTUM_RETURN) {
            gGameState.saved_scene = scene;
            gGameState.saved_entrance = entrance;
        }
    }
    if (gGameState.scene == 0xb4) {
        actor = Actor_Get(ACTOR_HOUSE_RESIDENT);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        if (GameFlag_IsSet(0x300) != 0) {
            Actor_EnableActionCallback(14, gRariberoPoseAction);
        }
        if (gGameState.entrance == ENTRANCE_HOUSE_REPORT) {
            Scene_RunPrimaryScript();
            Actor_SetActionCallback(Actor_Get(ACTOR_HOUSE_REPORT_FIRST), 6);
            Actor_SetActionCallback(Actor_Get(ACTOR_HOUSE_REPORT_SECOND), 6);
            gGameState.entrance = ENTRANCE_HOUSE_AFTER_REPORT;
        }
    }
    return 0;
}
