/// @file
/// Cloak.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// The step code of the cells Cloak can stay hidden on: byte 2 of a cell
/// (MapCell.collisionCode), or of a 4-byte gMapBlocks entry on the world map.
#define CLOAK_TILE 0xfb
/// The Psynergy request that ends Cloak: ACT_PACKED (0x2000, no PP) |
/// ABILITY_CLOAK (0x92).
#define CLOAK_END 0x2092

/// Scheduler callback of Cloak: when the tile under the leader is not
/// CLOAK_TILE, requests CLOAK_END. On the world map (event mode 3) it reads
/// the 32-pixel gMapBlocks, elsewhere the 16-pixel terrain map. Runs once a
/// frame from FieldFx_StartCloak, and again from FieldFx_ResumeCloakCheck
/// when a scene loads with the leader still cloaked.
void FieldFx_CheckCloakTile(void) {
#ifndef UBFIX
  u32 slot;
#endif
  struct FieldWork *field;
  struct ObjectRuntime *leader;
  u8 *tile;
  s32 x;
  s32 y;

#ifndef UBFIX
  slot = (u32)&gEventWork;
#endif
  field = (struct FieldWork *)gEventWork;
  leader = ObjectTable_Get(gGameState.selectedActor);
#ifdef UBFIX
  tile = gMapWork[0];
#else
  // The named map slot avoids reading outside gEventWork. It adds 4 bytes
  // to this function in the forced native EN build (0xc0 -> 0xc4).
  tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]
#endif

  // Whole blocks or cells, rounded toward zero as a division would be.
  if (field->mode == MODE_WORLD_MAP) {
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

    tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
  } else {
    if (leader->terrainId <= 2) {
      tile = (u8 *)((struct MapState *)tile)->layers[leader->terrainId].cells;
    } else
      tile = (u8 *)gMapCellBuffer;

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
    field->command = CLOAK_END;
}

/// Ends Cloak: stops the tile check and makes the leader blink back into
/// sight, clearing cloaked. The leader is white (SPRITE_COLOR_WHITE) for the
/// first five blinks, its outline (sprite flag bit 1) on and off; the last
/// five alternate white and its own colours (param 0). Each half is 2
/// frames. The leader ends with its shadow back (flags 1).
/// FieldEvent_RunTypeHandler runs it when the CLOAK_END request is handled.
void FieldFx_EndCloak(void) {
  struct ObjectRuntime *obj;
  struct AnimationObject *anim;
  struct AnimationEntry *entry;
  u32 i;

  obj = ObjectTable_Get(gGameState.selectedActor);
  anim = obj->animation;
  entry = anim->entries[0];

  AudioCommand_PlayFar(SE_CAST);
  Scheduler_RemoveCallback(FieldFx_CheckCloakTile);
  ObjectDispatch_SelectAnimationFar(obj, 0);
  ((struct ScriptMotionObject *)obj)->hook = 0;

  for (i = 0; i < 5; ++i) {
    entry->param = SPRITE_COLOR_WHITE;
    anim->dirty = 1;
    anim->flags = 2;
    WaitFrames(2);
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
  }

  for (i = 0; i < 5; ++i) {
    entry->param = SPRITE_COLOR_WHITE;
    anim->dirty = 1;
    anim->flags = 0;
    WaitFrames(2);
    entry->param = 0;
    anim->dirty = 1;
    WaitFrames(2);
  }

  anim->flags = 1;
  gGameState.cloaked = 0;
}

/// Starts the Cloak tile check if the leader is hidden. Field_RunScene calls
/// it as a scene loads, since gGameState.cloaked outlives the scene.
void FieldFx_ResumeCloakCheck(void) {
  if (gGameState.cloaked != 0) {
    Scheduler_AddOrUpdateCallback(FieldFx_CheckCloakTile, 0xc80);
  }
}

/// The leader blinks 10 times and hides (cloaked set), and the tile check
/// starts; where the leader cannot hide it shows again at once.
///
/// Abilities: Cloak (field kind 10).
///
/// Hidden, the leader is drawn as an outline only (sprite flags 2: outlined,
/// no shadow), and while gGameState.cloaked is set no random battles come
/// (Encounter_SelectEnemyGroup). Called by BattleFx_Run and
/// BattleFx_DispatchRequestKind.
void FieldFx_StartCloak(void) {
  struct FieldWork *field = (struct FieldWork *)gEventWork;
  struct ObjectRuntime *leader = ObjectTable_Get(gGameState.selectedActor);
  struct AnimationObject *animation = leader->animation;
  struct AnimationEntry *entry = animation->entries[0];
  u32 n;
  void (*check)(void);

  AudioCommand_PlayFar(SE_130);
  ObjectDispatch_SelectAnimationFar(leader, 0);
  ((struct ScriptMotionObject *)leader)->hook = 0;
  n = 0;
  do {
    entry->param = SPRITE_COLOR_WHITE;
    animation->dirty = 1;
    animation->flags = 2;
    WaitFrames(2);
    animation->dirty = 1;
    animation->flags = 0;
    WaitFrames(2);
    n++;
  } while (n <= 9);
  n = 0;
  entry->param = n;
  animation->flags = 2;
  animation->dirty = 1;
  // Check the tile at once: off a Cloak tile, unhide and drop the request
  // the check just raised (n is 0 here).
  check = FieldFx_CheckCloakTile;
  Scheduler_AddOrUpdateCallback(check, 0xc80);
  gGameState.cloaked = 1;
  check();
  if (field->command == CLOAK_END) {
    FieldFx_EndCloak();
    field->command = n;
  }
}

#undef CLOAK_TILE
#undef CLOAK_END
