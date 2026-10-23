/// @file
/// Vault, before and after the thieves are caught: the doorway to the Sanctum,
/// which opens in two frames as the leader walks into it facing north. Both
/// maps number that warp 11 (VAULT_TO_SANCTUM and VAULT_REVISIT_TO_SANCTUM),
/// so the shared code keeps the number.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "script_layer.h"
#include "map_tables.h"
#include "songs.h"

enum {
  VAULT_DOOR_OPENING_FRAME_COLUMN = 42,
  VAULT_DOOR_OPEN_FRAME_COLUMN = 40,
  VAULT_DOOR_HALF_SIZE = 2
};

/// The two square halves are stacked in the source layout and drawn side by
/// side.
static inline void Vault_DrawDoorwayFrame(s32 sourceColumn) {
  MapGrid_CopyCellsTo(sourceColumn, 33, 34, 16, VAULT_DOOR_HALF_SIZE,
                      VAULT_DOOR_HALF_SIZE);
  MapGrid_CopyCellsTo(sourceColumn, 35, 36, 16, VAULT_DOOR_HALF_SIZE,
                      VAULT_DOOR_HALF_SIZE);
}

/// Walking north into VAULT_CELL_SANCTUM_DOORWAY opens the doorway to the
/// Sanctum.
/// The leader passes behind the doorframe through the warp.
static void Vault_OpenDoorwayAndEnter(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct Sprite *sprite = objectEvent->sprite;

  Engine_AudioPlayCue(SE_188);
  Vault_DrawDoorwayFrame(VAULT_DOOR_OPENING_FRAME_COLUMN);
  Engine_ScriptWait(4);
  Vault_DrawDoorwayFrame(VAULT_DOOR_OPEN_FRAME_COLUMN);
  Engine_ScriptWait(4);
  MapGrid_CopyCellAttributes(33, 21, 2, 2, 3, 16);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(64, 272, 11);
}
