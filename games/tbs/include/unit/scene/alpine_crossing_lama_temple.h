#ifndef ALCHEMY_UNIT_MAP_ALPINE_CROSSING_LAMA_TEMPLE_H
#define ALCHEMY_UNIT_MAP_ALPINE_CROSSING_LAMA_TEMPLE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gAlpineCrossing_WorkerActorPlacementScript[];
extern s32 gAlpineCrossing_FeizhiPath[];
extern s32 gAlpineCrossing_LeaderPath[];
extern s32 gAlpineCrossing_HsuStruggle[];
extern u16 gLamaTemple_DoorOpenCells[];
extern u16 gLamaTemple_DoorCloseCells[];
extern struct SceneEntrance gLamaTemple_Entrances[];
extern struct SceneEntrance gAlpineCrossing_Entrances[];
extern u32 gAlpineCrossingLamaTemple_Exits[];
extern struct ActorPlacement gLamaTemple_ActorPlacements[];
extern struct ActorPlacement gAlpineCrossing_ActorPlacements[];
extern struct Trigger gLamaTemple_Triggers[];
extern struct Trigger gAlpineCrossing_Triggers[];

#endif
