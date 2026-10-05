#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"

enum {
    ENTRANCE_ALTERNATE_PLACEMENTS = 99
};


void ObjectDispatch_SetSingleChildFlags(void *object, s32 value);
void Object_SetPartAttribute(void *object, s32 value);

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

s32 SceneActor_ApplyCounterLowBitsAsMode(struct FieldActor *actor)
{
    Object_SetPartAttribute(actor, actor->timer & 15);
    return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor)
{
    ObjectDispatch_SetSingleChildFlags(actor, 0);
    return 0;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gPartyState.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
        return gScenePlacementsEntrance99;
    return gScenePlacements;
}
