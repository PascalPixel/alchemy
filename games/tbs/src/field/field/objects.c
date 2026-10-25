/// @file
/// Objects: creating and freeing them, dissolving sprites, starting scripts.
// Part of field.c.

#include "../field_internal.h"

/// One step of dissolving a sprite pixel by pixel. In each 8x8 tile (256
/// colours) one pixel is cleared, in gSpriteDissolve_PixelOrder's order; each
/// tile runs one step ahead of the last, and only tiles whose step is 64-127
/// clear a pixel. obj is the sprite (struct Sprite), pos the step
/// (0-127). Writes the sprite's tiles in OBJ VRAM. Called by the two
/// functions below and, as render_animated_tile_frameFar, by the battle's
/// playback (battle/battle/playback.c).
void SpriteDissolve_Step(u8 *obj, u32 pos) {
  u16 *tile;
  s32 tileCount;
  u32 i;
  const u8 *order;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  struct Sprite *sprite = (struct Sprite *)obj;

  // The sprite's tiles in OBJ VRAM.
  tile = (u16 *)(OBJ_VRAM0 + gVramCacheEntries[sprite->slot].offset);
  tileCount = (sprite->width * sprite->height) / 64;
  i = 0;

  if (i < tileCount) {
    order = gSpriteDissolve_PixelOrder;
    highMask = 0xff00;
    indexMask = 0x3f;
    offsetMask = 0x3e;

    // A tile is 64 bytes (32 halfwords) at 8 bits a pixel. The unsigned
    // test passes only steps 64 to 127, so later tiles start clearing
    // first and the dissolve sweeps across the sprite: tile i clears at
    // steps 64 - i to 127 - i, and even the last of a 64x64 sprite's 64
    // tiles has all 64 of its steps by step 127.
    for (; i < tileCount; i++, tile += 32, pos++) {
      if ((pos - 64) <= 63) {
        u8 pixel;
        u16 *pixelPair;
        u32 isOdd;

        // Each tile reads the order 17 entries on from the tile before, so
        // neighbouring tiles lose different pixels at the same step. VRAM
        // takes no byte writes: rewrite the pixel pair, clearing one; the
        // odd pixel is the pair's high byte.
        pixel = order[(pos + i * 16) & indexMask];
        pixelPair = (u16 *)((u8 *)tile + (pixel & offsetMask));
        isOdd = 1;
        isOdd &= pixel;
        if (isOdd != 0)
          *pixelPair = *(u8 *)pixelPair;
        else
          *pixelPair &= highMask;
      }
    }
  }
}

/// Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames.
/// Called through SpriteDissolve_RunFar by the auto
/// battle (battle/battle/auto_battle.c).
void SpriteDissolve_Run(u8 *sprite) {
  u32 step;

  step = 0;
  // Finish all four dissolve steps before waiting for the next video frame.
  do {
    SpriteDissolve_Step(sprite, step);
    SpriteDissolve_Step(sprite, step + 1);
    SpriteDissolve_Step(sprite, step + 2);
    SpriteDissolve_Step(sprite, step + 3);
    step += 4;
    WaitFrames(1);
  } while (step <= 127);
}

/// Dissolves count sprites together, pixel by pixel, over 32 frames.
/// Called through SpriteDissolve_RunManyFar by the battle's staging
/// (battle/battle/staging.c), after the unit is taken off its lists.
void SpriteDissolve_RunMany(u8 **sprites, s32 count) {
  u8 **firstSprite;
  u8 **spriteCursor;
  s32 spritesRemaining;
  u32 step;
  u32 secondDissolveStep;
  u32 thirdDissolveStep;
  u32 fourthDissolveStep;

  firstSprite = sprites;
  step = 0;
  do {
    if (count > 0) {
      secondDissolveStep = step + 1;
      thirdDissolveStep = step + 2;
      spriteCursor = firstSprite;
      fourthDissolveStep = step + 3;
      spritesRemaining = count;
      do {
        SpriteDissolve_Step(*spriteCursor, step);
        SpriteDissolve_Step(*spriteCursor, secondDissolveStep);
        SpriteDissolve_Step(*spriteCursor, thirdDissolveStep);
        spritesRemaining -= 1;
        SpriteDissolve_Step(*spriteCursor++, fourthDissolveStep);
      } while (spritesRemaining != 0);
    }
    WaitFrames(1U);
    step += 4;
  } while (step <= 0x7fU);
}

/// Writes the object's screen position, in pixels, to screenPosition[0..1] and
/// returns 0 when it is on or near the screen; otherwise writes 0, 0 and
/// returns -1. The position is x and z less the view's whole pixels: where a
/// layered map draws a point at height 0. It ignores y and the ground height,
/// which the camera takes off z when it draws the sprite and its shadow. No
/// code names it: its veneer in the field far-call table
/// (system/far_call/FIELD.S) is unnamed.
s32 MapGrid_GetScreenRelativePosition(struct Object *object,
                                      s32 *screenPosition) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
#ifndef UBFIX
  s32 *viewCoordinates = &grid->viewX;
#endif
  s32 viewX;
  s32 viewZ;
  s32 x;
  s32 y;

  // UB: viewY is a separate member, not a second element of viewX.
  // Direct member loads change the EN save set and load order (96 bytes each).
#ifdef UBFIX
  viewX = grid->viewX & 0xffff0000;
  viewZ = grid->viewY & 0xffff0000;
#else
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
  x = object->x - viewX;
  y = object->z - viewZ;
  // Across, from 32 pixels left of the screen to 32 past its 240; down,
  // below the view's top and above 224.
  if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
    *screenPosition++ = x >> 16;
    *screenPosition = y >> 16;
    return 0;
  }
  *screenPosition++ = 0;
  *screenPosition = 0;
  return -1;
}

/// Sets up the object system: allocates and zeroes the 64 objects and
/// registers the per-frame updates. OBJECT_SYSTEM_BATTLE moves objects with
/// Object_UpdateAllMotion, which runs no scripts; OBJECT_SYSTEM_WORLD_MAP and
/// OBJECT_SYSTEM_BATTLE use the fixed camera. Both blocks are heap slots, so a
/// later call gets the same ones back. No code releases HEAP_SLOT_OBJECTS or
/// HEAP_SLOT_OBJECT_SYSTEM by name, but Map_Run takes HEAP_SLOT_FIELD
/// from EWRAM before them, so its release of that slot as it returns frees
/// their memory too; InitHeap clears the slots at the next
/// map change.
///
/// Called through ObjectSystem_InitializeFar by the field map runner
/// (overworld/overworld/run_map.c) and the battle's encounter, and directly by
/// the sprite viewer, each with its enum ObjectSystemMode. Allocates
/// HEAP_SLOT_OBJECT_SYSTEM for the ObjectSystemWork and HEAP_SLOT_OBJECTS
/// for the objects, and sets the sprite outline colours.
void ObjectSystem_Initialize(s32 controlMode) {
  struct ObjectSystemWork *system;
  struct Object *objectPool;
  volatile u32 fill;

  system = AllocHeapSlotEwram(HEAP_SLOT_OBJECT_SYSTEM, sizeof(*system));
  objectPool =
      AllocHeapSlotEwram(HEAP_SLOT_OBJECTS, OBJECT_COUNT * sizeof(*objectPool));
  ObjectSystem_Configure(controlMode);
  Dma_Fill32Cell(fill, 0, objectPool, OBJECT_COUNT * sizeof(*objectPool));
  Dma_Fill32Cell(fill, 0, system, sizeof(*system));
  // The task runner runs higher priorities first: the objects move before
  // the camera follows them.
  if (controlMode == OBJECT_SYSTEM_BATTLE)
    Task_Add(Object_UpdateAllMotion, TASK_PRIORITY_MAIN_LOOP + 10);
  else
    Task_Add(Object_UpdateAllThumb, TASK_PRIORITY_MAIN_LOOP + 10);
  if (controlMode >= OBJECT_SYSTEM_WORLD_MAP &&
      controlMode <= OBJECT_SYSTEM_BATTLE) {
    Task_Add(ObjectSystem_ProjectSprites, TASK_PRIORITY_MAIN_LOOP);
  } else {
    Task_Add(ObjectSystem_PlaceSprites, TASK_PRIORITY_MAIN_LOOP);
    // XXX (no evidence): what the scrolling camera resets here; nothing in
    // the game reads these two IWRAM words, by name or by address.
    gCamera_UnreadWord2 = 0;
    gCamera_UnreadWord1 = 0;
  }
  system->edge = 15;
  system->fill = 0;
}

/// Unused: an empty function nothing calls.
UNUSED void nullsub_0800C0C4(void) {}

/// Unused: an empty function nothing calls.
UNUSED void nullsub_0800C0C8(void) {}

/// Returns the first of the 64 objects not in use (with no script), or 0.
/// Called by Object_CreateField.
void *Object_FindFree(void) {
  struct Object *object = gObjects;
  void *freeObject = 0;
  s32 objectIndex = 0;

  while (objectIndex <= OBJECT_COUNT - 1) {
    if (object->script == 0) {
      freeObject = object;
      break;
    }
    objectIndex++;
    object++;
  }
  return freeObject;
}

/// Releases an object's sprites and zeroes it, freeing it.
/// Clearing the script lets Object_FindFree hand the object out
/// again. Reached through SC_FREE (ObjectScript_FreeObject) and, as
/// Object_ReleaseFar, from the battle, the Psynergy effects and the
/// field map's objects.
void Object_Release(struct Object *object) {
  s32 spritesRemaining;
  void **sprites;

  if (object) {
    switch (object->spriteStorageKind & 15) {
    case OBJECT_SPRITE_ONE:
      Sprite_Destroy(object->sprite);
      break;
    case OBJECT_SPRITE_GROUP:
      sprites = (void **)object->sprite;
      spritesRemaining = 3;
      do {
        struct Sprite *sprite = *sprites++;
        if (sprite)
          Sprite_Destroy(sprite);
      } while (--spritesRemaining >= 0);
      break;
    }
    Dma_Fill32(0, object, sizeof(struct Object));
  }
}

/// Creates an object at (x, y, z), in 16.16 pixels, or returns 0 when
/// none is free. id / 4096 is its enum ObjectCreateKind and the low 12 bits
/// the sprite's animation id.
///
/// Called as Object_CreateFar by the battle's placement, the Psynergy
/// effects, the field map's object events and treasure spots and the map
/// scripts, and directly by the player control for footprints and ripples.
/// Gives the object gObject_DefaultScript (a lone SC_END), its home
/// position and the default motion values; a group's sprite list is taken from
/// the ObjectSystemWork's group tables.
struct Object *Object_CreateField(s32 encodedSprite, s32 x, s32 y, s32 z) {
  struct Object *object;
  struct ObjectEvent *objectEvent;
  struct Object *motion;
  struct Object *scripted;
  struct Sprite *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;

  // The ROM searches twice and drops the first result.
  Object_FindFree();
  spriteStorageKind = encodedSprite / 4096;
  encodedSprite &= 0xfff;
  object = Object_FindFree();
  if (object != NULL) {
    objectEvent = (struct ObjectEvent *)object;
    objectEvent->radius = 16;
    switch (spriteStorageKind) {
    case OBJECT_CREATE_ONE_SPRITE:
      sprite = Sprite_Create(encodedSprite);
      if (sprite != NULL) {
        object->spriteStorageKind = OBJECT_SPRITE_ONE;
        object->sprite = sprite;
        objectEvent->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
      } else {
        object->spriteStorageKind = OBJECT_SPRITE_NONE;
      }
      break;
    case OBJECT_CREATE_GROUP:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      // A list is four words but the next starts a word later, so a second
      // list would overlap the first and its zeroing clear count. No caller
      // passes OBJECT_CREATE_GROUP: every id is a sprite below 0x1000, an
      // object event template's (0 to 504), or a kind with no sprite.
      // XXX (no evidence): whether the lists were meant four words apart;
      // nothing in the game creates a group to show it.
      spriteSlots =
          (void **)((u32 *)gObjectSystemWork + gObjectSystemWork->count++ + 2);
      object->spriteStorageKind = spriteStorageKind;
      object->sprite = (struct Sprite *)spriteSlots;
      Dma_Fill32(0, spriteSlots, 0x10);
      sprite = Sprite_Create(encodedSprite);
      if (sprite != NULL) {
        objectEvent->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
        *spriteSlots++ = sprite;
      }
      sprite = Sprite_Create(encodedSprite + 1);
      if (sprite != NULL)
        *spriteSlots = sprite;
      break;
    }
  }
  if (object != NULL) {
    motion = object;
    scripted = (struct Object *)object;
    Object_SetPositionAndResetMotion(object, x, y, z);
    // Up to 2 pixels a frame, gaining 1 a frame; unit scale; following the
    // ground and falling; turning to face its motion; keeping a quarter of its
    // speed when it bounces; facing down the screen (FACING_SOUTH, the
    // heading gWorldMap_KeyHeadings gives Down).
    object->script = (s32 *)gObject_DefaultScript;
    object->speedLimit = 0x20000;
    object->scriptCursor = 0;
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
    object->acceleration = Q16_ONE;
    object->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    motion->gravity = Q16_ONE;
    motion->bounce = 0x4000;
    object->collisionFlags = 0;
    object->steeringFlags = OBJ_EVENT_FACES_MOTION;
    objectEvent->riseBaseY = 0;
    object->facing = FACING_SOUTH;
    // Its home position, in pixels.
    scripted->homeX = x / 65536;
    scripted->homeZ = z / 65536;
  }
  return object;
}

/// Gives an object a script and starts it from the top.
/// Sets the script and clears the cursor, the pause, the loop count and the
/// last flag test. Called by the Start functions below and the player
/// control, and as Object_InitializeFar by the battle's placement,
/// the Psynergy effects and the field map's scripts.
void Object_Initialize(struct Object *object, const s32 *script) {
  if (object != 0) {
    object->scriptCursor = 0;
    object->script = script;
    object->paused = 0;
    object->repeatCount = 0;
    object->conditionResult = 0;
  }
}

/// Defines name(object, value), which calls apply(sprite, value) on an
/// object's one sprite, or on each sprite of its group.
#define OBJECT_DEFINE_APPLY(name, object_type, apply)                          \
  void name(object_type rawObject, s32 value) {                                \
    struct Object *object;                                                     \
    void **sprites;                                                            \
    s32 spritesRemaining;                                                      \
    void *sprite;                                                              \
                                                                               \
    object = rawObject;                                                        \
    if (object != 0) {                                                         \
      switch (object->spriteStorageKind & 0xf) {                               \
      case OBJECT_SPRITE_ONE:                                                  \
        apply(object->sprite, value);                                          \
        break;                                                                 \
      case OBJECT_SPRITE_GROUP:                                                \
        sprites = (void **)object->sprite;                                     \
        spritesRemaining = 3;                                                  \
        do {                                                                   \
          sprite = *sprites++;                                                 \
          if (sprite != 0)                                                     \
            apply(sprite, value);                                              \
          spritesRemaining--;                                                  \
        } while (spritesRemaining >= 0);                                       \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// Switches all of an object's sprites to animation value.
OBJECT_DEFINE_APPLY(Object_SelectAnimation, void *, Sprite_StartAnim)

/// Sets the animation speed (timer step) of all of an object's sprites.
OBJECT_DEFINE_APPLY(Object_SetAnimationSpeed, struct Object *,
                    Sprite_SetAnimSpeed)

/// Both of the above: switches all of an object's sprites to animation and
/// sets their timer step to step.
void Object_SetAnimationAndSpeed(struct Object *object, s32 animation,
                                 s32 step) {
  void **sprites;
  void *sprite;
  s32 spritesRemaining;

  if (object != 0) {
    switch (object->spriteStorageKind & 15) {
    case OBJECT_SPRITE_ONE:
      Sprite_StartAnim(object->sprite, animation);
      Sprite_SetAnimSpeed(object->sprite, step);
      break;
    case OBJECT_SPRITE_GROUP:
      sprites = (void **)object->sprite;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
        sprite = *sprites++;
        if (sprite != 0) {
          Sprite_StartAnim(sprite, animation);
          Sprite_SetAnimSpeed(sprite, step);
        }
      }
      break;
    }
  }
}

/// Sets the rotation of an object's sprite, when it has just one.
void Object_SetSpriteRotation(struct Object *object, u32 rotation) {
  if (object != 0 && (object->spriteStorageKind & 0xf) == OBJECT_SPRITE_ONE)
    object->sprite->rotation = rotation;
}

/// Gives a one-sprite object's first anim animation id animationId and
/// restarts its anims.
/// A negative animationId does nothing.
void Anim_SetIndexAndInitializeObjects(void *rawDispatchObject,
                                       s32 animationId) {
  struct Object *object = rawDispatchObject;
  struct Sprite *sprite;

  if (object != NULL &&
      (object->spriteStorageKind & 0xf) == OBJECT_SPRITE_ONE) {
    sprite = object->sprite;
    if (animationId >= 0) {
      sprite->entries[0]->animationId = animationId;
      Sprite_ResetAnims(sprite);
    }
  }
}

/// Adds an anim of animation id animationId to a one-sprite object's sprite.
/// A negative animationId does nothing.
void Object_RegisterSpriteMetadata(struct Object *object, s32 animationId) {
  if (object != 0 && (object->spriteStorageKind & 0xf) == OBJECT_SPRITE_ONE) {
    struct Sprite *sprite = object->sprite;
    if (animationId >= 0)
      Sprite_AddAnim(sprite, animationId);
  }
}

/// Starts an object on the script that approaches its linked object
/// (SC_APPROACH_LINK), linked to linkedObjectHandle. The functions below start
/// the engine's other scripts (object_script.c).
void Object_StartApproachLink(struct Object *object, s32 linkedObjectHandle) {
  Object_Initialize(object, gObject_ApproachLinkScript);
  object->linkedObject = (struct Object *)linkedObjectHandle;
}

/// Starts an object on the script that runs SC_KEY_COUNT every frame.
/// The field map runner (overworld/overworld/run_map.c) gives it to the leader
/// when gSaveBlock.movementMode is 2.
void Object_StartKeyCheck(struct Object *object) {
  Object_Initialize(object, gObject_KeyCheckScript);
}

/// Starts an object on the script that moves it by the keys (SC_KEYS).
/// The field map runner gives it to the leader when gSaveBlock.movementMode
/// is 1.
void Object_StartMoveByKeys(struct Object *object) {
  Object_Initialize(object, gObject_KeysScript);
}

/// Starts an object on the player control script (SC_PLAYER).
/// The field map runner's usual choice for the leader on a layered map.
void Object_StartPlayerControl(struct Object *object) {
  Object_Initialize(object, gObject_PlayerScript);
}

/// Starts an object on the world map control script (SC_WORLD).
/// The field map runner gives it to the leader on the world map.
void Object_StartWorldMapControl(struct Object *object) {
  Object_Initialize(object, gObject_WorldMapScript);
}

/// Starts an object on the idle script (a lone SC_END).
void Object_StartIdle(struct Object *object) {
  Object_Initialize(object, gObject_IdleScript);
}

/// Starts an object on the camera script (SC_CAMERA). With a leader to
/// follow, it also gets a speed limit of 4 pixels a frame and an
/// acceleration of 0.5. Called through
/// Object_StartCameraFar by the field map's object events, the Psynergy
/// camera and the Colosso and Kolima Forest maps.
void Object_StartCamera(struct Object *object, s32 linkedObjectHandle) {
  Object_Initialize(object, gObject_CameraScript);
  if (linkedObjectHandle != 0) {
    object->acceleration = 0x8000;
    object->speedLimit = 0x40000;
    object->linkedObject = (struct Object *)linkedObjectHandle;
    object->action = 0;
  }
}

/// Waits until the object's script reaches SC_END (0x10), at most 300 frames
/// (five seconds); it returns as soon as the script idles, and after the
/// limit whether or not it has.
/// Called through Object_WaitForValue16Far by the Psynergy object event
/// commands and by map scripts that wait for a scripted move (Colosso, Venus
/// Lighthouse, Vale).
void Object_WaitForValue16(void *rawObject) {
  struct Object *interpreter = rawObject;
  s32 waitedFrames;

  waitedFrames = 0;
  if (interpreter->script[interpreter->scriptCursor] != SC_END) {
    do {
      WaitFrames(1);
      waitedFrames++;
      if (waitedFrames > 299)
        break;
    } while (interpreter->script[interpreter->scriptCursor] != SC_END);
  }
}

/// Sets the flags of a one-sprite object's sprite (bit 0: draw its shadow).
void Object_SetSpriteFlags(struct Object *object, u32 flags) {
  if (object != NULL) {
    if ((object->spriteStorageKind & 0xf) == OBJECT_SPRITE_ONE)
      object->sprite->flags = flags;
  }
}

/// Sets the OAM blend mode of a one-sprite object's sprite.
/// This one and the next two test the whole kind byte, not its low four
/// bits, so an object with high kind bits set is left alone.
void Object_SetSpriteBlendMode(struct Object *object, u32 mode) {
  if (object != 0 && object->spriteStorageKind == OBJECT_SPRITE_ONE) {
    struct Sprite *sprite = object->sprite;
    sprite->image.objectMode = mode;
  }
}

/// Makes the fixed camera draw a one-sprite object at its own scale instead of
/// scaling it by distance.
void Object_SetSpriteScale(struct Object *object, u32 fixedScale) {
  if (object != 0 && object->spriteStorageKind == OBJECT_SPRITE_ONE) {
    struct Sprite *sprite = object->sprite;
    sprite->useOwnScale = fixedScale;
  }
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite object's anims,
/// except those drawn hidden (see Sprite_SetAnimColorModes).
void Anim_ApplyChildValues(struct Object *object, u32 param) {
  if (object != NULL && object->spriteStorageKind == OBJECT_SPRITE_ONE)
    Sprite_SetAnimColorModes(object->sprite, param);
}

/// Suspends the object camera updates, starts the palette back to normal and,
/// a frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged.
/// Called through
/// ObjectSystem_SuspendProjectionAndShowSpritesFar by map scripts (Colosso, the
/// Tolbi ship, Vale, Venus Lighthouse). The scripts except Colosso's pair it
/// with ObjectSystem_ResumeProjectionAndHideLayers.
void ObjectSystem_SuspendProjectionAndShowSprites(void) {
  Task_SuspendAll(ObjectSystem_PlaceSprites);
  Task_SuspendAll(ObjectSystem_ProjectSprites);
  PaletteFade_SetTargetFar(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT =
      (REG_DISPCNT & ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      DISPCNT_OBJ_ON;
}

/// Resumes the object camera updates and hides BG1-3 and the sprites.
/// Called through ObjectSystem_ResumeProjectionAndHideLayersFar by the same
/// scripts except Colosso's.
void ObjectSystem_ResumeProjectionAndHideLayers(void) {
  Task_ResumeAll(ObjectSystem_PlaceSprites);
  Task_ResumeAll(ObjectSystem_ProjectSprites);
  REG_DISPCNT &=
      ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/// Unused: returns 1; nothing calls it.
UNUSED s32 Object_ReturnTrue(void) { return 1; }

#undef OBJECT_DEFINE_APPLY
