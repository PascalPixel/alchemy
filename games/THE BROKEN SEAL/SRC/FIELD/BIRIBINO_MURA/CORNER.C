#include "TYPES.H"
#include "FIELD_EVENT.H"

void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);

/* FAKEMATCH: halfword aggregates retain the short literal-pool reach. */
struct SpawnCounter {
    s16 frames;
};
extern struct SpawnCounter gCornerSpawnCounter;

/* Spawn a kind-24 effect every thirty calls while the selected actor is
 * below both coordinate limits. Only the y limit resets the counter. */
void BiribinoMura_UpdateCornerSpawn(void)
{
    struct FieldActor *actor;

    actor = Engine_ActorGet(gGameState.selected_actor);
    if (actor->x.fixed < 0x8e0000) {
        if (actor->y.fixed < 0x80000) {
            if (gCornerSpawnCounter.frames == 0)
                OverlayObject_SpawnKind24AtActor(actor);
            if (++gCornerSpawnCounter.frames == 30) {
                struct SpawnCounter zero = { 0 };
                gCornerSpawnCounter = zero;
            }
        } else {
            struct SpawnCounter *cnt = &gCornerSpawnCounter;
            struct SpawnCounter zero;
            /* FAKEMATCH: retain destination setup before the aggregate reset. */
            do { zero.frames = 0; } while (0);
            *cnt = zero;
        }
    }
}
