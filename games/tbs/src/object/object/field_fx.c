// Part of object.c, the field object, compiled only through it.
// Showing a found item, and Reveal's sparkles on hidden things.

#include "../object_internal.h"

STATIC_ASSERT(offsetof(struct ObjectRuntime, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct ObjectRuntime, flags) == 0x55);
STATIC_ASSERT(offsetof(struct ObjectRuntime, animation) == 0x50);

// An actor as these effects see it.
struct EffectObj {
  s32 scriptWord; // the encoded script pointer
  u8 pad0[0x24];
  s32 velocityY;
  u8 pad1[0x1c];
  s32 gravity;
  u8 pad2[4];
  struct AnimationObject *animation;
  u8 pad3[24];
  void (*updateHook)(union FieldObject *object); // frame hook at offset 0x6c
};

STATIC_ASSERT(sizeof(struct EffectObj) == 0x70);
STATIC_ASSERT(offsetof(struct EffectObj, updateHook) == 0x6c);

// ObjectSystem_Initialize allocates 64 ObjectRuntime records in heap slot 5.
#define EFFECT_OBJECT_COUNT 64
#ifdef UBFIX
#define EFFECT_OBJECT_POOL ((struct EffectObj *)gObjectSlots)
#else
// UB: gObjectSlots and gEventWork are separate globals. The named pool load
// changes register allocation and shrinks StartRandomParticleEmitter from
// 0x158 to 0x156 bytes (English, 2026-10-16); keep the native read here.
#define EFFECT_OBJECT_POOL (*(struct EffectObj **)((u8 *)&gEventWork - 88))
#endif
#define SPARKLE_OBJECT_ID 285

// Keeps the party leader off the markers: a leader within 16 pixels of a
// marker cell's centre is moved 20 pixels from that centre, in the direction
// it stood, and loses its move target.
void Marker_MoveLeaderAway(void) {
  struct ObjectRuntime *leader = ObjectTable_Get(gGameState.selectedActor);
  struct MarkerSlot *marker = ((struct FieldWork *)gEventWork)->markers;
  s32 markerIndex = 0;
  s32 x;
  s32 z;
  s32 cellX;
  s32 cellZ;
  s32 dx;
  s32 dz;

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

// Starts the particle script and its first child at priority 1, if the spawn
// succeeded. Allocation failure leaves the effect absent.
// Sharing the offset and spawn as well changes literal/argument load order in
// both emitters, without changing their sizes (English, 2026-10-16).
static inline void Object_InitializeSparkle(struct ObjectRuntime *obj) {
  if (obj != 0) {
    // Draw the sparkle at priority 1.
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_ParticleScript);
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
    ((struct AnimationObject *)obj->animation)->image.priority = 1;
  }
}

// Each frame of a found item's picture: one time in ten, a sparkle appears
// within 16 pixels of it.
void ItemDisplay_EmitSparkles(union FieldObject *fieldObject) {
  struct ObjectRuntime *emitter = (struct ObjectRuntime *)fieldObject;
  union PolarOffsetPosition position;
  struct ObjectRuntime *obj;
  u32 randomRadius;

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
  obj = Object_Spawn(SPARKLE_OBJECT_ID, position.vector.x, position.vector.y,
                     position.vector.z);
  Object_InitializeSparkle(obj);
}

// Shows the picture of item (0 for none) at the centre of the cell 16 pixels
// in front of character actor, 16 pixels up, with sparkles. While Reveal is
// active, its sparkles are cleared first. Returns the picture's actor, or 0.
struct EffectObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item) {
  struct EffectObj *picture;

  s32 work = (s32)gEventWork;
  u32 actorOffset = (actor * 4) + 0x14; // EventWork's actors[actor]

  s32 srcZ = (s32)*(struct FieldActor **)(work + actorOffset);
  union PolarOffsetPosition picturePosition;
  s32 pictureX;

  if (srcZ == 0)
    return 0;

  picturePosition.vector.x = ((struct FieldActor *)srcZ)->x.fixed;
  picturePosition.vector.y = ((struct FieldActor *)srcZ)->y.fixed;
  picturePosition.vector.z = ((struct FieldActor *)srcZ)->z.fixed;
  Vector_AddPolarOffset(16 << 16, ((struct FieldActor *)srcZ)->facing,
                        picturePosition.coordinates);
  pictureX = (picturePosition.vector.x & 0xfff00000) + (8 << 16);
  // srcZ now holds the cell's z.
  srcZ = (picturePosition.vector.z & 0xfff00000) + (8 << 16);

  if (((struct FieldWork *)work)->revealActive != 0) {
    // Stop both the marker emitters and the sparkle actors they already made.
    struct EffectObj *effect = EFFECT_OBJECT_POOL;

    // work now counts the actors left.
    work = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->scriptWord != 0) {
        if (effect->updateHook == RevealMarker_EmitSparkles) {
          ObjectDispatch_ReleaseFar(effect);
        }
        if (effect->scriptWord == (s32)gBattleFx_ParticleScript) {
          ObjectDispatch_ReleaseFar(effect);
        }
      }
      work--;
      effect++;
    } while (work >= 0);
  }

  WaitFrames(3);
  picture = (struct EffectObj *)Object_CreateFar(22, pictureX, 16 << 16, srcZ);
  if (picture == 0)
    return 0;

  ObjectDispatch_InitializeFar((struct DispatchObject *)picture,
                               gBattleFx_ParticleEmitterScript);
  {
    struct AnimationObject *sprite = picture->animation;
    void *iconBuffer;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    picture->velocityY = 0x20000;
    picture->gravity = 0x4000;

    // Draw the item's icon into heap block 17, then send it to the sprite's
    // tiles.
    iconBuffer = Runtime_AllocateHeapBlock(17, 0x608);
    Ui_PrepareTransferForItemFar(item);

    sprite->image.tile =
        VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 0x400);

    Runtime_ReleaseHeapBlock(17);
    picture->updateHook = ItemDisplay_EmitSparkles;
  }

  return picture;
}

// Destroys object if there is one.
void Object_DestroyIfPresent(void *object) {
  if (object != 0)
    ObjectDispatch_ReleaseFar(object);
}

// Raises a found item's picture to 36 pixels above the party leader, who
// holds it up (animation 28) facing the screen.
void ItemDisplay_RaiseAboveLeader(struct ObjectRuntime *object) {
  struct ObjectRuntime *leader;

  if (object == 0)
    return;

  leader = ObjectTable_Get(gGameState.selectedActor);
  object->acceleration = 1 << 16;
  object->speedLimit = 2 << 16;
  object->flags = 0;
  Object_SetMoveTargetFar(object, leader->x, leader->y + (36 << 16), leader->z);
  WaitFrames(3);
  ObjectDispatch_ApplyArgumentToChildrenFar(leader, 28);
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gBattleFx_RisingPulseScript);
  leader->angle = 0x4000;
}

// Shows a found item's picture: flags bit 0 adds sparkles and bit 1 raises it
// above the leader, with waits of 60 and 80 frames around that when both are
// set. The leader then returns to animation 1.
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags) {
  void *leader;

  if (object != NULL) {
    leader = ObjectTable_Get(gGameState.selectedActor);
    if (flags & 1) {
      ObjectDispatch_SetSingleChildFlagsFar(object, 0);
      ObjectDispatch_InitializeFar(object, gBattleFx_ParticleEmitterScript);
      ((struct EffectObj *)object)->velocityY = 0x20000;
      ((struct EffectObj *)object)->gravity = 0x4000;
      ((struct EffectObj *)object)->updateHook = ItemDisplay_EmitSparkles;
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
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
  }
}

// Shows the picture of item 36 pixels above the party leader for 80 frames;
// flags bit 0 adds sparkles and bit 1 raises it as a found item.
void BattleFx_StartEffectObject22(s32 item, s32 flags) {
  struct FieldActor *leader = ObjectTable_Get(gGameState.selectedActor);
  void *iconBuffer = Runtime_AllocateHeapBlock(17, 0x608);
  struct EffectObj *object = (struct EffectObj *)Object_CreateFar(
      22, leader->x.fixed, leader->y.fixed + (36 << 16), leader->z.fixed);

  if (object != 0) {
    struct AnimationObject *sprite = object->animation;

    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 1;

    Ui_PrepareTransferForItemFar(item);
    VramCache_Load(sprite->slot, 128, (u8 *)iconBuffer + 0x400);
    Runtime_ReleaseHeapBlock(17);

    if (flags & 1)
      object->updateHook = ItemDisplay_EmitSparkles;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader((struct ObjectRuntime *)object);

    WaitFrames(80);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
    ObjectDispatch_ReleaseFar(object);
  }
}

// The German, Spanish, French and Italian editions stop Reveal's sparkles
// while a menu is open.
#if TBS_PATCH_2

#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define MENU_OPEN_OFFSET 0xcca // in gEventWork: non-zero while a menu is open

// While Reveal is active, destroys all its sparkles.
void BattleFx_ClearRandomParticles(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
    struct EffectObj *effect = EFFECT_OBJECT_POOL;
    s32 remainingActors = EFFECT_OBJECT_COUNT - 1;

    do {
      if (effect->scriptWord != 0) {
        if (effect->scriptWord == (s32)gBattleFx_ParticleScript)
          ObjectDispatch_ReleaseFar(effect);
      }
      remainingActors--;
      effect++;
    } while (remainingActors >= 0);
  }
}

#endif

// Each frame of a Reveal marker: one time in ten, a sparkle appears within 16
// pixels of it.
void RevealMarker_EmitSparkles(union FieldObject *fieldObject) {
  const struct FieldActor *source = (const struct FieldActor *)fieldObject;
  union PolarOffsetPosition position;
  struct ObjectRuntime *obj;
  u32 randomRadius;

#if defined(PARTICLES_PAUSE_FOR_MENU)
  if (*(s8 *)((u8 *)gEventWork + MENU_OPEN_OFFSET) != 0)
    return;
#endif
  if ((100 * Random16() >> 16) > 9)
    return;

  position.vector.x = source->x.fixed;
  position.vector.y = source->y.fixed;
  position.vector.z = source->z.fixed;
  randomRadius = Random16();
  Vector_AddPolarOffset(randomRadius << 4, Random16(), position.coordinates);
  obj = Object_Spawn(SPARKLE_OBJECT_ID, position.vector.x, position.vector.y,
                     position.vector.z);
  Object_InitializeSparkle(obj);
}

// Returns 1 while Reveal is active, else 0.
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter) {
  s16 revealState;
  u32 isActive = 0;
  struct FieldWork *work = (struct FieldWork *)gEventWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}

// Reveal: marks with sparkles the hidden things within 8 cells across and 5
// down of the leader: map places 100 to 239 with no marker actor, whose search
// event (kind 3) gives an item, coins, a Psynergy stone or a mimic (types 0
// to 3 and 5) and has not been found yet.
void FieldEffect_SpawnNearbyMarkers(void) {
  u8 *markerData;
  struct ObjectRuntime *actor;
  const struct SceneEvent *entry;
  s32 leaderColumn;
  s32 leaderRow;
  s32 column;
  s32 row;
  s32 id;

  // The map's places: (x, z, id) triples, ended by x and z 0xff.
  markerData = *(u8 **)(((u8 *)gMapWork[0]) + 16);
  actor = ObjectTable_Get(gGameState.selectedActor);
  leaderColumn = actor->x >> 20;
  leaderRow = actor->z >> 20;
  if (markerData != 0) {
    while (column = *markerData++, row = *markerData++,
           column != 0xff || row != 0xff) {
      id = *markerData++;
      if (Marker_GetActor(id) != NULL)
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
      if (entry->control == (u32)-1)
        continue;
      do {
        if (entry->trigger == id && (entry->control & 15) == EVENT_SEARCH) {
          switch (entry->value & 0xfff00000) {
          case 0:
          case 0x100000:
          case 0x200000:
          case 0x300000:
          case 0x500000:
            if (entry->condition != -1 &&
                GameFlag_TestFar(entry->condition) == 0) {
              /* FAKEMATCH(temporary): the spawned marker reuses actor, so the
               * one pseudo keeps ObjectTable_Get's result in r5; as a variable
               * of its own, 9 of 163 instructions differ in tbs-en. */
              actor = (struct ObjectRuntime *)Object_CreateFar(
                  22, (column << 20) + (8 << 16), 0, (row << 20) + (8 << 16));
              if (actor != 0) {
                ObjectDispatch_InitializeFar((struct DispatchObject *)actor,
                                             gBattleFx_MarkerParticleScript);
                ObjectDispatch_SetSingleChildFlagsFar(actor, 0);
                ((struct EffectObj *)actor)->updateHook =
                    RevealMarker_EmitSparkles;
              }
            }
            break;
          }
        }
        entry++;
      } while (entry->control != (u32)-1);
    }
  }
}

#undef EFFECT_OBJECT_COUNT
#undef EFFECT_OBJECT_POOL
#undef SPARKLE_OBJECT_ID
#undef PARTICLES_PAUSE_FOR_MENU
#undef MENU_OPEN_OFFSET
