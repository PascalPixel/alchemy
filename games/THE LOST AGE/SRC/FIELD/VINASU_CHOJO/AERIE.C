#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * The top of Venus Lighthouse at the start of The Lost Age: the scene's
 * tables, the carved inscriptions the party can read, and the map cells its
 * events rearrange.
 */

enum {
    ENTRANCE_ALTERNATE_PLACEMENTS = 99
};

enum {
    MSG_RELIEF_INSCRIPTION = 0x1617,
    MSG_TRUE_ROAD_INSCRIPTION = 0x1618
};

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct ScenePlacement gScenePlacementsEntrance99[];
extern const struct SceneEvent gSceneEvents[];

/* Sets both sprite priorities of a placed object and stops automatic priority. */
void Object_SetSpritePriority(struct FieldActor *object, s32 priority)
{
    struct FieldSprite *sprite;

    if (object == 0 || object->unknown_54 == 0) {
        return;
    }
    sprite = object->sprite;
    sprite->priority = priority;
    sprite->part_priority = priority;
    object->priority_flags &= ~1;
}

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

/* Steps the actor's palette with its timer. */
s32 SceneActor_CyclePalette(struct FieldActor *actor)
{
    Object_SetPalette(actor, actor->timer & 15);
    return 0;
}

s32 SceneActor_ClearSpriteFlags(struct FieldActor *actor)
{
    Actor_SetSpriteFlags(actor, 0);
    return 0;
}

/* Arriving by entrance 99 places a different set of actors. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.entrance == ENTRANCE_ALTERNATE_PLACEMENTS) {
        return gScenePlacementsEntrance99;
    }
    return gScenePlacements;
}

/*
 * When actor 10 stands in map column 42: after half a second cue 188 plays,
 * the actor is set two pixels down, flag 0x200 is set and a block of map
 * cells is copied into place.
 */
void Scene_LowerActor10AtColumn42(void)
{
    struct FieldActor *actor;
    s32 value;
    s32 width;
    s32 height;

    actor = Actor_Get(10);
    Event_Begin();
    Event_SetMode(0);
    if (actor->x >> 20 == 42) {
        Event_Wait(30);
        Audio_PlayCue(188);
        actor->unknown_55 = 0;
        value = -0x20000;
        actor->unknown_14 = value;
        actor->y = value;
        GameFlag_Set(0x200);
        width = 3;
        height = 5;
        Map_CopyCellsTo(44, 117, 41, 117, width, height);
    }
    Event_End();
}

void Search_ReadRelief(void)
{
    Event_Begin();
    Event_SetMode(0);
    Actor_SetAnimation(Party_GetLeaderActor(), 1);
    Message_Show(MSG_RELIEF_INSCRIPTION, 1);
    Event_End();
}

void Search_ReadTrueRoadInscription(void)
{
    Event_Begin();
    Event_SetMode(0);
    Actor_SetAnimation(Party_GetLeaderActor(), 1);
    Message_Show(MSG_TRUE_ROAD_INSCRIPTION, 1);
    Event_End();
}

void SceneMap_ApplyFirstCellSet(void)
{
    s32 x;
    s32 y;
    s32 size;
    s32 one;

    x = 23;
    y = 8;
    Map_CopyCellAttributes(35, 8, 1, 3, x, y);
    size = 3;
    one = 1;
    Map_CopyCellsTo(35, 8, 23, 8, one, size);
    Map_CopyCellsTo(99, 8, 87, 8, one, size);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(57, 55, 3, 3, x, y);
    Map_CopyCellsTo(57, 55, 46, 55, size, size);
    Map_CopyCellsTo(121, 55, 110, 55, size, size);
}

void SceneMap_ApplySecondCellSet(void)
{
    s32 x;
    s32 y;
    s32 size;
    s32 one;

    x = 23;
    y = 8;
    Map_CopyCellAttributes(36, 8, 1, 3, x, y);
    size = 3;
    one = 1;
    Map_CopyCellsTo(36, 8, 23, 8, one, size);
    Map_CopyCellsTo(100, 8, 87, 8, one, size);
    x = 46;
    y = 55;
    Map_CopyCellAttributes(53, 55, 3, 3, x, y);
    Map_CopyCellsTo(53, 55, 46, 55, size, size);
    Map_CopyCellsTo(117, 55, 110, 55, size, size);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}

/* The event callback the scene installs; it does nothing. */
s32 SceneEvent_Idle(void)
{
    return 0;
}

/* The scene's last loader hook; the top of the lighthouse needs nothing. */
s32 Scene_PrepareMap(void)
{
    return 0;
}
