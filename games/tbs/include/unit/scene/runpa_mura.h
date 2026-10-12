#ifndef ALCHEMY_UNIT_SCENE_RUNPA_MURA_H
#define ALCHEMY_UNIT_SCENE_RUNPA_MURA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern const s32 *gEffectScripts[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gLunpaEntrances[];
extern struct SceneEntrance gGateEntrances[];
extern u32 gLunpaExits[];
extern u32 gGateExits[];
extern struct ScenePlacement gLunpaPlacements[];
extern struct ScenePlacement gGatePlacements[];
extern struct SceneEvent gLunpaSealedEvents[];
extern struct SceneEvent gLunpaReopenedEvents[];
extern struct SceneEvent gGateEvents[];
extern u32 gLunpaUnusedWord;

#endif
