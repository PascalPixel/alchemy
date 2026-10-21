#ifndef ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

/// The placed actors, numbered in the order of
/// gSolSanctumLunaSolRoomsPlacements; Jenna and Garet, its last rows, keep
/// their characters' ids.
enum SolSanctumLunaSolRoomsActor {
  /// Eight statues, four in each room, with the sprite of scene 12's statues
  /// (0xc9, sol_sanctum_statue.c).
  ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN = ACTOR_FIRST_STATUE + 8
};

extern u16 gSolSanctumLunaSolRooms_Light1Timer;
extern s32 gSolSanctumLunaSolRooms_FlashState;
extern u16 gSolSanctumLunaSolRooms_Light2Timer;
extern u16 gSolSanctumLunaSolRooms_Light3Timer;
extern u16 gSolSanctumLunaSolRooms_Light4Timer;

#endif
