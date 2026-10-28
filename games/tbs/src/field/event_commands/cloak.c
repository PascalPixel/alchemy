/// @file
/// Cloak.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The cell behavior of the cells Cloak can stay hidden on: byte 2 of a
/// cell (MapGridCell.behavior), or of a 4-byte gCellPictures entry on the
/// World Map.
#define CLOAK_TILE 0xfb
/// The Psynergy request that ends Cloak: ACT_PACKED (no PP) |
/// ABILITY_CLOAK.
#define CLOAK_END 0x2092

/// Task of Cloak: when the cell under the leader is not
/// CLOAK_TILE, requests CLOAK_END. On the World Map it reads
/// the 32-pixel gCellPictures, elsewhere the 16-pixel cells. Runs once a
/// frame from FieldPsynergy_StartCloak, and again from
/// FieldPsynergy_ResumeCloakCheck when a scene loads with the leader still
/// cloaked.
void FieldPsynergy_CheckCloakTile(void) {
#ifndef UBFIX
  u32 slot;
#endif
  struct FieldWork *work;
  struct Actor *leader;
  u8 *tile;
  s32 x;
  s32 y;

#ifndef UBFIX
  slot = (u32)&gFieldWork;
#endif
  work = gFieldWork;
  leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
#ifdef UBFIX
  tile = gMapGridWork[0];
#else
  // The named map slot avoids reading outside gFieldWork. It adds 4 bytes
  // to this function in the forced native EN build (0xc0 -> 0xc4).
  tile = (u8 *)*(struct MapGridScrollAnimView **)(slot - 76); // gMapGridWork[0]
#endif

  // Whole cell pictures or cells, rounded toward zero as a division would be.
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    u32 tx;
    u32 ty;

    x = leader->x.fixed;
    if (x < 0)
      x += 0x1fffff;
    tx = (x >> 21) & 31;

    y = leader->z.fixed;
    if (y < 0)
      y += 0x1fffff;
    ty = (y >> 21) & 31;

    tile = (u8 *)gCellPictures + ((tx + (ty << 5)) << 2);
  } else {
    if (leader->layer <= 2) {
      tile = (u8 *)((struct MapGridScrollAnimView *)tile)
                 ->layers[leader->layer]
                 .cells;
    } else
      tile = (u8 *)gMapGridCellBuffer;

    x = leader->x.fixed;
    if (x < 0)
      x += 0xfffff;
    {
      u32 tx = x >> 20;

      y = leader->z.fixed;
      if (y < 0)
        y += 0xfffff;

      tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
    }
  }

  if (tile[2] != CLOAK_TILE)
    work->psynergy = CLOAK_END;
}

/// Ends Cloak: stops the cell check and makes the leader blink back into
/// sight, clearing cloaked. The leader is white (SPRITE_COLOR_WHITE) for the
/// first five blinks, its outline (sprite flag bit 1) on and off; the last
/// five alternate white and its own colours (param 0). Each half is 2
/// frames. The leader ends with its shadow back.
/// Event_RunFieldPsynergy runs it when the CLOAK_END request is handled.
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
  Actor_SelectAnimationFar(actor, 0);
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
///
/// Abilities: Cloak (FIELD_PSYNERGY_CLOAK).
///
/// Hidden, the leader is drawn as an outline only (SPRITE_FLAG_OUTLINED, no
/// shadow), and while gMainRoutineWork.cloaked is set no random battles come
/// (Encounter_CountStepsAndSelectFormation). Called by FieldPsynergy_Run and
/// FieldPsynergy_Dispatch.
void FieldPsynergy_StartCloak(void) {
  struct FieldWork *work = gFieldWork;
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct Sprite *animation = leader->sprite;
  struct SpriteAnimation *entry = animation->entries[0];
  u32 n;
  void (*check)(void);

  Sound_PlayFar(SE_PSYNERGY_START);
  Actor_SelectAnimationFar(leader, 0);
  leader->update = 0;
  n = 0;
  do {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = SPRITE_FLAG_OUTLINED;
    wait(2);
    animation->dirty = 1;
    animation->flags = 0;
    wait(2);
    n++;
  } while (n <= 9);
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
