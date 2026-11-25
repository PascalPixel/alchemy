/// @file
/// Cloak: the leader blinks into a dark outline and stays hidden while it
/// walks on cells that allow it (CELL_BEHAVIOR_CLOAK).
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The World Map block of a 16.16 coordinate.
#define WORLD_MAP_BLOCK(coordinate) ((coordinate) >> WORLD_MAP_BLOCK_SHIFT)

/// Task of Cloak: when the cell under the leader is not CELL_BEHAVIOR_CLOAK,
/// requests CLOAK_END. The behavior is byte 2 of a cell (MapGridCell.behavior),
/// or of a 4-byte gCellTiles entry on the World Map.
void FieldPsynergy_CheckCloakTile(void) {
  struct FieldState *state;
  struct Actor *leader;
  u8 *cell;
  FIXED x;
  FIXED y;

  state = gFieldState;
  leader = Actor_Lookup(gMainRoutineState.leaderActorId);
  cell = gMapGridState[0];

  // Whole blocks or cells, rounded toward zero as a division would be.
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    u32 tx;
    u32 ty;

    x = leader->x.fixed;
    /* FAKEMATCH(dead): the division by toFIXED(WORLD_MAP_BLOCK_PIXELS)
     * rounded towards zero by hand; as x / toFIXED(WORLD_MAP_BLOCK_PIXELS). */
    if (x < 0)
      x += toFIXED(WORLD_MAP_BLOCK_PIXELS) - 1;
    tx = WORLD_MAP_BLOCK(x) & (WORLD_MAP_BLOCKS_ACROSS - 1);

    y = leader->z.fixed;
    /* FAKEMATCH(dead): the division by toFIXED(WORLD_MAP_BLOCK_PIXELS)
     * rounded towards zero by hand; as y / toFIXED(WORLD_MAP_BLOCK_PIXELS). */
    if (y < 0)
      y += toFIXED(WORLD_MAP_BLOCK_PIXELS) - 1;
    ty = WORLD_MAP_BLOCK(y) & (WORLD_MAP_BLOCKS_ACROSS - 1);

    cell = (u8 *)&gCellTiles[tx + ty * WORLD_MAP_BLOCKS_ACROSS];
  } else {
    if (leader->layer < MAPGRID_LAYER_COUNT) {
      cell = (u8 *)((struct MapGridScrollState *)cell)
                 ->layers[leader->layer]
                 .cells;
    } else
      cell = (u8 *)gMapGridCellBuffer;

    x = leader->x.fixed;
    {
      u32 tx = x / toFIXED(MAPGRID_CELL_PIXELS);

      y = leader->z.fixed;

      cell =
          (u8 *)(cell +
                 ((tx + y / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE) << 2));
    }
  }

  if (cell[2] != CELL_BEHAVIOR_CLOAK)
    state->psynergy = CLOAK_END;
}

/// Ends Cloak: stops the cell check and makes the leader blink back into sight,
/// clearing cloaked.
void FieldPsynergy_EndCloak(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  struct SpriteAnimation *entry;
  u32 i;

  actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  sprite = actor->sprite;
  entry = sprite->entries[0];

  Sound_PlayFar(SE_CAST);
  Task_Remove(FieldPsynergy_CheckCloakTile);
  Actor_SelectAnimationFar(actor, ANIM_STILL);
  actor->update = 0;

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
    entry->colorMode = SPRITE_COLOR_NORMAL;
    sprite->dirty = 1;
    wait(2);
  }

  sprite->flags = SPRITE_FLAG_SHADOW;
  gMainRoutineState.cloaked = 0;
}

/// Starts the Cloak cell check if the leader is hidden. FieldSequence_Run calls
/// it as a scene loads, since gMainRoutineState.cloaked outlives the scene.
void FieldPsynergy_ResumeCloakCheck(void) {
  if (gMainRoutineState.cloaked != 0) {
    Task_Add(FieldPsynergy_CheckCloakTile, TASK_PASS_FRAME);
  }
}

/// The leader blinks 10 times and hides (cloaked set), and the cell check
/// starts; where the leader cannot hide it shows again at once.
void FieldPsynergy_StartCloak(void) {
  struct FieldState *state = gFieldState;
  struct Actor *leader = Actor_Lookup(gMainRoutineState.leaderActorId);
  struct Sprite *sprite = leader->sprite;
  struct SpriteAnimation *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar(leader, ANIM_STILL);
  leader->update = 0;
  for (n = 0; n <= 9; n++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    sprite->dirty = 1;
    sprite->flags = 0;
    wait(2);
  }
  n = 0;
  entry->colorMode = n;
  sprite->flags = SPRITE_FLAG_OUTLINED;
  sprite->dirty = 1;
  // Check the cell at once: off a Cloak cell, unhide and drop the request
  // the check just raised (n is 0 here).
  check = FieldPsynergy_CheckCloakTile;
  Task_Add(check, TASK_PASS_FRAME);
  gMainRoutineState.cloaked = 1;
  check();
  if (state->psynergy == CLOAK_END) {
    FieldPsynergy_EndCloak();
    state->psynergy = n;
  }
}

#undef WORLD_MAP_BLOCK
