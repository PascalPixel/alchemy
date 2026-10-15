#ifndef ALCHEMY_UNIT_SCENE_TRET_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TRET_INTERIORS_H

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
s32 TretInteriors_EnterRoom(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s16 gTretInteriors_MapPatches[];
extern const s32 *gEffectScripts[];
extern s32 gTretInteriors_TableActions0[];
extern s32 gTretInteriors_TableActions1[];
extern s32 gTretInteriors_TableActions2[];
extern s32 gTretInteriors_TableActions3[];
extern s32 gTretInteriors_ActionTable1[];
extern s32 gTretInteriors_ActionTable2[];
extern u32 gTretInteriors_TintSteps[];
extern struct SceneEntrance gTretInteriorsEntrances[];
extern s16 gTretInteriorsRegions[];
extern u32 gTretInteriorsExits[];
extern struct ScenePlacement gTretInteriorsPlacements[];
extern struct SceneEvent gTretInteriorsEvents[];
extern s32 gTretInteriors_TintIndex;

#endif
