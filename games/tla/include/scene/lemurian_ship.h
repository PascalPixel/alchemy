#ifndef ALCHEMY_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_SCENE_LEMURIAN_SHIP_H

/* @file */
/* The Lemurian Ship's rooms: their tables, doors and set-ups, and the pillar */
/* Lift raises. */

#include "types.h"
#include "scene/scene.h"
#include "field/scene_tables.h"

extern s32 LemurianShip_Entrances[];
extern u32 LemurianShip_CabinsEntrances[];
extern u32 LemurianShip_CabinsRegions[];
extern u32 LemurianShip_AquaHydraRegions[];
extern ACTOR_PLACE LemurianShip_Placements[];
extern ACTOR_PLACE LemurianShip_TwoJellyRoomPlacements[];

extern ACTOR_PLACE LemurianShip_SixJellyRoomPlacements[];

extern ACTOR_PLACE LemurianShip_HoldPlacements[];
extern ACTOR_PLACE LemurianShip_PillarRoomPlacements[];
extern ACTOR_PLACE LemurianShip_EngineRoomPlacements[];
extern LEMURIAN_SHIP_WARP LemurianShip_Doors[];
extern LEMURIAN_SHIP_WARP LemurianShip_LowerDoors[];
extern s32 Leader_WalkThroughDoorScript[];
extern TRIGGER LemurianShip_Triggers[];
extern TRIGGER LemurianShip_CabinTriggers[];
extern TRIGGER LemurianShip_TwoJellyRoomTriggers[];
extern TRIGGER LemurianShip_SixJellyRoomTriggers[];
extern TRIGGER LemurianShip_PipeRoomTriggers[];
extern TRIGGER LemurianShip_LowerDeckTriggers[];
extern u32 LemurianShip_PiersToPedestal[];
extern u32 LemurianShip_ShebaStepsUp[];
extern u32 LemurianShip_JennaStepsUp[];
extern u32 LemurianShip_KradenStepsUp[];
void scLemurianShipSetUpStairRoom(void);

void scLemurianShipSetUpSixJellyRoom(void);

void scLemurianShipSetUpLedgeRoom(void);
void scLemurianShipSetUpPipeRoom(void);
void scLemurianShipSetUpHold(void);
void scLemurianShipSetUpPillarRoom(void);
void scLemurianShipSetUpEngineRoom(void);
extern s16 LemurianShip_DoorOpening[];
extern s16 LemurianShip_FirstDoorOpening[];
void scLemurianShipLiftPillar(void);

#endif
