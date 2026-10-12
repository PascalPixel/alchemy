#ifndef ALCHEMY_UNIT_FIELD_COMMON_SHIAN_JIIN_JIIN_H
#define ALCHEMY_UNIT_FIELD_COMMON_SHIAN_JIIN_JIIN_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 GetXianInitialState(void);
s32 GetXianMessageData(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ShianJiin_ApplyEntryState(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 ShianJiin_ActorTwelveScript[];
extern s32 gShianJiinPlacement1[];
extern u16 ShianJiin_CellStepsA[];
extern u16 ShianJiin_CellStepsB[];
extern u16 ShianJiin_CellStepsC[];
extern struct SceneEntrance gShianJiinEntrances1[];
extern struct SceneEntrance gShianJiinEntrancesOther[];
extern u32 ShianJiin_XianMessages[];
extern struct ScenePlacement gShianJiinPlacements1[];
extern struct ScenePlacement gShianJiinPlacements[];
extern struct ScenePlacement gShianJiinPlacementsEntrance3[];
extern struct SceneEvent gShianJiinEvents1[];
extern struct SceneEvent gShianJiinEvents[];
extern struct SceneEvent gShianJiinEventsEntrance3[];
extern s32 ShianJiin_ShotScript[];

#endif
