/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

STATIC_ASSERT(offsetof(struct Actor, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct Actor, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct Actor, sprite) == 0x50);

/// ActorSystem_Initialize allocates the 64 actors in
/// HEAP_SLOT_ACTORS.
#define EFFECT_OBJECT_COUNT 64
#define EFFECT_OBJECT_POOL gActors

/// Keeps the party leader off the treasure spots: a leader within 16 pixels of
/// a treasure spot's centre is moved 20 pixels from that centre, in the
/// direction it stood, and loses its move target.
void TreasureSpot_MoveLeaderAway(void) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct TreasureSpot *marker = gFieldWork->treasureSpots;
  s32 markerIndex = 0;
  s32 x;
  s32 z;
  s32 cellX;
  s32 cellZ;
  s32 dx;
  s32 dz;

  // The slots are filled in order, so the first with id 0 ends the list.
  if (marker->id != 0) {
    x = leader->x.fixed;
    z = leader->z.fixed;
    for (;;) {
      cellX = marker->column << 20;
      dx = x - cellX - px(8);
      cellZ = marker->row << 20;
      dz = z - cellZ - px(8);
      if ((dx >= -0xfffff && dx <= 0xfffff) &&
          (dz >= -0xfffff && dz <= 0xfffff)) {
        leader->x.fixed = cellX + px(8);
        leader->z.fixed = cellZ + px(8);
        Vector_AddPolarOffset(px(20), (u16)ArcTan2(dz, dx),
                              (struct Vector3 *)&leader->x.fixed);
        leader->targetX = ACTOR_NO_TARGET;
        leader->targetY = ACTOR_NO_TARGET;
        leader->targetZ = ACTOR_NO_TARGET;
        return;
      }
      markerIndex++;
      marker++;
      if (markerIndex > 9 || marker->id == 0)
        break;
    }
  }
}

/// Starts the particle script and its first child at priority 1, if the spawn
/// succeeded. Allocation failure leaves the sparkle absent.
static inline void Actor_InitializeSparkle(struct Actor *actor) {
  if (actor != 0) {
    Actor_StartScriptFar(actor, gFieldEffect_ParticleScript);
    Actor_SelectAnimationFar(actor, 0);
    actor->sprite->image.priority = 1;
  }
}

/// Each frame of a found item's picture: one time in ten, a sparkle appears
/// within 16 pixels of it.
void ItemDisplay_EmitSparkles(struct Actor *emitter) {
  union PolarOffsetPosition position;
  struct Actor *object;
  u32 randomRadius;

  // Once the picture's rise has all but stopped (under 1/256 pixel a frame),
  // clearing its motion flags (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN)
  // ends gravity, so it hangs at the top of its rise.
  if (emitter->velocityY >= -255 && emitter->velocityY <= 255)
    emitter->motionFlags = 0;

  // Test the chance before drawing the radius and angle from the RNG.
  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = emitter->x.fixed;
  position.vector.y = emitter->y.fixed;
  position.vector.z = emitter->z.fixed;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  object = Actor_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                       position.vector.z);
  Actor_InitializeSparkle(object);
}

/// Shows the picture of item (0 for none) at the centre of the cell 16 pixels
/// in front of character actorId, at a height of 16 pixels whatever the
/// actor's, with sparkles.
struct Actor *ItemDisplay_ShowItemInFront(s32 actorId, s32 item) {
  struct Actor *picture;

  s32 work = (s32)gFieldWork;
  u32 actorOffset = (actorId * 4) + offsetof(struct FieldWork, actors);

  s32 srcZ = (s32)*(struct Actor **)(work + actorOffset);
  union PolarOffsetPosition picturePosition;
  s32 pictureX;

  if (srcZ == 0)
    return 0;

  picturePosition.vector.x = ((struct Actor *)srcZ)->x.fixed;
  picturePosition.vector.y = ((struct Actor *)srcZ)->y.fixed;
  picturePosition.vector.z = ((struct Actor *)srcZ)->z.fixed;
  Vector_AddPolarOffset(px(16), ((struct Actor *)srcZ)->facing,
                        picturePosition.coordinates);
  pictureX = (picturePosition.vector.x & FIELD_CELL_MASK) + px(8);
  // srcZ now holds the cell's z.
  srcZ = (picturePosition.vector.z & FIELD_CELL_MASK) + px(8);

  if (((struct FieldWork *)work)->revealActive != 0) {
    // Stop both the marker emitters and the sparkle actors they already made.
    struct Actor *effect = EFFECT_OBJECT_POOL;

    // work now counts the actors left.
    work = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->script != 0) {
        if (effect->update == Reveal_EmitSparkles) {
          Actor_ReleaseFar(effect);
        }
        if (effect->script == gFieldEffect_ParticleScript) {
          Actor_ReleaseFar(effect);
        }
      }
      work--;
      effect++;
    } while (work >= 0);
  }

  wait(3);
  picture = Actor_CreateFar(SPRITE_BLANK, pictureX, px(16), srcZ);
  if (picture == 0)
    return 0;

  Actor_StartScriptFar(picture, gFieldEffect_ParticleEmitterScript);
  {
    struct Sprite *sprite = picture->sprite;
    void *iconBuffer;

    // No shadow, outline or sprite animations: a 16-colour sprite in OBJ
    // palette 0 whose tiles are the item's icon.
    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    picture->velocityY = px(2);
    picture->gravity = px(1) / 4;

    // Draw the item's icon into HEAP_SLOT_GLYPH_TRANSFER, then send it to the
    // sprite's tiles: 128 bytes from byte 1,024 of the block.
    iconBuffer =
        HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItemFar(item);

    sprite->image.tile =
        VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 1024);

    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    picture->update = ItemDisplay_EmitSparkles;
  }

  return picture;
}

/// Releases object unless it is NULL.
void Actor_ReleaseIfPresent(void *object) {
  if (object != 0)
    Actor_ReleaseFar(object);
}

/// Raises a found item's picture to 36 pixels above the party leader, who holds
/// it up (ANIM_LEADER_ARMS_RAISED) facing the screen.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *leader;

  if (actor == 0)
    return;

  leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  // Up to 2 pixels a frame, gaining 1 a frame, in free flight: no motion
  // flags, so it neither falls nor follows the ground.
  actor->acceleration = px(1);
  actor->speedLimit = px(2);
  actor->motionFlags = 0;
  Actor_SetMoveTargetFar(actor, leader->x.fixed, leader->y.fixed + px(36),
                         leader->z.fixed);
  wait(3);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_ARMS_RAISED);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  // FACING_SOUTH, a quarter turn, faces the screen.
  leader->facing = FACING_SOUTH;
}

/// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
/// above the leader, with waits of 60 and 80 frames around that when both are
/// set.
void ItemDisplay_ShowPicture(void *object, s32 flags) {
  void *leader;

  if (object != NULL) {
    leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
    if (flags & 1) {
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_StartScriptFar(object, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)object)->velocityY = px(2);
      ((struct Actor *)object)->gravity = px(1) / 4;
      ((struct Actor *)object)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == 3) {
      wait(60);
    }
    if (flags & 2) {
      ItemDisplay_RaiseAboveLeader(object);
    }
    if (flags == 3) {
      wait(80);
    }
    Actor_SelectAnimationFar(leader, ANIM_STAND);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames;
/// flags bit 0 adds sparkles and bit 1 raises it as a found item.
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  void *iconBuffer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  struct Actor *actor = Actor_CreateFar(
      SPRITE_BLANK, leader->x.fixed, leader->y.fixed + px(36), leader->z.fixed);

  if (actor != 0) {
    struct Sprite *sprite = actor->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 1024);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & 1)
      actor->update = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(actor);

    wait(80);
    Actor_SelectAnimationFar(leader, ANIM_STAND);
    Actor_ReleaseFar(actor);
  }
}

// The German, Spanish, French and Italian editions stop Reveal's sparkles
// while a menu is open.
#if TBS_PATCH_2

/// While Reveal is active, releases all its sparkles, leaving its marks.
void FieldEffect_ReleaseRevealSparkles(void) {
  if (gFieldWork->revealActive != 0) {
    struct Actor *effect = EFFECT_OBJECT_POOL;
    s32 remainingActors = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->script != 0) {
        if (effect->script == gFieldEffect_ParticleScript)
          Actor_ReleaseFar(effect);
      }
      remainingActors--;
      effect++;
    } while (remainingActors >= 0);
  }
}

#endif

/// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
/// pixels of it.
void Reveal_EmitSparkles(struct Actor *actor) {
  const struct Actor *source = (const struct Actor *)actor;
  union PolarOffsetPosition position;
  struct Actor *object;
  u32 randomRadius;

#if TBS_PATCH_2
  if (gFieldWork->menuOpen != 0)
    return;
#endif
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

/// Reveal: marks with sparkles the hidden things within 8 cells left or right
/// and 5 up or down of the leader: treasure spots (ids 100 to 239) with no
/// actor placed, whose examine trigger (TRIGGER_EXAMINE) gives an item, coins,
/// a Psynergy Stone or a mimic and whose flag is not yet set.
void FieldEffect_SpawnRevealMarkers(void) {
  u8 *markerData;
  struct Actor *actor;
  const struct Trigger *entry;
  s32 leaderColumn;
  s32 leaderRow;
  s32 column;
  s32 row;
  s32 id;

  // The layout's treasure spots: (column, row, id) triples, ended by 0xff,
  // 0xff; the same list as MapGridMarkers's markers in
  // field_sequence_treasure_spots.c.
  markerData = *(u8 **)(((u8 *)gMapGridWork[0]) + 16);
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  leaderColumn = FIELD_CELL(actor->x.fixed);
  leaderRow = FIELD_CELL(actor->z.fixed);
  if (markerData != 0) {
    while (column = *markerData++, row = *markerData++,
           column != 0xff || row != 0xff) {
      id = *markerData++;
      if (TreasureSpot_GetActor(id) != NULL)
        continue;
      if (id < 100 || id > 239)
        continue;
      if (leaderColumn - column >= 0) {
        if (leaderColumn - column > 8)
          continue;
      } else if (column - leaderColumn > 8)
        continue;
      if (leaderRow - row >= 0) {
        if (leaderRow - row > 5)
          continue;
      } else if (row - leaderRow > 5)
        continue;
      entry = gOverlayArea.triggers();
      if (entry->kind == -1)
        continue;
      do {
        if (entry->subject == id && (entry->kind & 15) == TRIGGER_EXAMINE) {
          switch (entry->value & FIND_TYPE_MASK) {
          case 0:
          case FIND_ITEM(0):
          case FIND_COINS(0):
          case FIND_PSYNERGY_STONE:
          case FIND_MIMIC:
            if (entry->condition != -1 &&
                GameFlag_GetFar(entry->condition) == 0) {
              // actor now holds the spawned marker.
              actor = Actor_CreateFar(SPRITE_BLANK, (column << 20) + px(8), 0,
                                      (row << 20) + px(8));
              if (actor != 0) {
                Actor_StartScriptFar(actor, gFieldEffect_MarkerParticleScript);
                Actor_SetSpriteFlagsFar(actor, 0);
                (actor)->update = Reveal_EmitSparkles;
              }
            }
            break;
          }
        }
        entry++;
      } while (entry->kind != -1);
    }
  }
}

#undef EFFECT_OBJECT_COUNT
#undef EFFECT_OBJECT_POOL
