/// @file
/// Cloak.
// Part of field_services.c.

#include "../field_services_internal.h"

/// The metatile behavior of the cells Cloak can stay hidden on: byte 2 of a
/// cell (MapGridCell.behavior), or of a 4-byte gMetatiles entry on the
/// world map.
#define CLOAK_TILE 0xfb
/// The Psynergy request that ends Cloak: ACT_PACKED (0x2000, no PP) |
/// ABILITY_CLOAK (0x92).
#define CLOAK_END 0x2092

/// Task of Cloak: when the cell under the leader is not
/// CLOAK_TILE, requests CLOAK_END. On the world map it reads
/// the 32-pixel gMetatiles, elsewhere the 16-pixel cells. Runs once a
/// frame from FieldEffect_StartCloak, and again from
/// FieldEffect_ResumeCloakCheck when a map loads with the leader still
/// cloaked.
void FieldEffect_CheckCloakTile(void) {
#ifndef UBFIX
  u32 slot;
#endif
  struct FieldWork *work;
  struct Object *leader;
  u8 *tile;
  s32 x;
  s32 y;

#ifndef UBFIX
  slot = (u32)&gFieldWork;
#endif
  work = gFieldWork;
  leader = Actor_GetByActorId(gSaveBlock.leaderActorId);
#ifdef UBFIX
  tile = gMapGridWork[0];
#else
  // The named map slot avoids reading outside gFieldWork. It adds 4 bytes
  // to this function in the forced native EN build (0xc0 -> 0xc4).
  tile = (u8 *)*(struct MapGridScrollAnimView **)(slot - 76); // gMapGridWork[0]
#endif

  // Whole metatiles or cells, rounded toward zero as a division would be.
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    u32 tx;
    u32 ty;

    x = leader->x;
    if (x < 0)
      x += 0x1fffff;
    tx = (x >> 21) & 31;

    y = leader->z;
    if (y < 0)
      y += 0x1fffff;
    ty = (y >> 21) & 31;

    tile = (u8 *)gMetatiles + ((tx + (ty << 5)) << 2);
  } else {
    if (leader->layer <= 2) {
      tile = (u8 *)((struct MapGridScrollAnimView *)tile)
                 ->layers[leader->layer]
                 .cells;
    } else
      tile = (u8 *)gMapGridCellBuffer;

    x = leader->x;
    if (x < 0)
      x += 0xfffff;
    {
      u32 tx = x >> 20;

      y = leader->z;
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
/// FieldScript_RunTypeHandler runs it when the CLOAK_END request is handled.
void FieldEffect_EndCloak(void) {
  struct Object *object;
  struct Sprite *animation;
  struct SpriteAnim *entry;
  u32 i;

  object = Actor_GetByActorId(gSaveBlock.leaderActorId);
  animation = object->sprite;
  entry = animation->entries[0];

  Sound_PlaySongFar(SE_CAST);
  Task_Remove(FieldEffect_CheckCloakTile);
  Object_SelectAnimationFar(object, 0);
  object->update = 0;

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = SPRITE_FLAG_OUTLINED;
    WaitFrames(2);
    animation->dirty = 1;
    animation->flags = 0;
    WaitFrames(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = 0;
    WaitFrames(2);
    entry->colorMode = SPRITE_COLOR_NORMAL;
    animation->dirty = 1;
    WaitFrames(2);
  }

  animation->flags = SPRITE_FLAG_SHADOW;
  gSaveBlock.cloaked = 0;
}

/// Starts the Cloak cell check if the leader is hidden. SceneManager_Run calls
/// it as a map loads, since gSaveBlock.cloaked outlives the map.
void FieldEffect_ResumeCloakCheck(void) {
  if (gSaveBlock.cloaked != 0) {
    Task_Add(FieldEffect_CheckCloakTile, TASK_PASS_FRAME);
  }
}

/// The leader blinks 10 times and hides (cloaked set), and the cell check
/// starts; where the leader cannot hide it shows again at once.
///
/// Abilities: Cloak (FIELD_PSYNERGY_CLOAK).
///
/// Hidden, the leader is drawn as an outline only (SPRITE_FLAG_OUTLINED, no
/// shadow), and while gSaveBlock.cloaked is set no random battles come
/// (Encounter_CountStepsAndSelectFormation). Called by FieldEffect_Run and
/// FieldEffect_DispatchRequestKind.
void FieldEffect_StartCloak(void) {
  struct FieldWork *work = gFieldWork;
  struct Object *leader = Actor_GetByActorId(gSaveBlock.leaderActorId);
  struct Sprite *animation = leader->sprite;
  struct SpriteAnim *entry = animation->entries[0];
  u32 n;
  void (*check)(void);

  Sound_PlaySongFar(SE_PSYNERGY_START);
  Object_SelectAnimationFar(leader, 0);
  leader->update = 0;
  n = 0;
  do {
    entry->colorMode = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = SPRITE_FLAG_OUTLINED;
    WaitFrames(2);
    animation->dirty = 1;
    animation->flags = 0;
    WaitFrames(2);
    n++;
  } while (n <= 9);
  n = 0;
  entry->colorMode = n;
  animation->flags = SPRITE_FLAG_OUTLINED;
  animation->dirty = 1;
  // Check the cell at once: off a Cloak cell, unhide and drop the request
  // the check just raised (n is 0 here).
  check = FieldEffect_CheckCloakTile;
  Task_Add(check, TASK_PASS_FRAME);
  gSaveBlock.cloaked = 1;
  check();
  if (work->psynergy == CLOAK_END) {
    FieldEffect_EndCloak();
    work->psynergy = n;
  }
}

#undef CLOAK_TILE
#undef CLOAK_END
