#ifndef ALCHEMY_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_SCENE_LEMURIAN_SHIP_H

/* The Lemurian Ship's rooms: their tables, doors and set-ups, and the pillar
   Lift raises. */

#include "types.h"
#include "scene/scene.h"
#include "field/scene_tables.h"

extern Sint32	   LemurianShip_Entrances[];
extern Uint32	   LemurianShip_CabinsEntrances[];
extern Uint32	   LemurianShip_CabinsRegions[];
extern Uint32	   LemurianShip_AquaHydraRegions[];
extern ACTOR_PLACE LemurianShip_Placements[];
extern ACTOR_PLACE LemurianShip_TwoJellyRoomPlacements[];

extern ACTOR_PLACE LemurianShip_SixJellyRoomPlacements[];

extern ACTOR_PLACE		  LemurianShip_HoldPlacements[];
extern ACTOR_PLACE		  LemurianShip_PillarRoomPlacements[];
extern ACTOR_PLACE		  LemurianShip_EngineRoomPlacements[];
extern LEMURIAN_SHIP_WARP LemurianShip_Doors[];
extern LEMURIAN_SHIP_WARP LemurianShip_LowerDoors[];
extern Sint32			  Leader_WalkThroughDoorScript[];
extern CUE				  LemurianShip_Cues[];
extern CUE				  LemurianShip_CabinCues[];
extern CUE				  LemurianShip_TwoJellyRoomCues[];
extern CUE				  LemurianShip_SixJellyRoomCues[];
extern CUE				  LemurianShip_PipeRoomCues[];
extern CUE				  LemurianShip_LowerDeckCues[];
extern Uint32			  LemurianShip_PiersToPedestal[];
extern Uint32			  LemurianShip_ShebaStepsUp[];
extern Uint32			  LemurianShip_JennaStepsUp[];
extern Uint32			  LemurianShip_KradenStepsUp[];
void					  scLemurianShipSetUpStairRoom(void);

void scLemurianShipSetUpSixJellyRoom(void);

void		  scLemurianShipSetUpLedgeRoom(void);
void		  scLemurianShipSetUpPipeRoom(void);
void		  scLemurianShipSetUpHold(void);
void		  scLemurianShipSetUpPillarRoom(void);
void		  scLemurianShipSetUpEngineRoom(void);
extern Sint16 LemurianShip_DoorOpening[];
extern Sint16 LemurianShip_FirstDoorOpening[];
void		  scLemurianShipLiftPillar(void);

#endif
