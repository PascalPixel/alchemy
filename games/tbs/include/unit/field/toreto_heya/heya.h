#ifndef ALCHEMY_UNIT_FIELD_TORETO_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_TORETO_HEYA_HEYA_H

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
s32 ToretoHeya_EnterRoom(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s16 ToretoHeya_MapPatches[];
extern const s32 *gEffectScripts[];
extern s32 ToretoHeya_TableActions0[];
extern s32 ToretoHeya_TableActions1[];
extern s32 ToretoHeya_TableActions2[];
extern s32 ToretoHeya_TableActions3[];
extern s32 ToretoHeya_ActionTable1[];
extern s32 ToretoHeya_ActionTable2[];
extern u32 ToretoHeya_TintSteps[];
extern struct SceneEntrance gToretoHeyaEntrances[];
extern s16 gToretoHeyaRegions[];
extern u32 gToretoHeyaExits[];
extern struct ScenePlacement gToretoHeyaPlacements[];
extern struct SceneEvent gToretoHeyaEvents[];
extern s32 ToretoHeya_TintIndex;

#endif
