/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of field_sequence.c.

#include "types.h"
#include "field/actor.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/glyph.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/objdisp.h"
#include "field/field_event.h"
#include "system/ram_buffer.h"
#include "system/heap_release.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "field/field_sprite.h"
#include "field/flag_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "graphics/display_transition.h"
#include "system/iwram_call.h"
#include "field/map_grid.h"
#include "system/frame.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "graphics/palette_buffers.h"
#include "debug/debug.h"
#include "field/sprite.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "system/io_write.h"
#include "system/main_routine_state.h"
#include "macros.h"
#include "field/field_sequence_event.h"
#include "system/input.h"
#include "field/screen_transition.h"
#include "field/object.h"
#include "field/field_sequence.h"
#include "field/event.h"
#include "gamepak/far_call/window.h"
#include "field/ground_mask.h"
#include "resource_ids.h"

/// Unused: only the far-call table refers to it. Keeps the party leader off the
/// treasure spots: a leader within 16 pixels of a treasure spot's actor is
/// moved 20 pixels from it, in the direction it stood, and loses its move
/// target.
void TreasureSpot_MoveLeaderAway(void) {
  struct Actor *leader;
  struct Actor *other;
  FIXED dx;
  FIXED dz;
  s32 i;

  leader = Actor_Lookup(Party_GetLeaderActor());
  for (i = 0; i <= SCOOPED_ITEM_ACTOR - FIELD_TREASURE_ACTOR; i++) {
    other = Actor_Lookup(i + FIELD_TREASURE_ACTOR);
    dx = leader->x.fixed - other->x.fixed;
    dz = leader->z.fixed - other->z.fixed;
    if (dx > -toFIXED(16) && dx < toFIXED(16))
      if (dz > -toFIXED(16) && dz < toFIXED(16)) {
        leader->x.fixed = other->x.fixed;
        leader->z.fixed = other->z.fixed;
        Vector_AddPolarOffset(toFIXED(20), (u16)Trig_Atan(dz, dx),
                              &leader->x.fixed);
        leader->targetX = ACTOR_NO_TARGET;
        leader->targetY = ACTOR_NO_TARGET;
        leader->targetZ = ACTOR_NO_TARGET;
        break;
      }
  }
}

/// Starts the particle script and its first animation at priority 1, if the
/// spawn succeeded. Allocation failure leaves the sparkle absent.
static inline void Actor_InitializeSparkle(struct Actor *actor) {
  if (actor != 0) {
    Actor_StartScriptFar(actor, (const s32 *)gBattleEffect_ParticleScript);
    Actor_SelectAnimationFar(actor, 0);
    ((struct Sprite *)actor->sprite)->image.priority = 1;
  }
}

/// Each frame of a found item's picture: one time in ten, a sparkle appears
/// within 16 pixels of it.
void ItemDisplay_EmitSparkles(struct Actor *emitter) {
  union PolarOffsetPosition position;
  struct Actor *sparkle;
  u32 randomRadius;

  // Once the picture's rise has all but stopped (under 1/256 pixel a frame),
  // clearing its motion flags (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN)
  // ends gravity, so it hangs at the top of its rise.
  if (emitter->velocityY > -(toFIXED(1) / 256) &&
      emitter->velocityY < toFIXED(1) / 256)
    emitter->motionFlags = 0;

  // Test the chance before drawing the radius and angle from the RNG.
  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = emitter->x.fixed;
  position.vector.y = emitter->y.fixed;
  position.vector.z = emitter->z.fixed;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  sparkle = Actor_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                        position.vector.z);
  Actor_InitializeSparkle(sparkle);
}

/// Returns 1 while Reveal is active, else 0.
s32 Field_IsRevealActive(struct Actor *interpreter) {
  struct FieldState *state = gFieldState;

  if (state == NULL)
    return 0;
  if (state->revealActive != 0)
    return 1;
  else
    return 0;
}

void Reveal_EmitSparkles(struct Actor *source);

/// Shows the picture of item 16 pixels above actor actorId, or for the leader
/// at the centre of the cell 16 pixels in front of them: it rises a little and
/// hangs there, sparkling. While Reveal is active it first releases Reveal's
/// sparkles, and then its mark on the picture's cell.
struct Actor *ItemDisplay_ShowItemInFront(s32 actorId, s32 item) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *picture = state->actors[actorId];
  struct Actor *actor;
  struct Sprite *sprite;
  FIXED position[3];
  GlyphTransfer *transfer;
  s32 tile;
  s32 i;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x.fixed;
  position[1] = picture->y.fixed;
  position[2] = picture->z.fixed;
  if (actorId == Party_GetLeaderActor()) {
    Vector_AddPolarOffset(toFIXED(16), picture->facing, position);
    position[0] = (position[0] & FIELD_CELL_MASK) + toFIXED(8);
    position[2] = (position[2] & FIELD_CELL_MASK) + toFIXED(8);
  }
  if (state->revealActive != 0) {
    // Stop both the marker emitters and the sparkle actors they already made.
    actor = Ram_HeapSlots->actors;
    i = ACTOR_COUNT - 1;
    do {
      if (actor->script != NULL) {
        if (actor->update == Reveal_EmitSparkles)
          Actor_ReleaseFar(actor);
        if (actor->script == (s32 *)gBattleEffect_ParticleScript)
          Actor_ReleaseFar(actor);
      }
      i--;
      actor++;
    } while (i >= 0);
  }
  wait(3);
  picture = Actor_CreateFar(SPRITE_UNSEEN_MARKER, position[0],
                            position[1] + toFIXED(16), position[2]);
  if (picture == NULL)
    return NULL;
  Actor_StartScriptFar(picture, gFieldEffect_ParticleEmitterScript);
  sprite = picture->sprite;
  // No shadow, outline or sprite animations: a 16-colour sprite in OBJ
  // palette 0 whose tiles are the item's icon.
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  sprite->image.priority = 1;
  picture->motionFlags = ACTOR_FALLS;
  picture->groundHeight = position[1];
  picture->velocityY = toFIXED(2);
  picture->gravity = toFIXED(1) / 4;
  transfer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItemFar(item);
  tile = VramCache_Load(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
  sprite->image.tile = tile;
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  picture->update = ItemDisplay_EmitSparkles;
  if (state->revealActive != 0) {
    struct Actor *marker = Ram_HeapSlots->actors;
    s32 j;

    for (j = 0; j < ACTOR_COUNT; j++, marker++) {
      if (marker->script != NULL &&
          marker->script == (s32 *)gBattleEffect_MarkerParticleScript &&
          FIELD_CELL(marker->x.fixed) == FIELD_CELL(picture->x.fixed) &&
          FIELD_CELL(marker->z.fixed) == FIELD_CELL(picture->z.fixed)) {
        Actor_ReleaseFar(marker);
        break;
      }
    }
  }
  return picture;
}

/// Releases actor unless it is NULL.
void Actor_ReleaseIfPresent(struct Actor *actor) {
  if (actor != 0)
    Actor_ReleaseFar(actor);
}

/// Does nothing; nothing calls it.
UNUSED void ItemDisplay_ReservedNoOp(void) {}

/// Raises a found item's picture to 36 pixels above the party leader, who holds
/// it up (ANIM_LEADER_ARMS_RAISED) facing the screen.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *leader;

  if (actor == 0)
    return;

  {
    struct Actor *found = Actor_Lookup(Party_GetLeaderActor());
    // Up to 2 pixels a frame, gaining 1 a frame, in free flight: no motion
    // flags, so it neither falls nor follows the ground.
    actor->acceleration = toFIXED(1);
    actor->speedLimit = toFIXED(2);
    actor->motionFlags = 0;
    leader = found;
  }
  Actor_SetMoveTargetFar(actor, leader->x.fixed, leader->y.fixed + toFIXED(36),
                         leader->z.fixed);
  Actor_SetSpritePriorityDirectFar(actor, 1);
  wait(3);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_ARMS_RAISED);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  // FACING_SOUTH, a quarter turn, faces the screen.
  leader->facing = FACING_SOUTH;
}

/// Shows a found item's picture as flags choose (ItemDisplayFlag), with waits
/// of 60 and 80 frames around its raising when both are set.
void ItemDisplay_ShowPicture(struct Actor *actor, s32 flags) {
  struct Actor *leader;

  if (actor != NULL) {
    leader = Actor_Lookup(Party_GetLeaderActor());
    if (flags & ITEM_DISPLAY_SPARKLES) {
      Actor_SetSpriteFlagsFar(actor, 0);
      Actor_StartScriptFar(actor, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)actor)->velocityY = toFIXED(2);
      ((struct Actor *)actor)->gravity = toFIXED(1) / 4;
      ((struct Actor *)actor)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
      wait(60);
    if (flags & ITEM_DISPLAY_RAISED)
      ItemDisplay_RaiseAboveLeader(actor);
    if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED))
      wait(80);
    Actor_SelectAnimationFar(leader, ANIM_STAND);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames,
/// as flags choose (ItemDisplayFlag).
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *leader = Actor_Lookup(Party_GetLeaderActor());
  GlyphTransfer *transfer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  struct Actor *actor =
      Actor_CreateFar(SPRITE_UNSEEN_MARKER, leader->x.fixed,
                      leader->y.fixed + toFIXED(36), leader->z.fixed);

  if (actor != 0) {
    struct Sprite *sprite = actor->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, transfer->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & ITEM_DISPLAY_SPARKLES)
      actor->update = ItemDisplay_EmitSparkles;
    if (flags & ITEM_DISPLAY_RAISED)
      ItemDisplay_RaiseAboveLeader(actor);

    wait(80);
    Actor_SelectAnimationFar(leader, ANIM_STAND);
    Actor_ReleaseFar(actor);
  }
}

/// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
/// pixels of it, as around a found item's picture (ItemDisplay_EmitSparkles,
/// whose sparkling it repeats line for line).
void Reveal_EmitSparkles(struct Actor *actor) {
  const struct Actor *source = (const struct Actor *)actor;
  union PolarOffsetPosition position;
  struct Actor *sparkle;
  u32 randomRadius;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = source->x.fixed;
  position.vector.y = source->y.fixed;
  position.vector.z = source->z.fixed;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  sparkle = Actor_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                        position.vector.z);
  Actor_InitializeSparkle(sparkle);
}

/// Places Reveal's markers from the scene's item rows (a type, an event id, a
/// flag and a pad): for each of the treasure spots' rows whose flag is clear
/// and whose event has not run, a sparkle marker goes at its actor, or at the
/// map cell of a tile event, when that point is on screen.
void FieldEffect_SpawnRevealMarkers(void) {
  s16 *entry = SceneItem_GetRows();
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  s32 *origin = &((struct MapState *)map)->viewX;
  s32 originX = origin[0] & FIELD_PIXEL_MASK;
  s32 originZ = origin[1] & FIELD_PIXEL_MASK;
  // A word of the map header, then the point SceneItem_FindPosition fills for
  // each row.
  struct {
    s32 headerWord;
    s32 position[3];
  } found;
  struct Actor *actor;
  s16 type;
  s32 id;
  s32 flag;
  s32 i;
  FIXED x;
  s32 y;
  FIXED z;
  s32 dx;
  s32 dz;

  // DEAD: the map header's second word is never read; the ROM keeps its load
  // and store.
  found.headerWord = (*(s32 **)map)[1];
  if (entry == NULL)
    return;
  for (i = 0; i < TREASURE_SPOT_MAX && (type = *entry++) != 0 &&
              !(type & SCENE_ITEMS_HEADER);
       i++) {
    id = entry[0];
    flag = entry[1];
    entry += 3;
    if (type != CONTAINER_CHEST && flag != -1 && GameFlag_Get(flag))
      continue;
    if (SceneItem_FindPosition(id, found.position))
      continue;
    actor = TreasureSpot_GetActor(i + FIELD_TREASURE_ACTOR);
    if (actor == NULL)
      continue;
    // A tile event's actor holds its map cell, marked by this value.
    if (actor->riseBaseY == TREASURE_SPOT_ON_CELL) {
      x = toFIXED(actor->timer);
      z = toFIXED((s16)actor->userValue);
      y = MapGrid_GetTerrainHeightFar(0, x, z);
    } else {
      if ((u16)(type - CONTAINER_CHEST) <= CONTAINER_MIMIC - CONTAINER_CHEST)
        continue;
      if (type == CONTAINER_PSYNERGY_STONE || type == CONTAINER_STONE_TABLET ||
          type == CONTAINER_POND || type == CONTAINER_GROUND)
        continue;
      x = actor->x.fixed;
      y = actor->y.fixed;
      z = actor->z.fixed;
    }
    dx = x - originX;
    dz = z - originZ - y;
    dx >>= 16;
    dz >>= 16;
    // On the 240 x 160 screen, give or take 15 pixels at the sides, 47 above
    // and 31 below.
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Get(flag))
      continue;
    actor = Actor_CreateFar(SPRITE_SPARKLE, x, y, z);
    if (actor != NULL) {
      actor->motionFlags = 0;
      Actor_StartScriptFar((struct Actor *)actor,
                           gBattleEffect_MarkerParticleScript);
      Actor_SetSpriteFlagsFar((struct Actor *)actor, 0);
      Actor_SelectAnimationFar(actor, ANIM_STAND);
      ((struct Sprite *)actor->sprite)->image.priority = 1;
    }
  }
}
