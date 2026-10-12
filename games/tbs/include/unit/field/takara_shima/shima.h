#ifndef ALCHEMY_UNIT_FIELD_TAKARA_SHIMA_SHIMA_H
#define ALCHEMY_UNIT_FIELD_TAKARA_SHIMA_SHIMA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u16 TakaraShima_EntranceCells[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gTakaraShimaEntrances1[];
extern struct SceneEntrance gTakaraShimaEntrances2[];
extern struct SceneEntrance gTakaraShimaEntrances3[];
extern struct SceneEntrance gTakaraShimaEntrances4[];
extern u16 gTakaraShimaEntrances5[];
extern struct SceneEntrance gTakaraShimaEntrancesOther[];
extern u32 gTakaraShimaExits[];
extern struct ScenePlacement gTakaraShimaPlacementsOther[];
extern struct ScenePlacement gTakaraShimaPlacements1[];
extern struct ScenePlacement gTakaraShimaPlacements3[];
extern struct ScenePlacement gTakaraShimaPlacements6To14[];
extern struct SceneEvent gTakaraShimaEventsOther[];
extern struct SceneEvent gTakaraShimaEvents2[];
extern struct SceneEvent gTakaraShimaEvents1[];
extern struct SceneEvent gTakaraShimaEvents3[];
extern struct SceneEvent gTakaraShimaEvents4[];
extern struct SceneEvent gTakaraShimaEvents5[];

#endif
