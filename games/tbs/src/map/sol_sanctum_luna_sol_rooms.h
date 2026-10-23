#ifndef ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H
#define ALCHEMY_SOL_SANCTUM_LUNA_SOL_ROOMS_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

/// The placed object events, numbered in the order of
/// gSolSanctumLunaSolRoomsObjectEventTemplates; Jenna and Garet, its last rows,
/// keep their characters' ids.
enum SolSanctumLunaSolRoomsLocalId {
  /// Eight statues, four in each room, with the sprite of
  /// MapId_SolSanctumStatue's statues (sol_sanctum_statue.c).
  /// ★ Named from what the player sees: winged stone statues standing at the
  /// corners of each room's floor (its render)
  LOCALID_FIRST_STATUE = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "What's wrong, Kraden?" (MsgSolSanctumWrongKraden)
  LOCALID_KRADEN = LOCALID_FIRST_STATUE + 8
};

extern u16 gSolSanctumLunaSolRooms_Light1Timer;
extern s32 gSolSanctumLunaSolRooms_FlashState;
extern u16 gSolSanctumLunaSolRooms_Light2Timer;
extern u16 gSolSanctumLunaSolRooms_Light3Timer;
extern u16 gSolSanctumLunaSolRooms_Light4Timer;

#endif
