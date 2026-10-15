#ifndef ALCHEMY_UNIT_SCENE_SOL_LUNA_ROOMS_H
#define ALCHEMY_UNIT_SCENE_SOL_LUNA_ROOMS_H

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
s32 SoruNichigetsu_RestoreEntryState(void);
extern struct SceneEntrance gSoruNichigetsuEntrances[];
extern u32 gSoruNichigetsuExits[];
extern struct ScenePlacement gSoruNichigetsuPlacements[];
extern struct SceneEvent gSoruNichigetsuEvents[];

#endif
