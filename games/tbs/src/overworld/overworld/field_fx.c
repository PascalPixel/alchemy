/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of overworld.c.

#include "../overworld_internal.h"

STATIC_ASSERT(offsetof(struct Actor, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct Actor, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct Actor, sprite) == 0x50);

/// ActorSystem_Initialize allocates the 64 actors in
/// HEAP_SLOT_ACTORS.
#define EFFECT_OBJECT_COUNT 64
#ifdef UBFIX
#define EFFECT_OBJECT_POOL gActors
#else
// UB: gActors and gFieldWork are separate globals. The named pool load
// changes register allocation and shrinks StartRandomParticleEmitter from 0x158
// to 0x156 bytes in tbs-en; keep the native read here.
#define EFFECT_OBJECT_POOL (*(struct Actor **)((u8 *)&gFieldWork - 88))
#endif

/// Keeps the party leader off the treasure spots: a leader within 16 pixels of
/// a treasure spot's centre is moved 20 pixels from that centre, in the
/// direction it stood, and loses its move target. Only the first treasure spot
/// it stands on moves it. Called by two scene overlays (Suhalla Desert, Lamakan
/// Desert).
void TreasureSpot_MoveLeaderAway(void) {
  struct Actor *leader = Actor_Lookup(gCommon.leaderActorId);
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
      dx = x - cellX - (8 << 16);
      cellZ = marker->row << 20;
      dz = z - cellZ - (8 << 16);
      if ((dx >= -0xfffff && dx <= 0xfffff) &&
          (dz >= -0xfffff && dz <= 0xfffff)) {
        leader->x.fixed = cellX + (8 << 16);
        leader->z.fixed = cellZ + (8 << 16);
        Vector_AddPolarOffset(20 << 16, (u16)ArcTan2(dz, dx),
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
/// succeeded. Allocation failure leaves the sparkle absent. The script sinks
/// the sparkle a quarter pixel a frame for 60 frames, then releases it.
static inline void Actor_InitializeSparkle(struct Actor *actor) {
  // Sharing the offset and spawn as well changes literal/argument load order in
  // both emitters, without changing their sizes.
  if (actor != 0) {
    Actor_StartScriptFar(actor, gFieldEffect_ParticleScript);
    Actor_SelectAnimationFar(actor, 0);
    actor->sprite->image.priority = 1;
  }
}

/// Each frame of a found item's picture: one time in ten, a sparkle appears
/// within 16 pixels of it. Installed as the picture's update routine by
/// FieldEffect_StartRandomParticleEmitter, ItemDisplay_ShowPicture
/// and ItemDisplay_ShowItem.
void ItemDisplay_EmitSparkles(struct Actor *actor) {
  struct Actor *emitter = (struct Actor *)actor;
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
/// actor's,
/// with sparkles. While Reveal is active, its marks and sparkles are cleared
/// first. Returns the picture's actor, or 0 without the actor or
/// an empty actor slot. The picture is sprite 22, thrown up (velocityY 2 << 16,
/// gravity 0x4000: it rises for 8 frames) while it grows to full size, its icon
/// drawn through HEAP_SLOT_GLYPH_TRANSFER, which is released again, and loaded
/// into VRAM. The caller owns the picture: Field_ExamineCell releases it once
/// the find is told. Called by Field_ExamineCell for coins and items found, and
/// by FieldEffect_PlayCueAndStartEmitterOnTarget.
struct Actor *FieldEffect_StartRandomParticleEmitter(s32 actorId, s32 item) {
  struct Actor *picture;

  s32 work = (s32)gFieldWork;
  u32 actorOffset = (actorId * 4) + 0x14; // FieldWork's actors

  s32 srcZ = (s32)*(struct Actor **)(work + actorOffset);
  union PolarOffsetPosition picturePosition;
  s32 pictureX;

  if (srcZ == 0)
    return 0;

  picturePosition.vector.x = ((struct Actor *)srcZ)->x.fixed;
  picturePosition.vector.y = ((struct Actor *)srcZ)->y.fixed;
  picturePosition.vector.z = ((struct Actor *)srcZ)->z.fixed;
  Vector_AddPolarOffset(16 << 16, ((struct Actor *)srcZ)->facing,
                        picturePosition.coordinates);
  pictureX = (picturePosition.vector.x & 0xfff00000) + (8 << 16);
  // srcZ now holds the cell's z.
  srcZ = (picturePosition.vector.z & 0xfff00000) + (8 << 16);

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

  WaitFrames(3);
  picture = Actor_CreateFar(SPRITE_BLANK, pictureX, 16 << 16, srcZ);
  if (picture == 0)
    return 0;

  Actor_StartScriptFar(picture, gFieldEffect_ParticleEmitterScript);
  {
    struct Sprite *sprite = picture->sprite;
    void *iconBuffer;

    // No shadow, outline or sprite anims: a 16-colour sprite in OBJ
    // palette 0 whose tiles are the item's icon.
    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    picture->velocityY = 0x20000;
    picture->gravity = 0x4000;

    // Draw the item's icon into HEAP_SLOT_GLYPH_TRANSFER, then send it to the
    // sprite's tiles: 128 bytes from offset 0x400 of the block.
    iconBuffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItemFar(item);

    sprite->image.tile =
        VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 0x400);

    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
    picture->update = ItemDisplay_EmitSparkles;
  }

  return picture;
}

/// Releases object unless it is NULL. Called by Field_ExamineCell to drop the
/// picture of an item nobody could carry.
void Actor_ReleaseIfPresent(void *object) {
  if (object != 0)
    Actor_ReleaseFar(object);
}

/// Raises a found item's picture to 36 pixels above the party leader, who
/// holds it up (ANIM_LEADER_ARMS_RAISED) facing the screen. Called by
/// Field_ExamineCell for what a search finds, by ItemDisplay_ShowPicture and by
/// ItemDisplay_ShowItem.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *leader;

  if (actor == 0)
    return;

  leader = Actor_Lookup(gCommon.leaderActorId);
  // Up to 2 pixels a frame, gaining 1 a frame, in free flight: no motion
  // flags, so it neither falls nor follows the ground.
  actor->acceleration = 1 << 16;
  actor->speedLimit = 2 << 16;
  actor->motionFlags = 0;
  Actor_SetMoveTargetFar(actor, leader->x.fixed, leader->y.fixed + (36 << 16),
                         leader->z.fixed);
  WaitFrames(3);
  Actor_SelectAnimationFar(leader, ANIM_LEADER_ARMS_RAISED);
  Actor_StartScriptFar(actor, gFieldEffect_RisingPulseScript);
  // Angle 0x4000, a quarter turn, faces the screen.
  leader->facing = FACING_SOUTH;
}

/// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
/// above the leader, with waits of 60 and 80 frames around that when both are
/// set. The leader then returns to animation 1. Scene overlays call it (six of
/// them).
void ItemDisplay_ShowPicture(void *object, s32 flags) {
  void *leader;

  if (object != NULL) {
    leader = Actor_Lookup(gCommon.leaderActorId);
    if (flags & 1) {
      Actor_SetSpriteFlagsFar(object, 0);
      Actor_StartScriptFar(object, gFieldEffect_ParticleEmitterScript);
      ((struct Actor *)object)->velocityY = 0x20000;
      ((struct Actor *)object)->gravity = 0x4000;
      ((struct Actor *)object)->update = ItemDisplay_EmitSparkles;
    }
    if (flags == 3) {
      WaitFrames(60);
    }
    if (flags & 2) {
      ItemDisplay_RaiseAboveLeader(object);
    }
    if (flags == 3) {
      WaitFrames(80);
    }
    Actor_SelectAnimationFar(leader, 1);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames;
/// flags bit 0 adds sparkles and bit 1 raises it as a found item. Then it
/// releases the picture and the leader returns to animation 1. Scene overlays
/// call it (41 of them). When no actor is
/// free, heap block 17, taken for the icon, is not released (the scene change's
/// heap reset frees it).
void ItemDisplay_ShowItem(s32 item, s32 flags) {
  struct Actor *leader = Actor_Lookup(gCommon.leaderActorId);
  void *iconBuffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  struct Actor *actor =
      Actor_CreateFar(SPRITE_BLANK, leader->x.fixed,
                      leader->y.fixed + (36 << 16), leader->z.fixed);

  if (actor != 0) {
    struct Sprite *sprite = actor->sprite;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 0x400);
    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & 1)
      actor->update = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(actor);

    WaitFrames(80);
    Actor_SelectAnimationFar(leader, 1);
    Actor_ReleaseFar(actor);
  }
}

// The German, Spanish, French and Italian editions stop Reveal's sparkles
// while a menu is open.
#if TBS_PATCH_2

#define REVEAL_SPARKLES_PAUSE_FOR_MENU 1
#endif

#if defined(REVEAL_SPARKLES_PAUSE_FOR_MENU)
/// In gFieldWork, the byte after queuedSound: FieldMenu_Run sets it
/// once FieldMenu_SelectCommand first returns, a cancel included, and clears it
/// as it returns.
#define MENU_OPEN_OFFSET 0xcca

/// While Reveal is active, releases all its sparkles, leaving its marks. Called
/// by the field menu (FieldMenu_Run, window/selection.c) each time
/// FieldMenu_SelectCommand returns, a cancel included, while Reveal is active.
void FieldEffect_ClearRandomParticles(void) {
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
/// pixels of it. Installed as the marker's update routine by
/// FieldEffect_SpawnNearbyMarkers.
void Reveal_EmitSparkles(struct Actor *actor) {
  const struct Actor *source = (const struct Actor *)actor;
  union PolarOffsetPosition position;
  struct Actor *object;
  u32 randomRadius;

#if defined(REVEAL_SPARKLES_PAUSE_FOR_MENU)
  if (*(s8 *)((u8 *)gFieldWork + MENU_OPEN_OFFSET) != 0)
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

/// Returns 1 while Reveal is active, else 0. Run by a Reveal marker's actor
/// script (gFieldEffect_MarkerParticleScript) through SC_CALL: the marker stays
/// while it returns 1 and releases itself once it returns 0.
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
/// a Psynergy Stone or a mimic and whose flag is not yet set. Each mark is an
/// actor of sprite 22 running gFieldEffect_MarkerParticleScript. Called by
/// Reveal (event_commands/reveal.c).
void FieldEffect_SpawnNearbyMarkers(void) {
  u8 *markerData;
  struct Actor *actor;
  const struct Trigger *entry;
  s32 leaderColumn;
  s32 leaderRow;
  s32 column;
  s32 row;
  s32 id;

  // The layout's treasure spots: (column, row, id) triples, ended by 0xff,
  // 0xff; the same list as MapGridMarkers's markers in markers.c.
  markerData = *(u8 **)(((u8 *)gMapGridWork[0]) + 16);
  actor = Actor_Lookup(gCommon.leaderActorId);
  leaderColumn = actor->x.fixed >> 20;
  leaderRow = actor->z.fixed >> 20;
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
      if (entry->kind == (u32)-1)
        continue;
      do {
        if (entry->subject == id && (entry->kind & 15) == TRIGGER_EXAMINE) {
          switch (entry->value & 0xfff00000) {
          case 0:
          case FIND_ITEM(0):
          case FIND_COINS(0):
          case FIND_PSYNERGY_STONE:
          case FIND_MIMIC:
            if (entry->condition != -1 &&
                GameFlag_GetFar(entry->condition) == 0) {
              /* FAKEMATCH(temporary): the spawned marker reuses actor, so
               * the one pseudo keeps Actor_Lookup's result in r5;
               * as a variable of its own, 9 of 163 instructions differ in
               * tbs-en. */
              actor = Actor_CreateFar(SPRITE_BLANK, (column << 20) + (8 << 16),
                                      0, (row << 20) + (8 << 16));
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
      } while (entry->kind != (u32)-1);
    }
  }
}

#undef EFFECT_OBJECT_COUNT
#undef EFFECT_OBJECT_POOL
#undef REVEAL_SPARKLES_PAUSE_FOR_MENU
#undef MENU_OPEN_OFFSET
