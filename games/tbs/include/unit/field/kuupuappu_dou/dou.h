#ifndef ALCHEMY_UNIT_FIELD_KUUPUAPPU_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_KUUPUAPPU_DOU_DOU_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
u8 *KuupuappuDou_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 KuupuappuDou_ApplyEntryState(void);
extern const s16 KuupuappuDou_DirectionSteps[];
extern const s32 KuupuappuDou_PuffScript[];
extern struct SceneEntrance gKuupuappuDouEntrancesOther[];
extern struct SceneEntrance gKuupuappuDouEntrances1[];
extern struct SceneEntrance gKuupuappuDouEntrances2[];
extern struct SceneEntrance gKuupuappuDouEntrances3[];
extern s16 gKuupuappuDouRegions[];
extern u32 gKuupuappuDouExitsOther[];
extern u32 gKuupuappuDouExits1[];
extern u32 gKuupuappuDouExits2[];
extern u32 gKuupuappuDouExits3[];
extern struct ScenePlacement gKuupuappuDouPlacementsOther[];
extern struct ScenePlacement gKuupuappuDouPlacements1[];
extern struct ScenePlacement gKuupuappuDouPlacements2[];
extern struct ScenePlacement gKuupuappuDouPlacements3[];
extern struct SceneEvent gKuupuappuDouEventsOther[];
extern struct SceneEvent gKuupuappuDouEvents1[];
extern struct SceneEvent gKuupuappuDouEvents2[];
extern struct SceneEvent gKuupuappuDouEvents3[];
extern u32 KuupuappuDou_ScheduleFrames[];
extern s32 KuupuappuDou_ScheduleTimer;
extern s32 KuupuappuDou_ScheduleIndex;
extern s32 KuupuappuDou_TickCounter;
extern s32 KuupuappuDou_TickValue;

#endif
