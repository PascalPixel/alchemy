/// @file
/// The object cameras, which place every object's sprite each frame.
/// Part of field.c, the field object, compiled only through it.

#include "../field_internal.h"

/// A map cell's word, as the camera reads it.
struct ObjectCameraTile {
  u32 block : 12;   ///< the block (metatile) number
  u32 layer : 2;    ///< if not 0, the layer of an object standing on it, + 1
  u32 priority : 2; ///< if not 0, the priority of an object standing on it
  u32 unk10 : 16;
};

/// A field object, as the camera reads it.
struct ObjectCameraObject {
  u32 active; ///< zero for a free slot
  u16 unk04;
  u16 angle; ///< the sprite's facing, before the camera's angle is added
  s32 x;
  s32 y; ///< up; the on-screen test uses z - y
  s32 z;
  s32 height; ///< the ground height under the object
  s32 scaleX; ///< Q16
  s32 scaleY; ///< Q16
  u8 unk20[2];
  u8 layer; ///< the map layer it stands on; the cell under it may change it
  u8 flags; ///< bits: 2 draw 320 pixels down | 1 up | 0 priority from cell
  u8 unk24[0x2c];
  void *sprite; ///< a sprite, or for kind 2 an array of four sprite pointers
  u8 kind;      ///< 1: one sprite, 2: up to four
  u8 unk55[7];
  u8 held; ///< nonzero keeps the sprite and its VRAM off screen or suspended
  u8 unk5d[0x13];
};

/// A point the fixed camera looks from or at.
struct ObjectCameraPoint {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};

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
  struct MapScrollWork *mapWork;
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

  // MapScrollWork owns all three layer cell pointers at their 0x30-byte stride.
  // UB: map and system work are separate globals, beyond gObjectSlots.
  // Reading their named slots adds eight bytes in the forced EN build.
#ifdef UBFIX
  mapWork = gMapWork[0];
#else
  mapWork = *(struct MapScrollWork **)(((u8 *)&gObjectSlots) + 12);
#endif
#ifdef UBFIX
  viewX = mapWork->viewX & 0xffff0000;
  viewZ = mapWork->viewY & 0xffff0000;
#else
  viewCoordinates = &mapWork->viewX;
  viewX = viewCoordinates[0] & 0xffff0000;
  viewZ = viewCoordinates[1] & 0xffff0000;
#endif
#ifdef UBFIX
  system = gMenuCtrlWork;
#else
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
#endif
  // Copy the frame decoder (Render_DecodeFrame) into its heap slot.
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
  system->used = 0;
  object = (struct ObjectCameraObject *)gObjectSlots;
  for (objectIndex = 0; objectIndex < FIELD_OBJECT_COUNT;
       objectIndex++, object++) {
    if (object->active == 0)
      continue;
    // An object at (x, z) = (0, 0) is not placed; only its sprite's VRAM is
    // given up.
    if (object->x != 0 || object->z != 0) {
      kind = object->kind & 15;
      if (kind == 0)
        continue;
      // This camera places single sprites only; groups (kind 2) are left.
      if (kind != 1)
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
        // The cell under the object.
        tile = &((struct ObjectCameraTile *)mapWork->layers[object->layer]
                     .cells)[(object->x >> 20) + ((object->z >> 20) << 7)];
        if (object->flags & 1) {
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
        // XXX: flag bits 1 and 2 draw the object 320 pixels up or down the
        // map from where it is; the scenes that set them would say why.
        if (object->flags & 2) {
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
      if (kind == 1) {
        sprite = object->sprite;
        if (object->held == 0 && !sprite->retainCacheEntry) {
          VramCache_ReleaseVram(sprite->slot);
          sprite->dirty = kind;
        }
      }
    }
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_SCRATCH);
}

UNUSED s32 ObjectCamera_ReturnTrue(void) { return 1; }

/// Each frame, with the fixed camera: places every object's sprite, from the
/// 64th object down, facing them by the angle from the eye to the target.
/// While battle commands are chosen (flag 363, set by
/// BattlePresentation_BuildActions) it uses the fixed view matrix and 1.0
/// scale. ObjectSystem_Initialize schedules it (order 0xc80) for modes 3 (the
/// world map) and 4 (battle); ObjectSystem_SuspendProjectionAndShowSprites
/// suspends it. Resets the system's used count.
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

  // UB: camera and system work are separate globals, beyond gObjectSlots.
  // Reading their named slots adds four bytes in the forced EN build.
#ifdef UBFIX
  camera = (struct ObjectCameraFixed *)gCameraWork;
  system = gMenuCtrlWork;
#else
  camera = *(struct ObjectCameraFixed **)(((u8 *)&gObjectSlots) + 0x1c);
  system = *(struct ObjectSystemWork **)(((u8 *)&gObjectSlots) + 4);
#endif
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodeFrame,
                   Render_DecodeFrameCodeSize);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
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
  object = (struct ObjectCameraObject *)gObjectSlots;
  object += FIELD_OBJECT_COUNT - 1;
  fixedScale = 0x10000;
  for (i = FIELD_OBJECT_COUNT - 1; i >= 0; i--, object--) {
    if (object->active == 0)
      continue;
    kind = object->kind & 15;
    switch (kind) {
    case 1:
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
    case 2:
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
    case 0:
      break;
    }
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_SCRATCH);
}
