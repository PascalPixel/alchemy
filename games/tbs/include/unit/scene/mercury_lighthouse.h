#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *MakyuriHeya_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 MakyuriHeya_RiseScript[];
extern s32 MakyuriHeya_SparkScript[];
extern s32 Makyuri_RampScript[];
extern s32 Makyuri_ScaleCounterScript[];
extern s32 Makyuri_PillarScript[];
extern u16 Makyuri_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 MakyuriHeya_SparkBurstScript[];
extern s32 MakyuriHeya_PushScriptA[];
extern s32 MakyuriHeya_PushScriptB[];
extern s32 MakyuriHeya_PushScriptD[];
extern s32 MakyuriHeya_PushScriptC[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern u16 MakyuriHeya_GateCells[];
extern u16 MakyuriHeya_GateCloseCells[];
extern u16 MakyuriHeya_FloorSwitchCells[];
extern u16 MakyuriHeya_FloorSwitchCloseCells[];
extern u16 MakyuriHeya_ColumnCells[];
extern struct SceneEntrance gMakyuriHeyaEntrancesOther[];
extern struct SceneEntrance gMakyuriHeyaEntrances2[];
extern struct SceneEntrance gMakyuriHeyaEntrances3[];
extern struct SceneEntrance gMakyuriHeyaEntrances4[];
extern u32 MakyuriHeya_SceneTable[];
extern struct ScenePlacement gMakyuriHeyaPlacementsOther[];
extern struct ScenePlacement gMakyuriHeyaPlacements1[];
extern struct ScenePlacement gMakyuriHeyaPlacements2[];
extern struct ScenePlacement gMakyuriHeyaPlacements3[];
extern struct ScenePlacement gMakyuriHeyaPlacements4[];
extern struct SceneEvent gMakyuriHeyaEvents1[];
extern struct SceneEvent gMakyuriHeyaEvents2[];
extern struct SceneEvent gMakyuriHeyaEvents3[];
extern struct SceneEvent gMakyuriHeyaEventsOther[];

#endif
