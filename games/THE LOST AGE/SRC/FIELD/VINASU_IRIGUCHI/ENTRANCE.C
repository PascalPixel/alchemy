#include "PROTO.H"
#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"



const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

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

s32 Scene_PrepareMap(void)
{
    return 0;
}
