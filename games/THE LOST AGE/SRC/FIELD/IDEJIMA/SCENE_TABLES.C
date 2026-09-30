#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* Idejima's tables: where the party appears, the exits, the placed actors,
   the events for each entrance, and its map regions. */

enum {
    ENTRANCE_WAKE = 2,
    ENTRANCE_WAKE_AGAIN = 4
};

extern const struct SceneEntrance gIdejimaEntrances[];
extern const u32 gIdejimaExits[];
extern const struct ScenePlacement gIdejimaPlacements[];
extern const struct SceneEvent gIdejimaEvents[];
extern const struct SceneEvent gIdejimaEventsEntrance1[];
/* The search events that start the wake-up scenes. */
extern const struct SceneEvent gIdejimaEventsWake[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gIdejimaEntrances;
}

/* Idejima declares no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gIdejimaExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gIdejimaPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    switch (gPartyState.entrance) {
    case 1:
    case 3:
        return gIdejimaEventsEntrance1;
    case ENTRANCE_WAKE:
    case ENTRANCE_WAKE_AGAIN:
        return gIdejimaEventsWake;
    default:
        return gIdejimaEvents;
    }
}
