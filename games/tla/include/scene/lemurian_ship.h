#ifndef ALCHEMY_SCENE_LEMURIAN_SHIP_H
#define ALCHEMY_SCENE_LEMURIAN_SHIP_H

/* The Lemurian Ship's rooms: their tables, doors and set-ups, and the pillar
   Lift raises. */

#include "types.h"
#include "scene/scene.h"
#include "field/scene_tables.h"

extern Sint32	   LemurianShipEntrances[];
extern Uint32	   LemurianShipCabinsEntrances[];
extern Uint32	   LemurianShipCabinsRegions[];
extern Uint32	   LemurianShipAquaHydraRegions[];
extern ACTOR_PLACE LemurianShipPlacements[];
extern ACTOR_PLACE LemurianShipTwoJellyRoomPlacements[];

extern ACTOR_PLACE LemurianShipSixJellyRoomPlacements[];

extern ACTOR_PLACE		  LemurianShipHoldPlacements[];
extern ACTOR_PLACE		  LemurianShipPillarRoomPlacements[];
extern ACTOR_PLACE		  LemurianShipEngineRoomPlacements[];
extern LEMURIAN_SHIP_WARP LemurianShipDoors[];
extern LEMURIAN_SHIP_WARP LemurianShipLowerDoors[];
extern Sint32			  LeaderWalkThroughDoorScript[];
extern CUE				  LemurianShipCues[];
extern CUE				  LemurianShipCabinCues[];
extern CUE				  LemurianShipTwoJellyRoomCues[];
extern CUE				  LemurianShipSixJellyRoomCues[];
extern CUE				  LemurianShipPipeRoomCues[];
extern CUE				  LemurianShipLowerDeckCues[];
extern Uint32			  LemurianShipPiersToPedestal[];
extern Uint32			  LemurianShipShebaStepsUp[];
extern Uint32			  LemurianShipJennaStepsUp[];
extern Uint32			  LemurianShipKradenStepsUp[];
void					  scLemurianShipSetUpStairRoom(void);

void scLemurianShipSetUpSixJellyRoom(void);

void		  scLemurianShipSetUpLedgeRoom(void);
void		  scLemurianShipSetUpPipeRoom(void);
void		  scLemurianShipSetUpHold(void);
void		  scLemurianShipSetUpPillarRoom(void);
void		  scLemurianShipSetUpEngineRoom(void);
extern Sint16 LemurianShipDoorOpening[];
extern Sint16 LemurianShipFirstDoorOpening[];
void		  scLemurianShipLiftPillar(void);

#endif
