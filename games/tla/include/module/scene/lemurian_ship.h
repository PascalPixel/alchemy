#ifndef ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_MODULE_SCENE_LEMURIAN_SHIP_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"
#include "scene_tables.h"

extern s32 gLemurianShip_Entrances[];
extern u32 gLemurianShip_Room1Entrances[];
extern u32 gLemurianShip_Room1Regions[];
extern u32 gLemurianShip_Room2Regions[];
extern struct ActorPlacement gLemurianShip_Placements[];
extern struct ActorPlacement gLemurianShip_TwoJellyRoomPlacements[];

extern struct ActorPlacement gLemurianShip_SixJellyRoomPlacements[];

extern struct ActorPlacement gLemurianShip_HoldPlacements[];
extern struct ActorPlacement gLemurianShip_PillarRoomPlacements[];
extern struct ActorPlacement gLemurianShip_EngineRoomPlacements[];
extern struct LemurianShipWarp gLemurianShip_Doors[];
extern struct LemurianShipWarp gLemurianShip_LowerDoors[];
extern s32 gLemurianShip_IntoPassage[];
extern struct Trigger gLemurianShip_Triggers[];
extern struct Trigger gLemurianShip_CabinTriggers[];
extern struct Trigger gLemurianShip_TwoJellyRoomTriggers[];
extern struct Trigger gLemurianShip_SixJellyRoomTriggers[];
extern struct Trigger gLemurianShip_SevenJellyRoomTriggers[];
extern struct Trigger gLemurianShip_LowerDeckTriggers[];
extern u32 gLemurianShip_PiersToPedestal[];
extern u32 gLemurianShip_ShebaStepsUp[];
extern u32 gLemurianShip_JennaStepsUp[];
extern u32 gLemurianShip_KradenStepsUp[];
void LemurianShip_SetUpStairRoom(void);

void LemurianShip_SetUpSixJellyRoom(void);

void LemurianShip_SetUpLedgeRoom(void);
void LemurianShip_SetUpSevenJellyRoom(void);
void LemurianShip_SetUpHold(void);
void LemurianShip_SetUpPillarRoom(void);
void LemurianShip_SetUpEngineRoom(void);
extern s16 gLemurianShip_DoorOpening[];
extern s16 gLemurianShip_FirstDoorOpening[];
void LemurianShip_LiftPillar(void);

#endif
