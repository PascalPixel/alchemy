#ifndef ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H
#define ALCHEMY_UNIT_SCENE_LUNPA_FORTRESS_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          GetEmptySceneData(void);
const u32                   *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          FieldScene_DispatchActorUpdate(void);
extern s32                   gLunpaFortressRandomPick;
extern s32                   gLunpaFortressPlacements2_1[];
extern s32                   gLunpaFortressPlacements2_0[];
extern s32                   gLunpaFortressPlacements2_3[];
extern s32                   gLunpaFortressPlacements1_Data[];
extern s32                   gLunpaFortressPlacements2_2[];
extern struct SceneEntrance  gLunpaFortressEntrances1[];
extern struct SceneEntrance  gLunpaFortressEntrances2[];
extern struct SceneEntrance  gLunpaFortressEntrances3[];
extern struct SceneEntrance  gLunpaFortressEntrancesOther[];
extern u32                   gLunpaFortressExitsOther[];
extern u32                   gLunpaFortressExits2[];
extern u32                   gLunpaFortressExits3And4[];
extern struct ScenePlacement gLunpaFortressPlacementsOther[];
extern struct ScenePlacement gLunpaFortressPlacementsWestLunpaCave[];
extern struct ScenePlacement gLunpaFortressPlacements1[];
extern struct ScenePlacement gLunpaFortressPlacements2[];
extern struct ScenePlacement gLunpaFortressPlacements3[];
extern struct ScenePlacement gLunpaFortressPlacements4[];
extern struct SceneEvent     gLunpaFortressNoEvents[];
extern struct SceneEvent     gLunpaFortressEvents1[];
extern struct SceneEvent     gLunpaFortressEvents2[];
extern struct SceneEvent     gLunpaFortressEvents3[];
extern struct SceneEvent     gLunpaFortressEventsOther[];
extern u32                   gLunpaFortressPairTableA[];
extern u32                   gLunpaFortressSupportPairs[];
extern u32                   gLunpaFortressPairTableB[];
extern u32                   gLunpaFortressPairTableC[];

#endif
