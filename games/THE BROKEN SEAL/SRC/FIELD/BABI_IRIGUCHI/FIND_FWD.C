#include "STAGED_ACTOR.H"

extern s32 StagedActor_DirectionSteps[];

s32 Map_GetTerrainHeight(s32 mode, s32 x, s32 z);

/* The terrain lookup consumes both computed coordinates, not just the mode.
 * Omitting x/z left their live argument registers unexplained in the draft.
 * Exact complete 72-byte owner, including both final pool words. */
struct StagedActor *BabiIriguchi_FindActorAhead(struct StagedActor *actor)
{
    s32 pos[3];
    s32 *p = pos;
    s32 step = StagedActor_DirectionSteps[actor->direction_and_kind >> 12];

    {
        s32 x = actor->x.value;
        s32 z = actor->z.value;

        x += -0x10000 & step;
        z += step << 16;
        p[0] = x;
        p[2] = z;
    }
    p[1] = Map_GetTerrainHeight(actor->transition_mode, p[0], p[2]);
    return StagedActor_FindAtTile(p, actor);
}
