#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "field_event.h"
#include "party_state.h"

#define ObjectDispatch_SetSingleChildFlags \
    ((void (*)(void *, s32))ObjectDispatch_SetSingleChildFlags)

#define Object_SetPartAttribute ((void (*)(void *, s32))Object_SetPartAttribute)

enum
{
    ENTRANCE_ALTERNATE_PLACEMENTS = 99
};

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
