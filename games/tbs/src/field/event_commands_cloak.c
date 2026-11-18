/// @file
/// Cloak.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The cell behavior of the cells Cloak can stay hidden on: byte 2 of a
/// cell (MapGridCell.behavior), or of a 4-byte gCellPictures entry on the
/// World Map.
#define CLOAK_TILE 251
/// The Psynergy request that ends Cloak, without paying PP.
#define CLOAK_END (ACT_PACKED | ABILITY_CLOAK)
/// The World Map's cells are 32 pixels square, and its cell buffer
/// (gCellPictures) holds 32 by 32 of them, wrapping round.
#define WORLD_MAP_CELL_PIXELS 32
#define WORLD_MAP_CELL(coordinate) ((coordinate) >> 21)
#define WORLD_MAP_BUFFER_CELLS 32

/// Task of Cloak: when the cell under the leader is not CLOAK_TILE, requests
/// CLOAK_END.
void FieldPsynergy_CheckCloakTile(void) {
  struct FieldWork *work;
  struct Actor *leader;
  u8 *tile;
  FIXED x;
  FIXED y;

  work = gFieldWork;
  leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  tile = gMapGridWork[0];

  // Whole cell pictures or cells, rounded toward zero as a division would be.
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    u32 tx;
    u32 ty;

    x = leader->x.fixed;
    if (x < 0)
      x += toFIXED(WORLD_MAP_CELL_PIXELS) - 1;
    tx = WORLD_MAP_CELL(x) & (WORLD_MAP_BUFFER_CELLS - 1);

    y = leader->z.fixed;
    if (y < 0)
      y += toFIXED(WORLD_MAP_CELL_PIXELS) - 1;
    ty = WORLD_MAP_CELL(y) & (WORLD_MAP_BUFFER_CELLS - 1);

    tile = (u8 *)&gCellPictures[tx + ty * WORLD_MAP_BUFFER_CELLS];
  } else {
    if (leader->layer <= 2) {
      tile =
          (u8 *)((struct MapGridScrollWork *)tile)->layers[leader->layer].cells;
    } else
      tile = (u8 *)gMapGridCellBuffer;

    x = leader->x.fixed;
    if (x < 0)
      x += toFIXED(MAPGRID_CELL_PIXELS) - 1;
    {
      u32 tx = FIELD_CELL(x);

      y = leader->z.fixed;
      if (y < 0)
        y += toFIXED(MAPGRID_CELL_PIXELS) - 1;

      tile = (u8 *)(tile + ((tx + FIELD_CELL(y) * MAPGRID_SIZE) << 2));
    }
  }

  if (tile[2] != CLOAK_TILE)
    work->psynergy = CLOAK_END;
}

/// Ends Cloak: stops the cell check and makes the leader blink back into sight,
/// clearing cloaked.
void FieldPsynergy_EndCloak(void) {
  struct Actor *actor;
  struct Sprite *animation;
  struct SpriteAnimation *entry;
  u32 i;

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  animation = actor->sprite;
  entry = animation->entries[0];

  Sound_PlayFar(SE_CAST);
  Task_Remove(FieldPsynergy_CheckCloakTile);
  Actor_SelectAnimationFar(actor, ANIM_STILL);
  actor->update = 0;

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    animation->dirty = 1;
    animation->flags = 0;
    wait(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = 0;
    wait(2);
    entry->colorMode = SPRITE_COLOR_NORMAL;
    animation->dirty = 1;
    wait(2);
  }

  animation->flags = SPRITE_FLAG_SHADOW;
  gMainRoutineWork.cloaked = 0;
}

/// Starts the Cloak cell check if the leader is hidden. FieldSequence_Run calls
/// it as a scene loads, since gMainRoutineWork.cloaked outlives the scene.
void FieldPsynergy_ResumeCloakCheck(void) {
  if (gMainRoutineWork.cloaked != 0) {
    Task_Add(FieldPsynergy_CheckCloakTile, TASK_PASS_FRAME);
  }
}

/// The leader blinks 10 times and hides (cloaked set), and the cell check
/// starts; where the leader cannot hide it shows again at once.
void FieldPsynergy_StartCloak(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *animation = leader->sprite;
  struct SpriteAnimation *entry = animation->entries[0];
  u32 n;
  void (*check)(void);

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar(leader, ANIM_STILL);
  leader->update = 0;
  for (n = 0; n <= 9; n++) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    animation->dirty = 1;
    animation->flags = 0;
    wait(2);
  }
  n = 0;
  entry->colorMode = n;
  animation->flags = SPRITE_FLAG_OUTLINED;
  animation->dirty = 1;
  // Check the cell at once: off a Cloak cell, unhide and drop the request
  // the check just raised (n is 0 here).
  check = FieldPsynergy_CheckCloakTile;
  Task_Add(check, TASK_PASS_FRAME);
  gMainRoutineWork.cloaked = 1;
  check();
  if (work->psynergy == CLOAK_END) {
    FieldPsynergy_EndCloak();
    work->psynergy = n;
  }
}

#undef CLOAK_TILE
#undef CLOAK_END
#undef WORLD_MAP_CELL_PIXELS
#undef WORLD_MAP_CELL
#undef WORLD_MAP_BUFFER_CELLS
