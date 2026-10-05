#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"

/* The opening's cast arrives through this entrance and stands apart. */
enum {
    ENTRANCE_ALTERNATE_PLACEMENTS = 99
};

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];

void ObjectDispatch_SetSingleChildField26(void *object, s32 value);
void Object_SetPartAttribute(void *object, s32 value);

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

/* Takes the low four bits of an actor's counter as its part attribute. */
s32 SceneActor_ApplyCounterLowBitsAsMode(struct FieldActor *actor)
{
    Object_SetPartAttribute(actor, actor->timer & 15);
    return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor)
{
    ObjectDispatch_SetSingleChildField26(actor, 0);
    return 0;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gPartyState.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
        return gScenePlacementsEntrance99;
    return gScenePlacements;
}
