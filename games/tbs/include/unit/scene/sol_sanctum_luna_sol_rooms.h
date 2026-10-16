#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_LUNA_SOL_ROOMS_H

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
s32 SolSanctumLunaSolRooms_RestoreEntryState(void);
extern struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[];
extern u32 gSolSanctumLunaSolRoomsExits[];
extern struct ScenePlacement gSolSanctumLunaSolRoomsPlacements[];
extern struct SceneEvent gSolSanctumLunaSolRoomsEvents[];

#endif
