/// @file
/// Field objects: the object pool, dissolving sprites, starting scripts.
// Part of field.c.

#include "../field_internal.h"

/// One step of dissolving a sprite pixel by pixel. In each 8x8 tile (256
/// colours) one pixel is cleared, in gSpriteDissolve_PixelOrder's order; each
/// tile runs one step ahead of the last, and only tiles whose step is 64-127
/// clear a pixel. obj is the sprite (struct AnimationObject), pos the step
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

  struct AnimationObject *sprite = (struct AnimationObject *)obj;

  // The sprite's tiles in OBJ VRAM.
  tile = (u16 *)(OBJ_VRAM0 + gVramBlockCache[sprite->slot].offset);
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
s32 MapGrid_GetScreenRelativePosition(struct ObjectRuntime *object,
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
/// registers the per-frame updates. MODE_BATTLE moves objects with
/// Object_UpdateAllMotion, which runs no scripts; MODE_WORLD_MAP and
/// MODE_BATTLE use the fixed camera. Both blocks are heap slots, so a later
/// call gets the same ones back. No code releases HEAP_SLOT_OBJECT_POOL or
/// HEAP_SLOT_MENU_CONTROL by name, but Field_RunMap takes HEAP_SLOT_EVENT
/// from EWRAM before them, so its release of that slot as it returns frees
/// their memory too; Runtime_InitializeHeap clears the slots at the next
/// scene change.
///
/// Called through ObjectSystem_InitializeFar by the field scene runner
/// (object/object/run_map.c) and the battle's encounter, and directly by
/// the sprite viewer, each with its enum ObjectSystemMode. Allocates
/// HEAP_SLOT_MENU_CONTROL for the ObjectSystemWork and HEAP_SLOT_OBJECT_POOL
/// for the objects, and sets the sprite outline colours.
void ObjectSystem_Initialize(s32 controlMode) {
  struct ObjectSystemWork *system;
  struct ObjectRuntime *objectPool;
  volatile u32 fill;

  system = Runtime_AllocateBlock(HEAP_SLOT_MENU_CONTROL, sizeof(*system));
  objectPool = Runtime_AllocateBlock(HEAP_SLOT_OBJECT_POOL,
                                     FIELD_OBJECT_COUNT * sizeof(*objectPool));
  ObjectSystem_Configure(controlMode);
  Dma_Fill32Cell(fill, 0, objectPool, FIELD_OBJECT_COUNT * sizeof(*objectPool));
  Dma_Fill32Cell(fill, 0, system, sizeof(*system));
  // The scheduler runs higher orders first: the objects move (0xc8a) before
  // the camera follows them (0xc80).
  if (controlMode == MODE_BATTLE)
    Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
  else
    Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
  if (controlMode >= MODE_WORLD_MAP && controlMode <= MODE_BATTLE) {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
  } else {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
    // XXX: nothing in the tree reads these two IWRAM words; code that reads
    // them by address would say what the scrolling camera resets here.
    gIwram_Data2 = 0;
    gIwram_Data1 = 0;
  }
  system->edge = 15;
  system->fill = 0;
}

/// Unused: an empty function nothing calls.
UNUSED void ResourceTable_ReservedNoOpC0C4(void) {}

/// Unused: an empty function nothing calls.
UNUSED void ResourceTable_ReservedNoOp2(void) {}

/// Returns the first of the 64 objects not in use (with no script), or 0.
/// Called by FieldObject_Create.
void *ObjectDispatch_FindFreeObject(void) {
  struct ObjectRuntime *object = gObjectSlots;
  void *freeObject = 0;
  s32 objectIndex = 0;

  while (objectIndex <= FIELD_OBJECT_COUNT - 1) {
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
/// Clearing the script lets ObjectDispatch_FindFreeObject hand the object out
/// again. Reached through SC_FREE (ActionScript_FreeObject) and, as
/// ObjectDispatch_ReleaseFar, from the battle, the Psynergy effects and the
/// field scene's objects.
void ObjectDispatch_Release(struct DispatchObject *object) {
  s32 spritesRemaining;
  void **sprites;

  if (object) {
    switch (object->kind & 15) {
    case OBJECT_SPRITE_ONE:
      ResourceObject_Release(object->target.child);
      break;
    case OBJECT_SPRITE_GROUP:
      sprites = object->target.children;
      spritesRemaining = 3;
      do {
        struct AnimationObject *sprite = *sprites++;
        if (sprite)
          ResourceObject_Release(sprite);
      } while (--spritesRemaining >= 0);
      break;
    }
    Dma_Fill32(0, object, sizeof(struct ObjectRuntime));
  }
}

/// Creates a field object at (x, y, z), in 16.16 pixels, or returns 0 when
/// none is free. id / 4096 is its enum ObjectCreateKind and the low 12 bits
/// the sprite's animation id.
///
/// Called as Object_CreateFar by the battle's placement, the Psynergy
/// effects, the field scene's actors and markers and the scene scripts, and
/// directly by the player control for footprints and ripples.
/// Gives the object gObjectDispatch_DefaultScript (a lone SC_END), its home
/// position and the default motion values; a group's sprite list is taken from
/// the ObjectSystemWork's group tables.
struct ObjectRuntime *FieldObject_Create(s32 encodedSprite, s32 x, s32 y,
                                         s32 z) {
  struct ObjectRuntime *object;
  struct ObjectEvent *objectEvent;
  struct ScriptMotionObject *motion;
  struct ScriptObjectRuntime *scripted;
  struct AnimationObject *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;

  // The ROM searches twice and drops the first result.
  ObjectDispatch_FindFreeObject();
  spriteStorageKind = encodedSprite / 4096;
  encodedSprite &= 0xfff;
  object = ObjectDispatch_FindFreeObject();
  if (object != NULL) {
    objectEvent = (struct ObjectEvent *)object;
    objectEvent->radius = 16;
    switch (spriteStorageKind) {
    case OBJECT_CREATE_ONE_SPRITE:
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        object->animationKind = OBJECT_SPRITE_ONE;
        object->animation = sprite;
        objectEvent->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
      } else {
        object->animationKind = OBJECT_SPRITE_NONE;
      }
      break;
    case OBJECT_CREATE_GROUP:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      // XXX: a list is four words but the next starts a word later, so a
      // second list overlaps the first and its zeroing clears count. No code
      // in the tree creates a group object to show the intended spacing.
      spriteSlots =
          (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
      object->animationKind = spriteStorageKind;
      object->animation = spriteSlots;
      Dma_Fill32(0, spriteSlots, 0x10);
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        objectEvent->radius =
            Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
        *spriteSlots++ = sprite;
      }
      sprite = ResourceObject_Create(encodedSprite + 1);
      if (sprite != NULL)
        *spriteSlots = sprite;
      break;
    }
  }
  if (object != NULL) {
    motion = (struct ScriptMotionObject *)object;
    scripted = (struct ScriptObjectRuntime *)object;
    Object_SetPositionAndResetMotion(object, x, y, z);
    // Up to 2 pixels a frame, gaining 1 a frame; unit scale; following the
    // terrain and falling; turning to face its motion; keeping a quarter of its
    // speed when it bounces; facing down the screen (0x4000, the heading
    // gWorldMap_KeyHeadings gives Down).
    object->script = (s32 *)gObjectDispatch_DefaultScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    objectEvent->scaleX = 0x10000;
    objectEvent->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->flags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    motion->gravity = 0x10000;
    motion->vertical.bounce = 0x4000;
    object->collisionFlags = 0;
    object->actionFlags = OBJ_EVENT_FACES_MOTION;
    objectEvent->riseBaseY = 0;
    object->angle = 0x4000;
    // Its home position, in pixels.
    scripted->homeX = x / 65536;
    scripted->homeZ = z / 65536;
  }
  return object;
}

/// Gives an object a script and starts it from the top.
/// Sets the script and clears the cursor, the pause, the loop count and the
/// last flag test. Called by the Start functions below and the player
/// control, and as ObjectDispatch_InitializeFar by the battle's placement,
/// the Psynergy effects and the field scene's events.
void ObjectDispatch_Initialize(struct DispatchObject *object,
                               const s32 *script) {
  if (object != 0) {
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = 0;
    object->value5d = 0;
    object->value57 = 0;
  }
}

/// Defines name(object, value), which calls apply(sprite, value) on an
/// object's one sprite, or on each sprite of its group.
#define OBJECT_DISPATCH_DEFINE_APPLY(name, object_type, apply)                 \
  void name(object_type rawObject, s32 value) {                                \
    struct DispatchObject *object;                                             \
    void **sprites;                                                            \
    s32 spritesRemaining;                                                      \
    void *sprite;                                                              \
                                                                               \
    object = rawObject;                                                        \
    if (object != 0) {                                                         \
      switch (object->kind & 0xf) {                                            \
      case OBJECT_SPRITE_ONE:                                                  \
        apply(object->target.child, value);                                    \
        break;                                                                 \
      case OBJECT_SPRITE_GROUP:                                                \
        sprites = object->target.children;                                     \
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
OBJECT_DISPATCH_DEFINE_APPLY(ObjectDispatch_SelectAnimation, void *,
                             AnimationObject_SelectAnimation)

/// Sets the animation speed (timer step) of all of an object's sprites.
OBJECT_DISPATCH_DEFINE_APPLY(ObjectDispatch_SetAnimationSpeed,
                             struct DispatchObject *,
                             AnimationObject_SetActiveTimerSteps)

/// Both of the above: switches all of an object's sprites to animation and
/// sets their timer step to step.
void ObjectDispatch_SetAnimationAndSpeed(struct DispatchObject *object,
                                         s32 animation, s32 step) {
  void **sprites;
  void *sprite;
  s32 spritesRemaining;

  if (object != 0) {
    switch (object->kind & 15) {
    case OBJECT_SPRITE_ONE:
      AnimationObject_SelectAnimation(object->target.child, animation);
      AnimationObject_SetActiveTimerSteps(object->target.child, step);
      break;
    case OBJECT_SPRITE_GROUP:
      sprites = object->target.children;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
        sprite = *sprites++;
        if (sprite != 0) {
          AnimationObject_SelectAnimation(sprite, animation);
          AnimationObject_SetActiveTimerSteps(sprite, step);
        }
      }
      break;
    }
  }
}

/// Sets the rotation of an object's sprite, when it has just one.
void ObjectDispatch_SetChildRotation(struct DispatchObject *object,
                                     u32 rotation) {
  if (object != 0 && (object->kind & 0xf) == OBJECT_SPRITE_ONE)
    object->target.child->rotation = rotation;
}

/// Gives a one-sprite object's first layer animation id animationId and
/// restarts its layers.
/// A negative animationId does nothing.
void Animation_SetIndexAndInitializeObjects(void *rawDispatchObject,
                                            s32 animationId) {
  struct DispatchObject *object = rawDispatchObject;
  struct AnimationObject *sprite;

  if (object != NULL && (object->kind & 0xf) == OBJECT_SPRITE_ONE) {
    sprite = object->target.child;
    if (animationId >= 0) {
      sprite->entries[0]->animationId = animationId;
      Animation_InitializeObjects(sprite);
    }
  }
}

/// Adds a layer of animation id animationId to a one-sprite object's sprite.
/// A negative animationId does nothing.
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object,
                                          s32 animationId) {
  if (object != 0 && (object->kind & 0xf) == OBJECT_SPRITE_ONE) {
    struct AnimationObject *sprite = object->target.child;
    if (animationId >= 0)
      ResourceMetadata_Register(sprite, animationId);
  }
}

/// Starts an object on the script that approaches its linked object
/// (SC_APPROACH_LINK), linked to linkedObjectHandle. The functions below start
/// the engine's other scripts (object_script.c).
void ObjectDispatch_StartApproachLink(struct DispatchObject *object,
                                      s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_ApproachLinkScript);
  object->argument = linkedObjectHandle;
}

/// Starts an object on the script that runs SC_KEY_COUNT every frame.
/// The field scene runner (object/object/run_map.c) gives it to the leader
/// when gGameState.movementMode is 2.
void ObjectDispatch_StartKeyCheck(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_KeyCheckScript);
}

/// Starts an object on the script that moves it by the keys (SC_KEYS).
/// The field scene runner gives it to the leader when gGameState.movementMode
/// is 1.
void ObjectDispatch_StartMoveByKeys(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_KeysScript);
}

/// Starts an object on the player control script (SC_PLAYER).
/// The field scene runner's usual choice for the leader on a layered map.
void ObjectDispatch_StartPlayerControl(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_PlayerScript);
}

/// Starts an object on the world map control script (SC_WORLD).
/// The field scene runner gives it to the leader on the world map.
void ObjectDispatch_StartWorldMapControl(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_WorldMapScript);
}

/// Starts an object on the idle script (a lone SC_END).
void ObjectDispatch_StartIdle(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_EndScript);
}

/// Starts an object on the camera script (SC_CAMERA). With a leader to
/// follow, it also gets a speed limit of 4 pixels a frame and an
/// acceleration of 0.5 (value30 and value34 are those fields). Called through
/// ObjectDispatch_StartCameraFar by the field scene's actors, the Psynergy
/// camera and the Colosso and Kolima Forest scenes.
void ObjectDispatch_StartCamera(struct DispatchObject *object,
                                s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_CameraScript);
  if (linkedObjectHandle != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = linkedObjectHandle;
    object->value64 = 0;
  }
}

/// Waits until the object's script reaches SC_END (0x10), at most 300 frames
/// (five seconds); it returns as soon as the script idles, and after the
/// limit whether or not it has.
/// Called through ObjectDispatch_WaitForValue16Far by the Psynergy actor
/// commands and by scenes that wait for a scripted move (Colosso, Venus
/// Lighthouse, Vale).
void ObjectDispatch_WaitForValue16(void *rawObject) {
  struct ScriptInterpreter *interpreter = rawObject;
  s32 waitedFrames;

  waitedFrames = 0;
  if (interpreter->script[interpreter->cursor] != SC_END) {
    do {
      WaitFrames(1);
      waitedFrames++;
      if (waitedFrames > 299)
        break;
    } while (interpreter->script[interpreter->cursor] != SC_END);
  }
}

/// Sets the flags of a one-sprite object's sprite (bit 0: draw its shadow).
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object,
                                        u32 flags) {
  if (object != NULL) {
    if ((object->kind & 0xf) == OBJECT_SPRITE_ONE)
      object->target.child->flags = flags;
  }
}

/// Sets the OAM blend mode of a one-sprite object's sprite.
/// This one and the next two test the whole kind byte, not its low four
/// bits, so an object with high kind bits set is left alone.
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *object, u32 mode) {
  if (object != 0 && object->kind == OBJECT_SPRITE_ONE) {
    struct AnimationObject *sprite = object->target.child;
    sprite->image.objectMode = mode;
  }
}

/// Makes the fixed camera draw a one-sprite object at its own scale instead of
/// scaling it by distance.
void ObjectDispatch_SetChildFixedScale(struct DispatchObject *object,
                                       u32 fixedScale) {
  if (object != 0 && object->kind == OBJECT_SPRITE_ONE) {
    struct AnimationObject *sprite = object->target.child;
    sprite->useOwnScale = fixedScale;
  }
}

/// Sets the colour mode (SpriteColorMode) of a one-sprite object's layers,
/// except those drawn hidden (see ObjectGroup_SetChildValueUnlessFifteen).
void Animation_ApplyChildValues(struct DispatchObject *object, u32 param) {
  if (object != NULL && object->kind == OBJECT_SPRITE_ONE)
    ObjectGroup_SetChildValueUnlessFifteen(object->target.child, param);
}

/// Suspends the object camera updates, starts the palette back to normal and,
/// a frame later, shows the sprites with BG1-3 hidden, leaving BG0 unchanged.
/// Called through
/// ObjectSystem_SuspendProjectionAndShowSpritesFar by scenes (Colosso, the
/// Tolbi ship, Vale, Venus Lighthouse). The scenes except Colosso pair it
/// with ObjectSystem_ResumeProjectionAndHideLayers.
void ObjectSystem_SuspendProjectionAndShowSprites(void) {
  Scheduler_SuspendTasks(ObjectSystem_UpdateCamera);
  Scheduler_SuspendTasks(ObjectSystem_UpdateCameraFixed);
  ColorBuffer_SetTargetFar(0x10000, 1);
  ColorBuffer_StartInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT =
      (REG_DISPCNT & ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      DISPCNT_OBJ_ON;
}

/// Resumes the object camera updates and hides BG1-3 and the sprites.
/// Called through ObjectSystem_ResumeProjectionAndHideLayersFar by the same
/// scenes except Colosso.
void ObjectSystem_ResumeProjectionAndHideLayers(void) {
  Scheduler_ResumeTasks(ObjectSystem_UpdateCamera);
  Scheduler_ResumeTasks(ObjectSystem_UpdateCameraFixed);
  REG_DISPCNT &=
      ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

/// Unused: returns 1; nothing calls it.
UNUSED s32 ObjectDispatch_ReturnTrue(void) { return 1; }

#undef OBJECT_DISPATCH_DEFINE_APPLY
