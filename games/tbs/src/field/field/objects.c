// Part of field.c, the field object, compiled only through it.
// Field objects: the object pool, dissolving sprites, starting scripts.

#include "../field_internal.h"

// One step of dissolving a sprite pixel by pixel. In each 8x8 tile (256
// colours) one pixel is cleared, in gMap_TileDissolveOrder's order; each
// tile runs one step ahead of the last, and only tiles whose step is 64-127
// clear a pixel. obj is the sprite (struct AnimationObject), pos the step
// (0-127).
void Map_RenderAnimatedTileFrame(u8 *obj, u32 pos) {
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
    order = gMap_TileDissolveOrder;
    highMask = 0xff00;
    indexMask = 0x3f;
    offsetMask = 0x3e;

    for (; i < tileCount; i++, tile += 32, pos++) {
      if ((pos - 64) <= 63) {
        u8 pixel;
        u16 *pixelPair;
        u32 isOdd;

        // VRAM takes no byte writes: rewrite the pixel pair, clearing one.
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

// Dissolves a sprite pixel by pixel, four steps a frame, over 32 frames.
void Map_RenderAnimatedTileFramesForObject(u8 *sprite) {
  u32 step;

  step = 0;
  // Finish all four dissolve steps before waiting for the next video frame.
  do {
    Map_RenderAnimatedTileFrame(sprite, step);
    Map_RenderAnimatedTileFrame(sprite, step + 1);
    Map_RenderAnimatedTileFrame(sprite, step + 2);
    Map_RenderAnimatedTileFrame(sprite, step + 3);
    step += 4;
    WaitFrames(1);
  } while (step <= 127);
}

// Dissolves count sprites together, pixel by pixel, over 32 frames.
void Map_RenderAllAnimatedTileFrames(u8 **sprites, s32 count) {
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
        Map_RenderAnimatedTileFrame(*spriteCursor, step);
        Map_RenderAnimatedTileFrame(*spriteCursor, secondDissolveStep);
        Map_RenderAnimatedTileFrame(*spriteCursor, thirdDissolveStep);
        spritesRemaining -= 1;
        Map_RenderAnimatedTileFrame(*spriteCursor++, fourthDissolveStep);
      } while (spritesRemaining != 0);
    }
    WaitFrames(1U);
    step += 4;
  } while (step <= 0x7fU);
}

// Writes the object's screen position, in pixels, to screenPosition[0..1] and
// returns 0 when it is on or near the screen; otherwise writes 0, 0 and
// returns -1.
s32 Map_GetScreenRelativePosition(struct ObjectRuntime *object,
                                  s32 *screenPosition) {
  struct MapScrollWork *mapWork = gMapWork[0];
#ifndef UBFIX
  s32 *viewCoordinates = &mapWork->viewX;
#endif
  s32 viewX;
  s32 viewZ;
  s32 x;
  s32 y;

  // UB: viewY is a separate member, not a second element of viewX.
  // Direct member loads change the EN save set and load order (96 bytes each).
#ifdef UBFIX
  viewX = mapWork->viewX & 0xffff0000;
  viewZ = mapWork->viewY & 0xffff0000;
#else
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
  x = object->x - viewX;
  y = object->z - viewZ;
  if ((x >= -0x1fffff && x <= 0x10fffff) && y > 0 && y < 0xe00000) {
    *screenPosition++ = x >> 16;
    *screenPosition = y >> 16;
    return 0;
  }
  *screenPosition++ = 0;
  *screenPosition = 0;
  return -1;
}

// Sets up the object system: allocates and zeroes the 64 objects and
// registers the per-frame updates. Mode 4 moves objects with
// Object_UpdateAllMotion; modes 3 and 4 use the fixed camera.
void ObjectSystem_Initialize(s32 controlMode) {
  struct ObjectSystemWork *objectSystem;
  struct ObjectRuntime *objectPool;
  volatile u32 fill;

  objectSystem = Runtime_AllocateBlock(6, sizeof(*objectSystem));
  objectPool = Runtime_AllocateBlock(5, 64 * sizeof(*objectPool));
  ObjectSystem_Configure(controlMode);
  Dma_Fill32Cell(fill, 0, objectPool, 64 * sizeof(*objectPool));
  Dma_Fill32Cell(fill, 0, objectSystem, sizeof(*objectSystem));
  if (controlMode == 4)
    Scheduler_AddOrUpdateCallback(Object_UpdateAllMotion, 0xc8a);
  else
    Scheduler_AddOrUpdateCallback(Object_UpdateAllThumb, 0xc8a);
  if (controlMode >= 3 && controlMode <= 4) {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCameraFixed, 0xc80);
  } else {
    Scheduler_AddOrUpdateCallback(ObjectSystem_UpdateCamera, 0xc80);
    gIwram_Data2 = 0;
    gIwram_Data1 = 0;
  }
  objectSystem->edge = 15;
  objectSystem->fill = 0;
}

// Does nothing.
UNUSED void ResourceTable_ReservedNoOpC0C4(void) {}

// Does nothing.
UNUSED void ResourceTable_ReservedNoOp2(void) {}

// Returns the first of the 64 objects not in use (with no script), or 0.
void *ObjectDispatch_FindFreeObject(void) {
  struct ObjectRuntime *object = gObjectSlots;
  void *freeObject = 0;
  s32 objectIndex = 0;

  while (objectIndex <= 63) {
    if (object->script == 0) {
      freeObject = object;
      break;
    }
    objectIndex++;
    object++;
  }
  return freeObject;
}

// Releases an object's sprites and zeroes it, freeing it.
void ObjectDispatch_Release(struct DispatchObject *object) {
  s32 spritesRemaining;
  void **sprites;

  if (object) {
    switch (object->kind & 15) {
    case 1: // one sprite
      ResourceObject_Release(object->target.child);
      break;
    case 2: // up to four sprites
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

// Creates a field object at (x, y, z), in 16.16 pixels, or returns 0 when
// none is free. id / 4096 is the kind (0: one sprite, 2: two sprites, id
// and id + 1) and the low 12 bits the sprite's animation id.
struct ObjectRuntime *FieldObject_Create(s32 encodedSprite, s32 x, s32 y,
                                         s32 z) {
  struct ObjectRuntime *object;
  struct FieldActor *actor;
  struct ScriptMotionObject *motion;
  struct ScriptObjectRuntime *scripted;
  struct AnimationObject *sprite;
  s32 spriteStorageKind;
  void **spriteSlots;

  ObjectDispatch_FindFreeObject();
  spriteStorageKind = encodedSprite / 4096;
  encodedSprite &= 0xfff;
  object = ObjectDispatch_FindFreeObject();
  if (object != NULL) {
    actor = (struct FieldActor *)object;
    actor->radius = 16;
    switch (spriteStorageKind) {
    case 0:
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->animation = sprite;
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      // Take a zeroed list of four sprites, and fill it with id and id + 1.
      spriteSlots =
          (void **)((u32 *)gMenuCtrlWork + gMenuCtrlWork->count++ + 2);
      object->animationKind = spriteStorageKind;
      object->animation = spriteSlots;
      Dma_Fill32(0, spriteSlots, 0x10);
      sprite = ResourceObject_Create(encodedSprite);
      if (sprite != NULL) {
        actor->radius = Resource_GetMetadataRecordFar(encodedSprite)->boxY >> 1;
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
    object->script = (s32 *)gObjectDispatch_DefaultScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    actor->scaleX = 0x10000;
    actor->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->flags = 3;
    motion->gravity = 0x10000;
    motion->vertical.bounce = 0x4000;
    object->unk59 = 0;
    object->actionFlags = 1;
    actor->riseBaseY = 0;
    object->angle = 0x4000;
    // Its home position, in pixels.
    scripted->homeX = x / 65536;
    scripted->homeZ = z / 65536;
  }
  return object;
}

// Gives an object a script and starts it from the top.
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

// Defines name(object, value), which calls apply(sprite, value) on an
// object's one sprite, or on each sprite of its group.
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
      case 1:                                                                  \
        apply(object->target.child, value);                                    \
        break;                                                                 \
      case 2:                                                                  \
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

// Switches all of an object's sprites to animation value.
OBJECT_DISPATCH_DEFINE_APPLY(ObjectDispatch_ApplyArgumentToChildren, void *,
                             AnimationObjects_SelectAnimation)

// Sets the animation speed (timer step) of all of an object's sprites.
OBJECT_DISPATCH_DEFINE_APPLY(ObjectDispatch_ApplyValueToChildren,
                             struct DispatchObject *,
                             AnimationObjects_SetActiveTimerSteps)

// Both of the above: switches all of an object's sprites to animation and
// sets their timer step to step.
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *object,
                                        s32 animation, s32 step) {
  void **sprites;
  void *sprite;
  s32 spritesRemaining;

  if (object != 0) {
    switch (object->kind & 15) {
    case 1:
      AnimationObjects_SelectAnimation(object->target.child, animation);
      AnimationObjects_SetActiveTimerSteps(object->target.child, step);
      break;
    case 2:
      sprites = object->target.children;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
        sprite = *sprites++;
        if (sprite != 0) {
          AnimationObjects_SelectAnimation(sprite, animation);
          AnimationObjects_SetActiveTimerSteps(sprite, step);
        }
      }
      break;
    }
  }
}

// Sets the rotation of an object's sprite, when it has just one.
void ObjectDispatch_SetChildRotation(struct DispatchObject *object,
                                     u32 rotation) {
  if (object != 0 && (object->kind & 0xf) == 1)
    object->target.child->rotation = rotation;
}

// Gives a one-sprite object's first layer animation id animId and restarts
// its layers.
// A negative animationId does nothing.
void Animation_SetIndexAndInitObjects(void *rawDispatchObject,
                                      s32 animationId) {
  struct DispatchObject *object = rawDispatchObject;
  struct AnimationObject *sprite;

  if (object != NULL && (object->kind & 0xf) == 1) {
    sprite = object->target.child;
    if (animationId >= 0) {
      sprite->entries[0]->animId = animationId;
      Animation_InitializeObjects(sprite);
    }
  }
}

// Adds a layer of animation id animId to a one-sprite object's sprite.
// A negative animId does nothing.
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object,
                                          s32 animationId) {
  if (object != 0 && (object->kind & 0xf) == 1) {
    struct AnimationObject *sprite = object->target.child;
    if (animationId >= 0)
      ResourceMetadata_Register(sprite, animationId);
  }
}

// Starts an object on the script that approaches its linked object
// (SC_APPROACH_LINK), linked to linkedObjectHandle. The functions below start
// the engine's other scripts (object_script.c).
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *object,
                                               s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table5Script);
  object->argument = linkedObjectHandle;
}

// Starts an object on the script that runs SC_KEY_COUNT every frame.
void ObjectDispatch_InitFromTable0(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table0Script);
}

// Starts an object on the script that moves it by the keys (SC_KEYS).
void ObjectDispatch_InitFromTable1(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table1Script);
}

// Starts an object on the player control script (SC_PLAYER).
void ObjectDispatch_InitFromTable2(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table2Script);
}

// Starts an object on the world map control script (SC_WORLD).
void ObjectDispatch_InitFromTable3(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table3Script);
}

// Starts an object on the idle script (a lone SC_END).
void ObjectDispatch_InitFromTable6(struct DispatchObject *object) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table6Script);
}

// Starts an object on the camera script (SC_CAMERA). With a leader to
// follow, it also gets a speed limit of 4 pixels a frame and an
// acceleration of 0.5 (value30 and value34 are those fields).
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object,
                                               s32 linkedObjectHandle) {
  ObjectDispatch_Initialize(object, gObjectDispatch_Table4Script);
  if (linkedObjectHandle != 0) {
    object->value34 = 0x8000;
    object->value30 = 0x40000;
    object->argument = linkedObjectHandle;
    object->value64 = 0;
  }
}

// Waits until the object's script reaches SC_END (0x10), at most 300 frames.
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

// Sets the flags of a one-sprite object's sprite (bit 0: draw its shadow).
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object,
                                        u32 flags) {
  if (object != NULL) {
    if ((object->kind & 0xf) == 1)
      object->target.child->flags = flags;
  }
}

// Sets the OAM blend mode of a one-sprite object's sprite.
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *object, u32 mode) {
  if (object != 0 && object->kind == 1) {
    struct AnimationObject *sprite = object->target.child;
    sprite->image.objectMode = mode;
  }
}

// Makes the fixed camera draw a one-sprite object at its own scale instead of
// scaling it by distance.
void ObjectDispatch_SetChildFixedScale(struct DispatchObject *object,
                                       u32 fixedScale) {
  if (object != 0 && object->kind == 1) {
    struct AnimationObject *sprite = object->target.child;
    sprite->useOwnScale = fixedScale;
  }
}

// Sets the draw parameter of a one-sprite object's layers (see
// ObjectGroup_SetChildValueUnlessFifteen).
void Animation_ApplyChildValues(struct DispatchObject *object, u32 param) {
  if (object != NULL && object->kind == 1)
    ObjectGroup_SetChildValueUnlessFifteen(object->target.child, param);
}

// Suspends the object camera updates, starts the palette back to normal and,
// a frame later, shows BG0 and the sprites with BG1-3 hidden.
void Graphics_EnableObjLayerAndCallbacks(void) {
  Scheduler_SuspendTasks(ObjectSystem_UpdateCamera);
  Scheduler_SuspendTasks(ObjectSystem_UpdateCameraFixed);
  BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleFx_StartBufferInterpolationFar(1);
  WaitFrames(1);
  REG_DISPCNT =
      (REG_DISPCNT & ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)) |
      DISPCNT_OBJ_ON;
}

// Resumes the object camera updates and hides BG1-3 and the sprites.
void ObjectDispatch_StopCallbacksAndHideLayers(void) {
  Scheduler_ResumeTasks(ObjectSystem_UpdateCamera);
  Scheduler_ResumeTasks(ObjectSystem_UpdateCameraFixed);
  REG_DISPCNT &=
      ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON);
}

// Returns 1.
UNUSED s32 ObjectDispatch_ReturnTrue(void) { return 1; }

#undef OBJECT_DISPATCH_DEFINE_APPLY
