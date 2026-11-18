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
#include "system/main_routine_work.h"
#include "macros.h"
#include "field/field_sequence_event.h"
#include "system/input.h"
#include "field/screen_transition.h"
#include "field/object.h"
#include "field/event.h"
#include "gamepak/far_call/window.h"
#include "field/ground_mask.h"
#include "resource_ids.h"

/// Unused: only the far-call table refers to it. Finds the first treasure spot
/// actor within 16 pixels of the leader on both axes and puts the leader 20
/// pixels from it, on the line from it to the leader, with no destination.
void Leader_StepAwayFromNearbyActor(void) {
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
        leader->targetX = TARGET_UNSET;
        leader->targetY = TARGET_UNSET;
        leader->targetZ = TARGET_UNSET;
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
  struct Actor *actor;
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
  actor = Actor_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                      position.vector.z);
  Actor_InitializeSparkle(actor);
}

/// Returns 1 while Reveal is active, else 0.
s32 Field_IsRevealActive(struct Actor *interpreter) {
  /* FAKEMATCH(temporary): the state tested for nonzero by the sign bit of
   * (0 - x) | x, held in a local; as work->revealActive != 0. */
  s16 revealState;
  u32 isActive = 0;
  struct FieldWork *work = gFieldWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}

void Reveal_EmitSparkles(struct Actor *source);

/// Shows the picture of item above actor (in front of the controlled owner,
/// on the centre of the cell), clearing Reveal's sparkles first.
struct Actor *ItemDisplay_ShowItemInFront(s32 actorId, s32 item) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *picture = work->actors[actorId];
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
  if (work->revealActive != 0) {
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
  tile = VramCache_Load(sprite->slot, 4 * TILE_SIZE_4BPP, transfer->tiles);
  sprite->image.tile = tile;
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  picture->update = ItemDisplay_EmitSparkles;
  if (work->revealActive != 0) {
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

/// Releases object unless it is NULL.
void Actor_ReleaseIfPresent(struct Actor *actor) {
  if (actor != 0)
    Actor_ReleaseFar(actor);
}

/// Does nothing; nothing in the image calls it.
void ScreenTransition_ReservedNoOp(void) {}

/// Places the object 36 pixels above the controlled owner, gives it its
/// sprite priority and, three frames on, the descent script, and turns the
/// owner to face a quarter turn.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *owner;

  if (actor == 0)
    return;

  {
    struct Actor *found = Actor_Lookup(Party_GetLeaderActor());
    actor->acceleration = toFIXED(1);
    actor->speedLimit = toFIXED(2);
    actor->motionFlags = 0;
    owner = found;
  }
  Actor_SetMoveTargetFar(actor, owner->x.fixed, owner->y.fixed + toFIXED(36),
                         owner->z.fixed);
  Actor_SetSpritePriorityDirectFar(actor, 1);
  wait(3);
  Actor_SelectAnimationFar(owner, ANIM_LEADER_ARMS_RAISED);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  owner->facing = FACING_SOUTH;
}

/// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
/// above the leader, with waits of 60 and 80 frames around that when both are
/// set.
void ItemDisplay_ShowPicture(struct Actor *actor, s32 flags) {
  struct Actor *owner;

  if (actor != NULL) {
    owner = Actor_Lookup(Party_GetLeaderActor());
    if (flags & 1) {
      Actor_SetSpriteFlagsFar(actor, 0);
      Actor_StartScriptFar(actor, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)actor)->velocityY = toFIXED(2);
      ((struct Actor *)actor)->gravity = toFIXED(1) / 4;
      ((struct Actor *)actor)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == 3)
      wait(60);
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(actor);
    if (flags == 3)
      wait(80);
    Actor_SelectAnimationFar(owner, ANIM_STAND);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames;
/// flags bit 0 adds sparkles and bit 1 raises it as a found item.
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *owner = Actor_Lookup(Party_GetLeaderActor());
  GlyphTransfer *transfer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  struct Actor *actor =
      Actor_CreateFar(SPRITE_UNSEEN_MARKER, owner->x.fixed,
                      owner->y.fixed + toFIXED(36), owner->z.fixed);

  if (actor != 0) {
    struct Sprite *sprite = actor->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, 4 * TILE_SIZE_4BPP, transfer->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & 1)
      actor->update = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(actor);

    wait(80);
    Actor_SelectAnimationFar(owner, ANIM_STAND);
    Actor_ReleaseFar(actor);
  }
}

/// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
/// pixels of it.
void Reveal_EmitSparkles(struct Actor *actor) {
  const struct Actor *source = (const struct Actor *)actor;
  union PolarOffsetPosition position;
  struct Actor *object;
  u32 randomRadius;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = source->x.fixed;
  position.vector.y = source->y.fixed;
  position.vector.z = source->z.fixed;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  object = Actor_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                       position.vector.z);
  Actor_InitializeSparkle(object);
}

/// Places Reveal's markers from the scene's item rows (a type, an event id, a
/// flag and a pad): for each of the treasure spots' rows whose flag is clear
/// and whose event has not run, a sparkle marker goes at its actor, or at the
/// map cell of a tile event, when that point is on screen.
void FieldEffect_SpawnRevealMarkers(void) {
  s16 *entry = SceneItem_GetRows();
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  s32 *origin = &((struct MapWork *)map)->viewX;
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
