/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of overworld.c.

#include "../overworld_internal.h"

STATIC_ASSERT(offsetof(struct Object, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct Object, flags) == 0x55);
STATIC_ASSERT(offsetof(struct Object, animation) == 0x50);

/// An object of the 64-object pool (struct Object, 0x70 bytes), seen
/// as the found item's picture, the sparkles and the Reveal marks use it.
struct EffectObject {
  /// The script pointer, as a word; 0 while the object is free.
  s32 scriptWord;
  u8 pad0[0x24];
  s32 velocityY; ///< 16.16 fixed-point pixels a frame; up is positive
  u8 pad1[0x1c];
  /// 16.16 fixed-point pixels taken off velocityY each frame while the object
  /// falls (motion bit 1); 0x10000 when created.
  s32 gravity;
  u8 pad2[4];
  struct Sprite *animation;
  u8 pad3[24];
  void (*updateHook)(
      union FieldObject *fieldObject); ///< frame hook at offset 0x6c
};

STATIC_ASSERT(sizeof(struct EffectObject) == 0x70);
STATIC_ASSERT(offsetof(struct EffectObject, updateHook) == 0x6c);

/// ObjectSystem_Initialize allocates the 64 objects in
/// HEAP_SLOT_OBJECTS.
#define EFFECT_OBJECT_COUNT 64
#ifdef UBFIX
#define EFFECT_OBJECT_POOL ((struct EffectObject *)gObjects)
#else
// UB: gObjects and gFieldWork are separate globals. The named pool load
// changes register allocation and shrinks StartRandomParticleEmitter from 0x158
// to 0x156 bytes in tbs-en; keep the native read here.
#define EFFECT_OBJECT_POOL (*(struct EffectObject **)((u8 *)&gFieldWork - 88))
#endif

/// Keeps the party leader off the treasure spots: a leader within 16 pixels of
/// a treasure spot's centre is moved 20 pixels from that centre, in the
/// direction it stood, and loses its move target. Only the first treasure spot
/// it stands on moves it. Called by two map overlays (Suhalla Desert, Lamakan
/// Desert).
void TreasureSpot_MoveLeaderAway(void) {
  struct Object *leader = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  struct TreasureSpot *marker = ((struct FieldWork *)gFieldWork)->treasureSpots;
  s32 markerIndex = 0;
  s32 x;
  s32 z;
  s32 cellX;
  s32 cellZ;
  s32 dx;
  s32 dz;

  // The slots are filled in order, so the first with id 0 ends the list.
  if (marker->id != 0) {
    x = leader->x;
    z = leader->z;
    for (;;) {
      cellX = marker->column << 20;
      dx = x - cellX - (8 << 16);
      cellZ = marker->row << 20;
      dz = z - cellZ - (8 << 16);
      if ((dx >= -0xfffff && dx <= 0xfffff) &&
          (dz >= -0xfffff && dz <= 0xfffff)) {
        leader->x = cellX + (8 << 16);
        leader->z = cellZ + (8 << 16);
        Vector_AddPolarOffset(20 << 16, (u16)ArcTan2(dz, dx),
                              (struct EffectVector *)&leader->x);
        leader->targetX = OBJ_EVENT_NO_TARGET;
        leader->targetY = OBJ_EVENT_NO_TARGET;
        leader->targetZ = OBJ_EVENT_NO_TARGET;
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
/// succeeded. Allocation failure leaves the effect absent. The script sinks
/// the sparkle a quarter pixel a frame for 60 frames, then frees it.
static inline void Object_InitializeSparkle(struct Object *object) {
  // Sharing the offset and spawn as well changes literal/argument load order in
  // both emitters, without changing their sizes.
  if (object != 0) {
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 gFieldEffect_ParticleScript);
    ObjectDispatch_SelectAnimationFar(object, 0);
    ((struct Sprite *)object->animation)->image.priority = 1;
  }
}

/// Each frame of a found item's picture: one time in ten, a sparkle appears
/// within 16 pixels of it. Installed as the picture's updateHook by
/// FieldEffect_StartRandomParticleEmitter, ItemDisplay_ShowPicture
/// and FieldEffect_StartEffectObject22.
void ItemDisplay_EmitSparkles(union FieldObject *fieldObject) {
  struct Object *emitter = (struct Object *)fieldObject;
  union PolarOffsetPosition position;
  struct Object *object;
  u32 randomRadius;

  // Once the picture's rise has all but stopped (under 1/256 pixel a frame),
  // clearing its motion flags (OBJ_EVENT_FALLS and OBJ_EVENT_FOLLOWS_TERRAIN)
  // ends gravity, so it hangs at the top of its rise.
  if (emitter->velocityY >= -255 && emitter->velocityY <= 255)
    emitter->flags = 0;

  // Test the chance before drawing the radius and angle from the RNG.
  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = emitter->x;
  position.vector.y = emitter->y;
  position.vector.z = emitter->z;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  object = Object_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                        position.vector.z);
  Object_InitializeSparkle(object);
}

/// Shows the picture of item (0 for none) at the centre of the cell 16 pixels
/// in front of character localId, at a height of 16 pixels whatever the
/// object event's,
/// with sparkles. While Reveal is active, its marks and sparkles are cleared
/// first. Returns the picture's object event, or 0 without the object event or
/// a free object. The picture is object 22, thrown up (velocityY 2 << 16,
/// gravity 0x4000: it rises for 8 frames) while it grows to full size, its icon
/// drawn through HEAP_SLOT_GLYPH_TRANSFER, which is released again, and loaded
/// into VRAM. The caller owns the picture: Field_SearchCell releases it once
/// the find is told. Called by Field_SearchCell for coins and items found, and
/// by FieldEffect_PlayCueAndStartEmitterOnTarget.
struct EffectObject *FieldEffect_StartRandomParticleEmitter(s32 localId,
                                                            s32 item) {
  struct EffectObject *picture;

  s32 work = (s32)gFieldWork;
  u32 objectEventOffset = (localId * 4) + 0x14; // EventWork's objectEvents

  s32 srcZ = (s32)*(struct ObjectEvent **)(work + objectEventOffset);
  union PolarOffsetPosition picturePosition;
  s32 pictureX;

  if (srcZ == 0)
    return 0;

  picturePosition.vector.x = ((struct ObjectEvent *)srcZ)->x.fixed;
  picturePosition.vector.y = ((struct ObjectEvent *)srcZ)->y.fixed;
  picturePosition.vector.z = ((struct ObjectEvent *)srcZ)->z.fixed;
  Vector_AddPolarOffset(16 << 16, ((struct ObjectEvent *)srcZ)->facing,
                        picturePosition.coordinates);
  pictureX = (picturePosition.vector.x & 0xfff00000) + (8 << 16);
  // srcZ now holds the cell's z.
  srcZ = (picturePosition.vector.z & 0xfff00000) + (8 << 16);

  if (((struct FieldWork *)work)->revealActive != 0) {
    // Stop both the marker emitters and the sparkle objects they already made.
    struct EffectObject *effect = EFFECT_OBJECT_POOL;

    // work now counts the objects left.
    work = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->scriptWord != 0) {
        if (effect->updateHook == Reveal_EmitSparkles) {
          ObjectDispatch_ReleaseFar((struct DispatchObject *)effect);
        }
        if (effect->scriptWord == (s32)gFieldEffect_ParticleScript) {
          ObjectDispatch_ReleaseFar((struct DispatchObject *)effect);
        }
      }
      work--;
      effect++;
    } while (work >= 0);
  }

  WaitFrames(3);
  picture = (struct EffectObject *)Object_CreateFar(SPRITE_BLANK, pictureX,
                                                    16 << 16, srcZ);
  if (picture == 0)
    return 0;

  ObjectDispatch_InitializeFar((struct DispatchObject *)picture,
                               gFieldEffect_ParticleEmitterScript);
  {
    struct Sprite *sprite = picture->animation;
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
    picture->updateHook = ItemDisplay_EmitSparkles;
  }

  return picture;
}

/// Releases object unless it is NULL. Called by Field_SearchCell to drop the
/// picture of an item nobody could carry.
void Object_DestroyIfPresent(void *object) {
  if (object != 0)
    ObjectDispatch_ReleaseFar(object);
}

/// Raises a found item's picture to 36 pixels above the party leader, who
/// holds it up (animation 28) facing the screen. Called by Field_SearchCell for
/// what a search finds, by ItemDisplay_ShowPicture and by
/// FieldEffect_StartEffectObject22.
void ItemDisplay_RaiseAboveLeader(struct Object *object) {
  struct Object *leader;

  if (object == 0)
    return;

  leader = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  // Up to 2 pixels a frame, gaining 1 a frame, in free flight: no motion
  // flags, so it neither falls nor follows the ground.
  object->acceleration = 1 << 16;
  object->speedLimit = 2 << 16;
  object->flags = 0;
  Object_SetMoveTargetFar(object, leader->x, leader->y + (36 << 16), leader->z);
  WaitFrames(3);
  ObjectDispatch_SelectAnimationFar(leader, 28);
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gFieldEffect_RisingPulseScript);
  // Angle 0x4000, a quarter turn, faces the screen.
  leader->facing = 0x4000;
}

/// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
/// above the leader, with waits of 60 and 80 frames around that when both are
/// set. The leader then returns to animation 1. Map overlays call it (six of
/// them).
void ItemDisplay_ShowPicture(void *object, s32 flags) {
  void *leader;

  if (object != NULL) {
    leader = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
    if (flags & 1) {
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_InitializeFar(object, gFieldEffect_ParticleEmitterScript);
      ((struct EffectObject *)object)->velocityY = 0x20000;
      ((struct EffectObject *)object)->gravity = 0x4000;
      ((struct EffectObject *)object)->updateHook = ItemDisplay_EmitSparkles;
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
    ObjectDispatch_SelectAnimationFar(leader, 1);
  }
}

/// Shows the picture of item 36 pixels above the party leader for 80 frames;
/// flags bit 0 adds sparkles and bit 1 raises it as a found item. Then it
/// releases the picture and the leader returns to animation 1. Map overlays
/// call it (41 of them). When no object is
/// free, heap block 17, taken for the icon, is not released (the map change's
/// heap reset frees it).
void FieldEffect_StartEffectObject22(s32 item, s32 flags) {
  struct ObjectEvent *leader =
      ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  void *iconBuffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  struct EffectObject *object = (struct EffectObject *)Object_CreateFar(
      22, leader->x.fixed, leader->y.fixed + (36 << 16), leader->z.fixed);

  if (object != 0) {
    struct Sprite *sprite = object->animation;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Icon_DrawItemFar(item);
    VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 0x400);
    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);

    if (flags & 1)
      object->updateHook = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader((struct Object *)object);

    WaitFrames(80);
    ObjectDispatch_SelectAnimationFar(leader, 1);
    ObjectDispatch_ReleaseFar((struct DispatchObject *)object);
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

/// While Reveal is active, destroys all its sparkles, leaving its marks. Called
/// by the field menu (FieldMenu_Run, window/selection.c) each time
/// FieldMenu_SelectCommand returns, a cancel included, while Reveal is active.
void FieldEffect_ClearRandomParticles(void) {
  if (((struct FieldWork *)gFieldWork)->revealActive != 0) {
    struct EffectObject *effect = EFFECT_OBJECT_POOL;
    s32 remainingObjectEvents = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->scriptWord != 0) {
        if (effect->scriptWord == (s32)gFieldEffect_ParticleScript)
          ObjectDispatch_ReleaseFar((struct DispatchObject *)effect);
      }
      remainingObjectEvents--;
      effect++;
    } while (remainingObjectEvents >= 0);
  }
}

#endif

/// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
/// pixels of it. Installed as the marker's updateHook by
/// FieldEffect_SpawnNearbyMarkers.
void Reveal_EmitSparkles(union FieldObject *fieldObject) {
  const struct ObjectEvent *source = (const struct ObjectEvent *)fieldObject;
  union PolarOffsetPosition position;
  struct Object *object;
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
  object = Object_Spawn(SPRITE_SPARKLE, position.vector.x, position.vector.y,
                        position.vector.z);
  Object_InitializeSparkle(object);
}

/// Returns 1 while Reveal is active, else 0. Run by a Reveal marker's object
/// script (gFieldEffect_MarkerParticleScript) through SC_CALL: the marker stays
/// while it returns 1 and frees itself once it returns 0.
s32 Field_IsRevealActive(struct ObjectScriptContext *interpreter) {
  s16 revealState;
  u32 isActive = 0;
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}

/// Reveal: marks with sparkles the hidden things within 8 cells left or right
/// and 5 up or down of the leader: treasure spots (ids 100 to 239) with no
/// object placed, whose bg event (MAP_EVENT_BG) gives an item, coins, a
/// Psynergy Stone or a mimic and whose flag is not yet set. Each mark is an
/// object of sprite 22 running gFieldEffect_MarkerParticleScript. Called by
/// Reveal's effect (field_services/reveal.c).
void FieldEffect_SpawnNearbyMarkers(void) {
  u8 *markerData;
  struct Object *objectEvent;
  const struct MapEvent *entry;
  s32 leaderColumn;
  s32 leaderRow;
  s32 column;
  s32 row;
  s32 id;

  // The layout's treasure spots: (column, row, id) triples, ended by 0xff,
  // 0xff; the same list as MapGridMarkers's markers in markers.c.
  markerData = *(u8 **)(((u8 *)gMapGridWork[0]) + 16);
  objectEvent = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  leaderColumn = objectEvent->x >> 20;
  leaderRow = objectEvent->z >> 20;
  if (markerData != 0) {
    while (column = *markerData++, row = *markerData++,
           column != 0xff || row != 0xff) {
      id = *markerData++;
      if (TreasureSpot_GetObjectEvent(id) != NULL)
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
      entry = gOverlayArea.events();
      if (entry->kind == (u32)-1)
        continue;
      do {
        if (entry->trigger == id && (entry->kind & 15) == MAP_EVENT_BG) {
          switch (entry->value & 0xfff00000) {
          case 0:
          case FIND_ITEM(0):
          case FIND_COINS(0):
          case FIND_PSYNERGY_STONE:
          case FIND_MIMIC:
            if (entry->condition != -1 &&
                GameFlag_TestFar(entry->condition) == 0) {
              /* FAKEMATCH(temporary): the spawned marker reuses objectEvent, so
               * the one pseudo keeps ObjectEvent_GetByLocalId's result in r5;
               * as a variable of its own, 9 of 163 instructions differ in
               * tbs-en. */
              objectEvent = (struct Object *)Object_CreateFar(
                  22, (column << 20) + (8 << 16), 0, (row << 20) + (8 << 16));
              if (objectEvent != 0) {
                ObjectDispatch_InitializeFar(
                    (struct DispatchObject *)objectEvent,
                    gFieldEffect_MarkerParticleScript);
                ObjectDispatch_SetSingleChildFlagsFar(
                    (struct DispatchObject *)objectEvent, 0);
                ((struct EffectObject *)objectEvent)->updateHook =
                    Reveal_EmitSparkles;
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
