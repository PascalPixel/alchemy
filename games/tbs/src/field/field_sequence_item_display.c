/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// ItemDisplay_ShowPicture's and ItemDisplay_ShowItem's flags.
enum ItemDisplayFlag {
  ITEM_DISPLAY_SPARKLES = 1, ///< sparkles appear round the picture
  ITEM_DISPLAY_RAISED = 2    ///< the leader holds it up over his head
};

/// Keeps the party leader off the treasure spots: a leader within 16 pixels of
/// a treasure spot's centre is moved 20 pixels from that centre, in the
/// direction it stood, and loses its move target.
void TreasureSpot_MoveLeaderAway(void) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct TreasureSpot *marker = gFieldWork->treasureSpots;
  s32 markerIndex = 0;
  FIXED x;
  FIXED z;
  s32 cellX;
  s32 cellZ;
  FIXED dx;
  FIXED dz;

  // The slots are filled in order, so the first with id 0 ends the list.
  if (marker->id != 0) {
    x = leader->x.fixed;
    z = leader->z.fixed;
    for (;;) {
      cellX = marker->column * toFIXED(MAPGRID_CELL_PIXELS);
      dx = x - cellX - toFIXED(8);
      cellZ = marker->row * toFIXED(MAPGRID_CELL_PIXELS);
      dz = z - cellZ - toFIXED(8);
      if ((dx > -toFIXED(MAPGRID_CELL_PIXELS) &&
           dx < toFIXED(MAPGRID_CELL_PIXELS)) &&
          (dz > -toFIXED(MAPGRID_CELL_PIXELS) &&
           dz < toFIXED(MAPGRID_CELL_PIXELS))) {
        leader->x.fixed = cellX + toFIXED(8);
        leader->z.fixed = cellZ + toFIXED(8);
        Vector_AddPolarOffset(toFIXED(20), (u16)Trig_Atan(dz, dx),
                              (struct Vector3 *)&leader->x.fixed);
        leader->targetX = ACTOR_NO_TARGET;
        leader->targetY = ACTOR_NO_TARGET;
        leader->targetZ = ACTOR_NO_TARGET;
        return;
      }
      markerIndex++;
      marker++;
      if (markerIndex > TREASURE_SPOT_MAX - 1 || marker->id == 0)
        break;
    }
  }
}

/// Starts a sparkle twinkling, drawn at priority 1; one that could not be
/// spawned is left out.
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

  // Once the picture's rise has all but stopped, clearing its motion flags
  // (ACTOR_FALLS and ACTOR_FOLLOWS_TERRAIN) ends gravity, so it hangs at the
  // top of its rise.
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

  s32 source = (s32)*(struct Actor **)(work + actorOffset);
  union PolarOffsetPosition picturePosition;
  FIXED pictureX;

  if (source == 0)
    return 0;

  picturePosition.vector.x = ((struct Actor *)source)->x.fixed;
  picturePosition.vector.y = ((struct Actor *)source)->y.fixed;
  picturePosition.vector.z = ((struct Actor *)source)->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), ((struct Actor *)source)->facing,
                        picturePosition.coordinates);
  pictureX = (picturePosition.vector.x & FIELD_CELL_MASK) + toFIXED(8);
  // source now holds the cell's z, no longer the actor.
  source = (picturePosition.vector.z & FIELD_CELL_MASK) + toFIXED(8);

  if (((struct FieldWork *)work)->revealActive != 0) {
    // Stop both the marker emitters and the sparkle actors they already made.
    struct Actor *effect = gActors;

    // work now counts the actors left.
    work = ACTOR_COUNT - 1;

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
  picture = Actor_CreateFar(SPRITE_BLANK, pictureX, toFIXED(16), source);
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

    picture->velocityY = toFIXED(2);
    picture->gravity = toFIXED(1) / 4;

    // Draw the item's icon into HEAP_SLOT_GLYPH_TRANSFER, then send its tiles
    // to the sprite's.
    iconBuffer =
        HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItemFar(item);

    sprite->image.tile =
        VramCache_Load(sprite->slot, ICON_TILE_BYTES,
                       (u8 *)iconBuffer + offsetof(GlyphTransfer, tiles));

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
  actor->acceleration = toFIXED(1);
  actor->speedLimit = toFIXED(2);
  actor->motionFlags = 0;
  Actor_SetMoveTargetFar(actor, leader->x.fixed, leader->y.fixed + toFIXED(36),
                         leader->z.fixed);
  wait(3);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_ARMS_RAISED);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  // FACING_SOUTH, a quarter turn, faces the screen.
  leader->facing = FACING_SOUTH;
}

/// Shows a found item's picture, with sparkles (ITEM_DISPLAY_SPARKLES) or held
/// up by the leader (ITEM_DISPLAY_RAISED); with both it sparkles for 60 frames
/// before he raises it, and he holds it 80.
void ItemDisplay_ShowPicture(void *object, s32 flags) {
  void *leader;

  if (object != NULL) {
    leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
    if (flags & ITEM_DISPLAY_SPARKLES) {
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_StartScriptFar(object, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)object)->velocityY = toFIXED(2);
      ((struct Actor *)object)->gravity = toFIXED(1) / 4;
      ((struct Actor *)object)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED)) {
      wait(60);
    }
    if (flags & ITEM_DISPLAY_RAISED) {
      ItemDisplay_RaiseAboveLeader(object);
    }
    if (flags == (ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED)) {
      wait(80);
    }
    Actor_SelectAnimationFar(leader, ANIM_STAND);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames,
/// with sparkles (ITEM_DISPLAY_SPARKLES) or held up as a found item
/// (ITEM_DISPLAY_RAISED).
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  void *iconBuffer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  struct Actor *actor =
      Actor_CreateFar(SPRITE_BLANK, leader->x.fixed,
                      leader->y.fixed + toFIXED(36), leader->z.fixed);

  if (actor != 0) {
    struct Sprite *sprite = actor->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES,
                   (u8 *)iconBuffer + offsetof(GlyphTransfer, tiles));
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

// The German, Spanish, French and Italian editions stop Reveal's sparkles
// while a menu is open.
#if TBS_PATCH_2

/// While Reveal is active, releases all its sparkles, leaving its marks.
void FieldEffect_ReleaseRevealSparkles(void) {
  if (gFieldWork->revealActive != 0) {
    struct Actor *effect = gActors;
    s32 remainingActors = ACTOR_COUNT - 1;

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

  // The layout's treasure spots: (column, row, id) triples, ended by
  // TREASURE_SPOT_LIST_END twice, as Field_PlaceTreasureSpots reads them.
  markerData = ((struct MapGridScrollWork *)gMapGridWork[0])->markers;
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  leaderColumn = FIELD_CELL(actor->x.fixed);
  leaderRow = FIELD_CELL(actor->z.fixed);
  if (markerData != 0) {
    while (column = *markerData++, row = *markerData++,
           column != TREASURE_SPOT_LIST_END || row != TREASURE_SPOT_LIST_END) {
      id = *markerData++;
      if (TreasureSpot_GetActor(id) != NULL)
        continue;
      if (id < TREASURE_SPOT_FIRST_ID || id > TREASURE_SPOT_LAST_ID)
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
      for (; entry->kind != -1; entry++) {
        if (entry->subject == id &&
            (entry->kind & TRIGGER_KIND_MASK) == TRIGGER_EXAMINE) {
          switch (entry->value & FIND_TYPE_MASK) {
          case 0:
          case FIND_ITEM(0):
          case FIND_COINS(0):
          case FIND_PSYNERGY_STONE:
          case FIND_MIMIC:
            if (entry->condition != -1 &&
                GameFlag_GetFar(entry->condition) == 0) {
              // actor now holds the spawned marker.
              actor = Actor_CreateFar(
                  SPRITE_BLANK,
                  column * toFIXED(MAPGRID_CELL_PIXELS) + toFIXED(8), 0,
                  row * toFIXED(MAPGRID_CELL_PIXELS) + toFIXED(8));
              if (actor != 0) {
                Actor_StartScriptFar(actor, gFieldEffect_MarkerParticleScript);
                Actor_SetSpriteFlagsFar(actor, 0);
                actor->update = Reveal_EmitSparkles;
              }
            }
            break;
          }
        }
      }
    }
  }
}
