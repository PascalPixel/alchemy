/// @file
/// The actor cameras, which place every actor's sprite each frame.
// Part of field.c.

#include "field_internal.h"

/// A map cell's word, as the camera reads it.
struct ActorCameraTile {
  u32 block : 12;   ///< the cell picture number
  u32 layer : 2;    ///< if not 0, the layer of an actor standing on it, + 1
  u32 priority : 2; ///< if not 0, the priority of an actor standing on it
  /// MapGridCell's cell behavior and shape; not read here.
  u32 behaviorAndCollision : 16;
};

/// A point the fixed camera looks from or at.
struct ActorCameraPoint {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};

/// The start of the 3D camera's work (heap slot HEAP_SLOT_CAMERA,
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
   * touches: of the 80 bytes it reserves (sub sp, #80), no instruction
   * addresses the 36 at sp+44 to sp+79, above the position; as without them, 4
   * of 267 instructions differ in tbs-en. */
  UNUSED struct ActorCameraPoint unusedPoints[3];
  s32 actorIndex;
  struct MapGridScrollWork *grid;
  s32 viewX;
  s32 viewZ;
  struct ActorSystemWork *system;
  struct Actor *actor;
  struct Sprite *sprite;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 screenTop;
  struct ActorCameraTile *tile;
  u32 priority;
  struct SpriteScale scale;
  struct SpritePlacement drawPosition;
  u32 layerPlusOne;

  // MapGridScrollWork owns all three layer cell pointers at their 48-byte
  // stride.
  grid = gMapGridWork[0];
  {
    s32 *view = grid->view;

    viewX = view[0] & FIELD_PIXEL_MASK;
    viewZ = view[1] & FIELD_PIXEL_MASK;
  }
  system = gActorSystemWork;
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
      kind = actor->spriteStorageKind & 15;
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
      if (dx > -px(32) && dx < px(272) && screenTop > -px(32) &&
          screenTop < px(224)) {
        // The cell under the actor: x and z >> 20 are 16-pixel cells, and a
        // row holds 128 of them.
        tile = &((struct ActorCameraTile *)grid->layers[actor->layer]
                     .cells)[(FIELD_CELL(actor->x.fixed)) +
                             (FIELD_CELL(actor->z.fixed) << 7)];
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
        scale.x = Iwram_MulQ16(actor->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(actor->scaleY, sprite->scale);
        drawPosition.x = dx;
        drawPosition.height = actor->y.fixed;
        drawPosition.y = dz;
        drawPosition.groundHeight = actor->groundHeight;
        // ACTOR_PRIORITY_UNDERFOOT and bit 2 move the point 320 pixels up or
        // down the map with its height and ground height, so the sprite and
        // shadow stay where they are on screen; only the draw slot changes,
        // which Render_ApplyProjectedPlacement takes from the point's y before
        // its height is taken off.
        if (actor->priorityFlags & ACTOR_PRIORITY_UNDERFOOT) {
          drawPosition.height += -px(320);
          drawPosition.y += -px(320);
          drawPosition.groundHeight += -px(320);
        }
        if (actor->priorityFlags & 4) {
          drawPosition.height += px(320);
          drawPosition.y += px(320);
          drawPosition.groundHeight += px(320);
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
      kind = actor->spriteStorageKind & 15;
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
  struct ActorSystemWork *system;
  struct ActorCameraPoint *eye;
  struct ActorCameraPoint *target;
  struct Actor *actor;
  s32 angle;
  s32 part;
  s32 kind;
  s32 *position;
  void **sprites;
  void *sprite;
  s32 groupScale[2];
  s32 scale[2];
  /* LEFTOVER(frame): two unused points the ROM's frame reserves and never
   * touches: of the 52 bytes it reserves (sub sp, #52), no instruction
   * addresses the 24 at sp+28 to sp+51, above the scales; as without them, 4 of
   * 181 instructions differ in tbs-en. */
  UNUSED struct ActorCameraPoint unusedPoints[2];

  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  system = gActorSystemWork;
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
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  Transform_Reset();
  // Under the fixed view matrix every sprite also faces an eighth of a turn
  // further back.
  if (GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
    angle += -(TRIG_TURN / 8);
    Iwram_TransformMatrix(gCamera_FixedViewMatrix);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  // The actors are placed from the last down.
  actor = gActors;
  actor += ACTOR_COUNT - 1;
  {
    s32 fixedScale = Q16_ONE;

    for (i = ACTOR_COUNT - 1; i >= 0; i--, actor--) {
      if (actor->script == 0)
        continue;
      kind = actor->spriteStorageKind & 15;
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
