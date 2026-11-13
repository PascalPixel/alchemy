#ifndef ALCHEMY_MODULE_SCENE_VALE_PROLOGUE_H
#define ALCHEMY_MODULE_SCENE_VALE_PROLOGUE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u8 gValePrologue_FrameModes[];
extern s32 gValePrologue_ShovingManScript[];
extern s32 gValePrologue_UnusedGaretRunsUpScript[];
extern s32 gValePrologue_UnusedGaretPantingScript[];
extern s32 gValePrologue_BoulderWaits[];
extern s32 gValePrologue_BoulderFallLeaderPath[];
extern s32 gValePrologue_BoulderFallGaretPath[];
extern s32 gValePrologue_DoraRunsIntoRiversideHouse[];
extern s32 gValePrologue_JennaRunsIntoRiversideHouse[];
extern s32 gValePrologue_DoraComesOutOfRiversideHouse[];
extern s32 gValePrologue_JennaComesOutOfRiversideHouse[];
extern s32 gValePrologue_DoraHeadsNorth[];
extern s32 gValePrologue_JennaHeadsForPlaza[];
extern s32 gValePrologue_DoraLeavesForHelp[];
extern s32 gValePrologue_DoraRunsToRiver[];
extern s32 gValePrologue_ToRiverbankPath[];
extern s32 gValePrologue_ExhaustedStrongmanGatherPath[];
extern s32 gValePrologue_JennaGatherPath[];
extern s32 gValePrologue_LeaderGatherPath[];
extern s32 gValePrologue_GaretGatherPath[];
extern s32 gValePrologue_LookAroundScript[];
extern s32 gValePrologue_SquashAndStretchScript[];
extern s32 gValePrologue_BoulderRollsDownriver[];
extern struct SceneEntrance gValePrologue_Entrances[];
extern u32 gValePrologue_Exits[];
extern struct ActorPlacement gValePrologue_ActorPlacements[];
extern struct ActorPlacement gValePrologue_BoulderWatchers[];
extern struct Trigger gValePrologue_Triggers[];
extern u16 gValePrologue_SanctumDoorOpens[];
extern u16 gValePrologue_IsaacsDoorOpens[];
extern u16 gValePrologue_GrandparentsDoorOpens[];
extern u16 gValePrologue_WestHouseDoorOpens[];
extern u16 gValePrologue_IsaacsDoorCloses[];
extern u16 gValePrologue_GrandparentsDoorCloses[];

#endif
