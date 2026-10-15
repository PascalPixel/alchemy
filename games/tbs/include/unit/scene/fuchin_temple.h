#ifndef ALCHEMY_UNIT_SCENE_FUCHIN_TEMPLE_H
#define ALCHEMY_UNIT_SCENE_FUCHIN_TEMPLE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          GetXianInitialState(void);
s32                          GetXianMessageData(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          FuchinTemple_ApplyEntryState(void);
extern s32                   gEffectScript0[];
extern s32                   gEffectScript1[];
extern s32                   gEffectScript2[];
extern const s32            *gEffectScripts[];
extern s32                   gFuchinTemple_ActorTwelveScript[];
extern s32                   gFuchinTemplePlacement1[];
extern u16                   gFuchinTemple_CellStepsA[];
extern u16                   gFuchinTemple_CellStepsB[];
extern u16                   gFuchinTemple_CellStepsC[];
extern struct SceneEntrance  gFuchinTempleEntrances1[];
extern struct SceneEntrance  gFuchinTempleEntrancesOther[];
extern u32                   gFuchinTemple_XianMessages[];
extern struct ScenePlacement gFuchinTemplePlacements1[];
extern struct ScenePlacement gFuchinTemplePlacements[];
extern struct ScenePlacement gFuchinTemplePlacementsEntrance3[];
extern struct SceneEvent     gFuchinTempleEvents1[];
extern struct SceneEvent     gFuchinTempleEvents[];
extern struct SceneEvent     gFuchinTempleEventsEntrance3[];
extern s32                   gFuchinTemple_ShotScript[];

#endif
