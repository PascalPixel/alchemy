/// @file
/// The object cameras, which place every object's sprite each frame.
// Part of field.c.

#include "../field_internal.h"

/// A map cell's word, as the camera reads it.
struct ObjectCameraTile {
  u32 block : 12;   ///< the metatile number
  u32 layer : 2;    ///< if not 0, the layer of an object standing on it, + 1
  u32 priority : 2; ///< if not 0, the priority of an object standing on it
  /// MapGridCell's metatile behavior and collision attribute; not read here.
  u32 behaviorAndCollision : 16;
};

/// An object event (one of gObjects' 64, 0x70 bytes), as the cameras read
/// it.
struct ObjectCameraObject {
  u32 active;       ///< zero for a free slot
  u16 scriptCursor; ///< ObjectEvent's; not named here
  u16 angle;        ///< the sprite's facing, before the camera's angle is added
  s32 x;            ///< 16.16 fixed-point pixels, across the map
  s32 y;      ///< 16.16 fixed-point pixels, up; the on-screen test uses z - y
  s32 z;      ///< 16.16 fixed-point pixels, down the map
  s32 height; ///< the ground height under the object, 16.16 pixels
  s32 scaleX; ///< Q16
  s32 scaleY; ///< Q16
  u16 radius; ///< ObjectEvent's; not named here
  /// The map layer it stands on, which the cell under it may change; the
  /// fixed camera uses it as the draw slot, 0 to sort by distance.
  u8 layer;
  /// bits: 2 sorted as 320 pixels lower | 1 sorted as 320 pixels higher |
  /// 0 priority from cell
  u8 flags;
  u8 motionToRise[0x2c]; ///< ObjectEvent's velocityX to riseBaseY
  /// A sprite, or for OBJECT_SPRITE_GROUP an array of four sprite pointers.
  void *sprite;
  u8 kind;              ///< enum ObjectSpriteKind in its low four bits
  u8 motionToPaused[7]; ///< ObjectEvent's motionFlags to paused
  u8 held; ///< nonzero keeps the sprite and its VRAM off screen or suspended
  u8 repeatToUpdate[0x13]; ///< ObjectEvent's repeatCount to its update hook
};

/// A point the fixed camera looks from or at.
struct ObjectCameraPoint {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};

/// The start of the 3D camera's work (heap slot HEAP_SLOT_CAMERA, gCameraWork,
/// a struct BattleCamera), as the fixed camera reads it: the eye and target it
/// looks from and at, or the points that stand in for them. The battle, the
/// world map or Lucky Dice owns the block.
struct ObjectCameraFixed {
  struct ObjectCameraPoint eye;
  struct ObjectCameraPoint target;
  struct ObjectCameraPoint *eyeOverride;    ///< when set, used as the eye
  struct ObjectCameraPoint *targetOverride; ///< when set, used as the target
};

/// Each frame, with the scrolling camera: places every object's sprite on the
/// screen. An object on or near the screen takes its layer and priority from
/// the cell under it; one off screen gives up its VRAM cache entry.
/// ObjectSystem_Initialize schedules it (order 0xc80, after the objects move)
/// for every mode but 3 and 4; ObjectSystem_SuspendProjectionAndShowSprites
/// suspends it. Resets the system's used count, may change an object's layer
/// and its sprite's priorities, and holds the frame decoder in heap slot
/// HEAP_SLOT_FRAME_SCRATCH only while it runs.
void ObjectSystem_UpdateCamera(void) {
  /* FAKEMATCH(dead): an unused local for the room the native frame keeps for
   * three points above the position; without it, 2 of 267 instructions differ
   * in tbs-en. */
  UNUSED struct ObjectCameraPoint unusedPoints[3];
  s32 objectIndex;
  struct MapGridScrollWork *grid;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct ObjectCameraObject *object;
  struct AnimationObject *sprite;
#ifndef UBFIX
  s32 *viewCoordinates;
#endif
  u32 kind;
  s32 dx;
  s32 dz;
  s32 screenTop;
  struct ObjectCameraTile *tile;
  u32 priority;
  struct SpriteScale scale;
  struct SpritePlacement drawPosition;
  u32 layerPlusOne;

  // MapGridScrollWork owns all three layer cell pointers at their 0x30-byte
  // stride. UB: map and system work are separate globals, beyond gObjects.
  // Reading their named slots adds eight bytes in the forced EN build.
#ifdef UBFIX
  grid = gMapGridWork[0];
#else
  grid = *(struct MapGridScrollWork **)(((u8 *)&gObjects) + 12);
#endif
#ifdef UBFIX
  viewX = grid->viewX & 0xffff0000;
  viewZ = grid->viewY & 0xffff0000;
#else
  viewCoordinates = &grid->viewX;
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
#ifdef UBFIX
  system = gObjectSystemWork;
#else
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjects) + 4);
#endif
  // Copy the frame decoder (Render_DecodeFrame) into its heap slot.
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
  system->used = 0;
  object = (struct ObjectCameraObject *)gObjects;
  for (objectIndex = 0; objectIndex < OBJECT_COUNT; objectIndex++, object++) {
    if (object->active == 0)
      continue;
    // An object at (x, z) = (0, 0) is not placed; only its sprite's VRAM is
    // given up.
    if (object->x != 0 || object->z != 0) {
      kind = object->kind & 15;
      if (kind == OBJECT_SPRITE_NONE)
        continue;
      // This camera places single sprites only; groups are left.
      if (kind != OBJECT_SPRITE_ONE)
        continue;
      // While a menu has sprite projection suspended, only held objects are
      // drawn.
      if (system->suspended != 0 && object->held == 0) {
        struct AnimationObject *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      sprite = object->sprite;
      // From 32 pixels left of the screen to 32 past its 240, and from 32
      // pixels above it to 224.
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000) {
        // The cell under the object: x and z >> 20 are 16-pixel cells, and a
        // row holds 128 of them.
        tile = &((struct ObjectCameraTile *)grid->layers[object->layer]
                     .cells)[(object->x >> 20) + ((object->z >> 20) << 7)];
        if (object->flags & OBJ_EVENT_PRIORITY_AUTOMATIC) {
          priority = tile->priority;
          if (priority != 0) {
            sprite->image.priority = priority;
            sprite->shadow.priority = priority;
          }
        }
        layerPlusOne = tile->layer;
        if (layerPlusOne != 0)
          object->layer = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        drawPosition.x = dx;
        drawPosition.height = object->y;
        drawPosition.y = dz;
        drawPosition.groundHeight = object->height;
        // OBJ_EVENT_PRIORITY_UNDERFOOT and bit 2 move the point 320 pixels up
        // or down the map with its height and ground height, so the sprite and
        // shadow stay where they are on screen; only the draw slot changes,
        // which Render_ApplyProjectedPlacement takes from the point's y
        // before its height is taken off. OBJ_EVENT_PRIORITY_UNDERFOOT, which
        // footprints take, moves the slot 160 back, and an object at most 220
        // pixels up crosses that function's 100-pixel line into its lowest
        // slot; bit 2 moves it 160 on.
        if (object->flags & OBJ_EVENT_PRIORITY_UNDERFOOT) {
          drawPosition.height += 0xfec00000; // -320 pixels
          drawPosition.y += 0xfec00000;
          drawPosition.groundHeight += 0xfec00000;
        }
        if (object->flags & 4) {
          drawPosition.height += 0x1400000; // +320 pixels
          drawPosition.y += 0x1400000;
          drawPosition.groundHeight += 0x1400000;
        }
        Render_ApplyProjectedPlacement(sprite, &drawPosition, &scale,
                                       object->angle);
        continue;
      }
      // Off screen: the sprite's VRAM goes back to the cache, and dirty makes
      // it redraw when it comes back.
      if (object->held == 0) {
        if (!sprite->retainCacheEntry) {
          VramCache_ReleaseVram(sprite->slot);
          sprite->dirty = 1;
        }
      }
    } else {
      kind = object->kind & 15;
      if (kind == OBJECT_SPRITE_ONE) {
        sprite = object->sprite;
        if (object->held == 0 && !sprite->retainCacheEntry) {
          VramCache_ReleaseVram(sprite->slot);
          sprite->dirty = kind;
        }
      }
    }
  }
  FreeHeapSlot(HEAP_SLOT_FRAME_SCRATCH);
}

/// Unused: returns 1; nothing calls it.
UNUSED s32 ObjectCamera_ReturnTrue(void) { return 1; }

/// Each frame, with the fixed camera: places every object's sprite, from the
/// 64th object down, facing them by the angle from the eye to the target.
/// While battle commands are chosen (FLAG_CHOOSING_COMMANDS, set by
/// BattlePresentation_BuildActions) it uses the fixed view matrix and 1.0
/// scale. ObjectSystem_Initialize schedules it (order 0xc80) for
/// OBJECT_SYSTEM_WORLD_MAP and OBJECT_SYSTEM_BATTLE;
/// ObjectSystem_SuspendProjectionAndShowSprites suspends it. Resets the
/// system's used count.
void ObjectSystem_UpdateCameraFixed(void) {
  s32 i;
  struct ObjectCameraFixed *camera;
  struct ObjectSystemWork *system;
  struct ObjectCameraPoint *eye;
  struct ObjectCameraPoint *target;
  struct ObjectCameraObject *object;
  s32 angle;
  s32 spritesRemaining;
  s32 kind;
  s32 *objectPosition;
  s32 fixedScale;
  void **sprites;
  void *sprite;
  s32 groupScale[2];
  s32 scale[2];
  /* FAKEMATCH(dead): an unused local for the room the native frame keeps for
   * two points above the scales; without it, 2 of 181 instructions differ in
   * tbs-en. */
  UNUSED struct ObjectCameraPoint unusedPoints[2];

  // UB: camera and system work are separate globals, beyond gObjects.
  // Reading their named slots adds four bytes in the forced EN build.
#ifdef UBFIX
  camera = (struct ObjectCameraFixed *)gCameraWork;
  system = gObjectSystemWork;
#else
  camera = *(struct ObjectCameraFixed **)(((u8 *)&gObjects) + 0x1c);
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjects) + 4);
#endif
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  // The camera's heading over the ground, from the eye's offset from the
  // target in whole pixels; each sprite's facing is drawn relative to it, so
  // a sprite keeps its world heading as the camera turns.
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  Render_ResetTransformState();
  // Under the fixed view matrix every sprite also faces an eighth of a turn
  // (0x2000) further back.
  if (GameFlag_TestFar(FLAG_CHOOSING_COMMANDS)) {
    angle += 0xffffe000;
    Iwram_TransformMatrix(gCamera_FixedViewMatrix);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  // The objects are placed from the last down. A sprite's layer byte is its
  // draw slot here: 0 sorts it by distance (Render_PlaceProjectedSprite).
  object = (struct ObjectCameraObject *)gObjects;
  object += OBJECT_COUNT - 1;
  fixedScale = 0x10000;
  for (i = OBJECT_COUNT - 1; i >= 0; i--, object--) {
    if (object->active == 0)
      continue;
    kind = object->kind & 15;
    switch (kind) {
    case OBJECT_SPRITE_ONE:
      objectPosition = &object->x;
      scale[0] = object->scaleX;
      scale[1] = object->scaleY;
      sprite = object->sprite;
      if (GameFlag_TestFar(FLAG_CHOOSING_COMMANDS)) {
        scale[0] = fixedScale;
        scale[1] = fixedScale;
      }
      Render_PlaceProjectedSprite(sprite, objectPosition, scale,
                                  object->angle + angle, object->layer);
      break;
    case OBJECT_SPRITE_GROUP:
      groupScale[0] = object->scaleX;
      groupScale[1] = object->scaleY;
      if (GameFlag_TestFar(FLAG_CHOOSING_COMMANDS)) {
        groupScale[0] = fixedScale;
        groupScale[1] = fixedScale;
      }
      // A group's sprite field points to its four sprite pointers; empty
      // ones are null.
      sprites = object->sprite;
      for (spritesRemaining = 3; spritesRemaining >= 0; spritesRemaining--) {
        sprite = *sprites++;
        if (sprite != NULL)
          Render_PlaceProjectedSprite(sprite, &object->x, groupScale,
                                      object->angle + angle, object->layer);
      }
      break;
    case OBJECT_SPRITE_NONE:
      break;
    }
  }
  FreeHeapSlot(HEAP_SLOT_FRAME_SCRATCH);
}
