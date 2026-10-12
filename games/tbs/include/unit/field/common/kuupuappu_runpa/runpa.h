#ifndef ALCHEMY_UNIT_FIELD_COMMON_KUUPUAPPU_RUNPA_RUNPA_H
#define ALCHEMY_UNIT_FIELD_COMMON_KUUPUAPPU_RUNPA_RUNPA_H

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
extern const struct SceneEntrance gJailEntrances[];
extern const u32 gJailExits[];
extern const struct ScenePlacement gJailThiefPlacements[];
extern const struct ScenePlacement gJailPrisonerPlacements[];
extern const struct SceneEvent gJailThiefEvents[];
extern const struct SceneEvent gJailPrisonerEvents[];

#endif
