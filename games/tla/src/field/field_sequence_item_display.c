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

// The parts, in ROM order.
#include "field/screen_transition.h"
#include "field/object.h"
#include "field/event.h"
#include "gamepak/far_call/window.h"
#include "field/ground_mask.h"
#include "resource_ids.h"

/// Unused: only the far-call table refers to it. Finds the first of actors 64
/// to 79 within 16 pixels of the leader on both axes and puts the leader 20
/// pixels from it, on the line from it to the leader, with no destination.
void Leader_StepAwayFromNearbyActor(void) {
  struct Actor *leader;
  struct Actor *other;
  s32 dx;
  s32 dz;
  s32 i;

  leader = Actor_Lookup(Party_GetLeaderActor());
  for (i = 0; i <= 15; i++) {
    other = Actor_Lookup(i + 64);
    dx = leader->x.fixed - other->x.fixed;
    dz = leader->z.fixed - other->z.fixed;
    if (dx > -px(16) && dx < px(16))
      if (dz > -px(16) && dz < px(16)) {
        leader->x.fixed = other->x.fixed;
        leader->z.fixed = other->z.fixed;
        Vector_AddPolarOffset(px(20), (u16)ArcTan2(dz, dx), &leader->x.fixed);
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
  struct ParticlePosition position;
  struct Actor *object;
  u32 randomRadius;

  // Once the picture's rise has all but stopped (under 1/256 pixel a frame),
  // clearing its motion flags ends gravity, so it hangs at the top of its
  // rise.
  if (emitter->velocityY >= -255 && emitter->velocityY <= 255)
    emitter->motionFlags = 0;

  // Test the chance before drawing the radius and angle from the RNG.
  if ((100 * Random16() >> 16) > 9)
    return;

  SET_POSITION(&position, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), &position);
  object = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  Actor_InitializeSparkle(object);
}

/// Returns 1 while Reveal is active, else 0.
s32 Field_IsRevealActive(struct Actor *interpreter) {
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
struct Actor *ItemDisplay_ShowItemInFront(s32 actor, s32 item) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *picture = work->actors[actor];
  struct Actor *object;
  struct Sprite *sprite;
  s32 position[3];
  u8 *buffer;
  s32 tile;
  s32 i;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x.fixed;
  position[1] = picture->y.fixed;
  position[2] = picture->z.fixed;
  if (actor == Party_GetLeaderActor()) {
    Vector_AddPolarOffset(px(16), picture->facing, position);
    position[0] = (position[0] & FIELD_CELL_MASK) + px(8);
    position[2] = (position[2] & FIELD_CELL_MASK) + px(8);
  }
  if (work->revealActive != 0) {
    object = Ram_HeapSlots->actors;
    i = 63;
    do {
      if (object->script != NULL) {
        if (object->update == Reveal_EmitSparkles)
          Actor_ReleaseFar(object);
        if (object->script == (s32 *)gBattleEffect_ParticleScript)
          Actor_ReleaseFar(object);
      }
      i--;
      object++;
    } while (i >= 0);
  }
  wait(3);
  picture =
      Actor_CreateFar(489, position[0], position[1] + px(16), position[2]);
  if (picture == NULL)
    return NULL;
  Actor_StartScriptFar(picture, gFieldEffect_ParticleEmitterScript);
  sprite = picture->sprite;
  sprite->flags = 0;
  sprite->count = 0;
  sprite->image.fullColor = 0;
  sprite->image.palette = 0;
  sprite->image.priority = 1;
  picture->motionFlags = 2;
  picture->groundHeight = position[1];
  picture->velocityY = px(2);
  picture->gravity = px(1) / 4;
  buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItemFar(item);
  tile = VramCache_Load(sprite->slot, 4 * TILE_SIZE_4BPP, buffer + 1024);
  sprite->image.tile = tile;
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  picture->update = ItemDisplay_EmitSparkles;
  if (work->revealActive != 0) {
    struct Actor *marker = Ram_HeapSlots->actors;
    s32 j;

    for (j = 0; j <= 63; j++, marker++) {
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
void Actor_ReleaseIfPresent(struct Actor *object) {
  if (object != 0)
    Actor_ReleaseFar(object);
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
    actor->acceleration = px(1);
    actor->speedLimit = px(2);
    actor->motionFlags = 0;
    owner = found;
  }
  Actor_SetMoveTargetFar(actor, owner->x.fixed, owner->y.fixed + px(36),
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
void ItemDisplay_ShowPicture(struct Actor *object, s32 flags) {
  struct Actor *owner;

  if (object != NULL) {
    owner = Actor_Lookup(Party_GetLeaderActor());
    if (flags & 1) {
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_StartScriptFar(object, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)object)->velocityY = px(2);
      ((struct Actor *)object)->gravity = px(1) / 4;
      ((struct Actor *)object)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == 3)
      wait(60);
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);
    if (flags == 3)
      wait(80);
    Actor_SelectAnimationFar(owner, ANIM_STAND);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames;
/// flags bit 0 adds sparkles and bit 1 raises it as a found item.
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *owner = Actor_Lookup(Party_GetLeaderActor());
  u8 *buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  struct Actor *object = Actor_CreateFar(
      489, owner->x.fixed, owner->y.fixed + px(36), owner->z.fixed);

  if (object != 0) {
    struct Sprite *sprite = object->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, 4 * TILE_SIZE_4BPP, buffer + 1024);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & 1)
      object->update = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);

    wait(80);
    Actor_SelectAnimationFar(owner, ANIM_STAND);
    Actor_ReleaseFar(object);
  }
}

/// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
/// pixels of it.
void Reveal_EmitSparkles(struct Actor *source) {
  struct ParticlePosition position;
  struct Actor *object;
  u32 randomRadius;

  if ((100 * Random16() >> 16) > 9)
    return;

  SET_POSITION(&position, source->x.fixed, source->y.fixed, source->z.fixed);
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), &position);
  object = Actor_Spawn(SPRITE_SPARKLE, position.x, position.y, position.z);
  Actor_InitializeSparkle(object);
}

// Places the scene's markers from its scene item rows (a type, an event id, a
// flag and a pad): for each of the first 16 rows whose flag is clear and whose
// event has not run, a marker object goes at its event object, or at the map
// cell of a tile event, when that point is on screen.

/// Places a marker at each waiting scene item event that is on screen.
void FieldEffect_SpawnRevealMarkers(void) {
  // The row type is s16, as the rows are: with it the copied head test stays
  // apart from the bottom one.
  s16 *entry = SceneItem_GetRows();
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0] & FIELD_PIXEL_MASK;
  s32 originZ = origin[1] & FIELD_PIXEL_MASK;
  s32 result[3];
  /* FAKEMATCH(volatile): headerWord lives at sp+4, stored once. */
  volatile s32 headerWord;
  struct Actor *object;
  s16 type;
  s32 id;
  s32 flag;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 dx;
  s32 dz;

  // DEAD: the map header's second word is never read; the ROM keeps its load
  // and store.
  headerWord = (*(s32 **)map)[1];
  if (entry == NULL)
    return;
  for (i = 0; i <= 15 && (type = *entry++) != 0 && !(type & 0x4000); i++) {
    id = entry[0];
    flag = entry[1];
    entry += 3;
    if (type != 128 && flag != -1 && GameFlag_Get(flag))
      continue;
    if (SceneItem_FindPosition(id, result))
      continue;
    object = TreasureSpot_GetActor(i + 64);
    if (object == NULL)
      continue;
    // A tile event's object holds its map cell, marked by this value.
    if (object->riseBaseY == 0x31415927) {
      x = object->timer << 16;
      z = (s16)object->userValue << 16;
      y = MapGrid_GetTerrainHeightFar(0, x, z);
    } else {
      if ((u16)(type - 128) <= 1)
        continue;
      if (type == 130 || type == 132 || type == 133 || type == 131)
        continue;
      x = object->x.fixed;
      y = object->y.fixed;
      z = object->z.fixed;
    }
    dx = x - originX;
    dz = z - originZ - y;
    dx >>= 16;
    dz >>= 16;
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Get(flag))
      continue;
    object = Actor_CreateFar(SPRITE_SPARKLE, x, y, z);
    if (object != NULL) {
      object->motionFlags = 0;
      Actor_StartScriptFar((struct Actor *)object,
                           gBattleEffect_MarkerParticleScript);
      Actor_SetSpriteFlagsFar((struct Actor *)object, 0);
      Actor_SelectAnimationFar(object, 1);
      ((struct Sprite *)object->sprite)->image.priority = 1;
    }
  }
}
