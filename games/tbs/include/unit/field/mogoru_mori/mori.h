#ifndef ALCHEMY_UNIT_FIELD_MOGORU_MORI_MORI_H
#define ALCHEMY_UNIT_FIELD_MOGORU_MORI_MORI_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MogoruMori_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_RunSceneEntryHook(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gMogoruMoriPlacements2_0[];
extern s32 MogoruMori_ActorScript[];
extern s32 gMogoruMoriPlacements1_1[];
extern struct SceneEntrance gMogoruMoriEntrances1[];
extern struct SceneEntrance gMogoruMoriEntrances2[];
extern struct SceneEntrance gMogoruMoriEntrances3[];
extern struct SceneEntrance gMogoruMoriEntrancesOther[];
extern u32 MogoruMori_SceneTable[];
extern struct ScenePlacement gMogoruMoriPlacements1[];
extern struct ScenePlacement gMogoruMoriPlacements2[];
extern struct ScenePlacement gMogoruMoriPlacements3[];
extern struct ScenePlacement gMogoruMoriPlacementsOther[];
extern struct SceneEvent gMogoruMoriEvents1[];
extern struct SceneEvent gMogoruMoriEvents2[];
extern struct SceneEvent gMogoruMoriEvents3[];
extern struct SceneEvent gMogoruMoriEventsOther[];

#endif
