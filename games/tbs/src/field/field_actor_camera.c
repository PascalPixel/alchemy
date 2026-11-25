/// @file
/// The actor cameras, which place every actor's sprite each frame.
// Part of field.c.

#include "field_internal.h"

/// A map cell's word, as the camera reads it.
struct ActorCameraTile {
  u32 block : 12;   ///< the number of the cell's tiles
  u32 layer : 2;    ///< if not 0, the layer of an actor standing on it, + 1
  u32 priority : 2; ///< if not 0, the priority of an actor standing on it
  /// MapGridCell's cell behavior and shape; not read here.
  u32 behaviorAndCollision : 16;
};

/// A point the fixed camera looks from or at.
struct ActorCameraPoint {
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
};

/// The start of the 3D camera's state (heap slot HEAP_SLOT_CAMERA,
/// gBattleCamera, a struct BattleCamera), as the fixed camera reads it: the eye
/// and target it looks from and at, or the points that stand in for them.
struct ActorCameraFixed {
  struct ActorCameraPoint eye;
  struct ActorCameraPoint target;
  struct ActorCameraPoint *followEye;      ///< when set, used as the eye
  struct ActorCameraPoint *followPosition; ///< when set, used as the target
};

/// Each frame, with the scrolling camera: places every actor's sprite on the
/// screen.
void ActorSystem_PlaceSprites(void) {
  /* LEFTOVER(frame): three unused points the ROM's frame reserves and never
   * touches. */
  UNUSED struct ActorCameraPoint unusedPoints[3];
  s32 actorIndex;
  struct MapGridScrollState *grid;
  s32 viewX;
  s32 viewZ;
  struct ActorSystemState *system;
  struct Actor *actor;
  struct Sprite *sprite;
  u32 kind;
  FIXED dx;
  FIXED dz;
  FIXED screenTop;
  struct ActorCameraTile *tile;
  u32 priority;
  struct SpriteScale scale;
  struct SpritePlacement drawPosition;
  u32 layerPlusOne;

  // MapGridScrollState owns all three layer cell pointers at their 48-byte
  // stride.
  grid = gMapGridState[0];
  viewX = MapGridScrollState_View(grid)[0] & FIELD_PIXEL_MASK;
  viewZ = MapGridScrollState_View(grid)[1] & FIELD_PIXEL_MASK;
  system = gActorSystemState;
  // Copy the picture decoder (Render_DecodePicture) into its heap slot.
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodePicture,
                   Render_DecodePictureCodeSize);
  system->used = 0;
  actor = gActors;
  for (actorIndex = 0; actorIndex < ACTOR_COUNT; actorIndex++, actor++) {
    if (actor->script == 0)
      continue;
    // An actor at (x, z) = (0, 0) is not placed; only its sprite's VRAM is
    // given up.
    if (actor->x.fixed != 0 || actor->z.fixed != 0) {
      kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == ACTOR_SPRITE_NONE)
        continue;
      // This camera places single sprites only; groups are left.
      if (kind != ACTOR_SPRITE_ONE)
        continue;
      // While a menu has sprite projection suspended, only held actors are
      // drawn.
      if (system->suspended != 0 && actor->retainSpriteCache == 0) {
        struct Sprite *frozen = actor->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      dx = actor->x.fixed - viewX;
      dz = actor->z.fixed - viewZ;
      screenTop = dz - actor->y.fixed;
      sprite = actor->sprite;
      // From 32 pixels left of the screen to 32 past its 240, and from 32
      // pixels above it to 224.
      if (dx > -toFIXED(32) && dx < toFIXED(272) && screenTop > -toFIXED(32) &&
          screenTop < toFIXED(224)) {
        // The cell under the actor: x and z >> 20 are 16-pixel cells, and a
        // row holds 128 of them.
        tile = &((struct ActorCameraTile *)grid->layers[actor->layer]
                     .cells)[(FIELD_CELL(actor->x.fixed)) +
                             FIELD_CELL(actor->z.fixed) * MAPGRID_SIZE];
        if (actor->priorityFlags & ACTOR_PRIORITY_AUTOMATIC) {
          priority = tile->priority;
          if (priority != 0) {
            sprite->image.priority = priority;
            sprite->shadow.priority = priority;
          }
        }
        layerPlusOne = tile->layer;
        if (layerPlusOne != 0)
          actor->layer = layerPlusOne - 1;
        scale.x = Iwram_MulFX(actor->scaleX, sprite->scale);
        scale.y = Iwram_MulFX(actor->scaleY, sprite->scale);
        drawPosition.x = dx;
        drawPosition.height = actor->y.fixed;
        drawPosition.y = dz;
        drawPosition.groundHeight = actor->groundHeight;
        // ACTOR_PRIORITY_UNDERFOOT and _OVERHEAD move the point 320 pixels up
        // or down the map with its height and ground height, so the sprite and
        // shadow stay where they are on screen; only the draw slot changes,
        // which Render_ApplyProjectedPlacement takes from the point's y before
        // its height is taken off.
        if (actor->priorityFlags & ACTOR_PRIORITY_UNDERFOOT) {
          drawPosition.height += -toFIXED(320);
          drawPosition.y += -toFIXED(320);
          drawPosition.groundHeight += -toFIXED(320);
        }
        if (actor->priorityFlags & ACTOR_PRIORITY_OVERHEAD) {
          drawPosition.height += toFIXED(320);
          drawPosition.y += toFIXED(320);
          drawPosition.groundHeight += toFIXED(320);
        }
        Render_ApplyProjectedPlacement(sprite, &drawPosition, &scale,
                                       actor->facing);
        continue;
      }
      // Off screen: the sprite's VRAM goes back to the cache, and dirty makes
      // it redraw when it comes back.
      if (actor->retainSpriteCache == 0) {
        if (!sprite->retainCacheEntry) {
          VramCache_ReleaseVram(sprite->slot);
          sprite->dirty = 1;
        }
      }
    } else {
      kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      if (kind == ACTOR_SPRITE_ONE) {
        sprite = actor->sprite;
        if (actor->retainSpriteCache == 0 && !sprite->retainCacheEntry) {
          VramCache_ReleaseVram(sprite->slot);
          sprite->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
}

/// Unused: returns 1.
UNUSED s32 ActorCamera_ReturnTrue(void) { return 1; }

/// Each frame, with the fixed camera: places every actor's sprite, from the
/// 64th actor down, facing them by the angle from the eye to the target.
void ActorSystem_ProjectSprites(void) {
  s32 i;
  struct ActorCameraFixed *camera;
  struct ActorSystemState *system;
  struct ActorCameraPoint *eye;
  struct ActorCameraPoint *target;
  struct Actor *actor;
  s32 angle;
  s32 part;
  s32 kind;
  s32 *position;
  void **sprites;
  void *sprite;
  FIXED groupScale[2];
  FIXED scale[2];
  /* LEFTOVER(frame): two unused points the ROM's frame reserves and never
   * touches. */
  UNUSED struct ActorCameraPoint unusedPoints[2];

  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  system = gActorSystemState;
  RamCode_LoadSlot(HEAP_SLOT_FRAME_SCRATCH, Render_DecodePicture,
                   Render_DecodePictureCodeSize);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->followEye != NULL)
    eye = camera->followEye;
  if (camera->followPosition != NULL)
    target = camera->followPosition;
  // The camera's heading over the ground, from the eye's offset from the
  // target in whole pixels; each sprite's facing is drawn relative to it, so
  // a sprite keeps its world heading as the camera turns.
  angle = (s16)Trig_Atan(FIXEDtoINT(eye->x - target->x),
                         FIXEDtoINT(eye->z - target->z));
  system->used = 0;
  Matrix_Initialize();
  // Under the fixed view matrix every sprite also faces an eighth of a turn
  // further back.
  if (GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
    angle += -(DEGtoANG(45));
    Iwram_MultiplyMatrix(gCamera_FixedViewMatrix);
    Matrix_LookAt((s32 *)eye, (s32 *)target);
  } else {
    Matrix_SetLookAt((s32 *)eye, (s32 *)target);
  }
  // The actors are placed from the last down.
  actor = gActors;
  actor += ACTOR_COUNT - 1;
  {
    FIXED fixedScale = FIXED_ONE;

    for (i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
      if (actor->script == 0)
        continue;
      kind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
      switch (kind) {
      case ACTOR_SPRITE_ONE:
        position = &actor->x.fixed;
        scale[0] = actor->scaleX;
        scale[1] = actor->scaleY;
        sprite = actor->sprite;
        if (GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
          scale[0] = fixedScale;
          scale[1] = fixedScale;
        }
        Render_PlaceProjectedSprite(sprite, position, scale,
                                    actor->facing + angle, actor->layer);
        break;
      case ACTOR_SPRITE_GROUP:
        groupScale[0] = actor->scaleX;
        groupScale[1] = actor->scaleY;
        if (GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
          groupScale[0] = fixedScale;
          groupScale[1] = fixedScale;
        }
        // A group's sprite field points to its four sprite pointers; empty
        // ones are null.
        sprites = (void **)actor->sprite;
        for (part = 0; part < 4; part++) {
          sprite = *sprites++;
          if (sprite != NULL)
            Render_PlaceProjectedSprite(sprite, &actor->x.fixed, groupScale,
                                        actor->facing + angle, actor->layer);
        }
        break;
      case ACTOR_SPRITE_NONE:
        break;
      }
    }
    HeapSlot_Release(HEAP_SLOT_FRAME_SCRATCH);
  }
}
