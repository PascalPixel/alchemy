#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* The approach to Venus Lighthouse: the scene's tables and its particle trail. */

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

void ParticleTrail_Spawn(struct FieldActor *actor, s32 x, s32 y, s32 z);

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

/* This scene declares no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

/* Leaves a particle at the actor each frame and plays cue 125 every eighth. */
void Actor_UpdateParticleTrail(struct FieldActor *actor)
{
    ParticleTrail_Spawn(actor, actor->x, actor->y, actor->z);
    if ((++actor->timer % 8) == 0) {
        Audio_PlayCue(125);
    }
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}

/* The scene's last loader hook; the approach needs nothing. */
s32 Scene_PrepareMap(void)
{
    return 0;
}
