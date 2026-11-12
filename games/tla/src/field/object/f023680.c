#include "actor_view.h"

void Animation_SetStateFlags(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->stateFlags = value;
}

void Animation_SetDisplayFlag(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->displayFlag = value;
}

#include "field_sprite.h"

void Animation_ApplyChildValues(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    Animation_ApplyChildValuesToRecord(
        ((struct ActorSpriteView *)object->sprite), value);
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  if (object == NULL || object->animationKind != 1) {
    return;
  }
  sprite = ((struct FieldSprite *)object->sprite);
  sprite->priority = priority;
  sprite->partPriority = priority;
  object->priorityFlags &= ~1;
}

#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "battle.h"
#include "system.h"
#include "heap_release.h"
#include "resource.h"
#include "render_input.h"
#include "edition.h"
#include "fixed_math.h"

struct IconSprite {
  u8 unk00[4];
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

struct IconOwner {
  u8 unk00[80];
  struct IconSprite *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

void Func_08038248(s32 id);

/// Shows icon id on its owner's 16 by 16 sprite: Func_08038248 prepares the
/// icon in heap slot 68, whose tiles at +0x400 go to the sprite's VRAM cache
/// slot, and the sprite is reset to scale 1.0, 16-colour palette 0.
// No ☀️ twin found.
void Func_0802372c(struct IconOwner *owner, s32 id) {
  struct IconSprite *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = HeapSlot_Alloc(68, 0x608);
  Func_08038248(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  HeapSlot_Release(68);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = Q16_ONE;
  sprite->oam.index = tile;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
}

s32 Func_080237c8(u8 *object) {
  u8 *track = *(u8 **)(object + 0x50);

  if (object != NULL && track != NULL && track != (u8 *)-0x28)
    return **(s16 **)(track + 0x28);
  return 0;
}

void Animation_WaitForTrackEnd(void *track);

void Func_080237e8(u8 *object) {
  if (object != NULL)
    Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

void Func_0802386c(void);
void ObjectSystem_UpdateCameraFixed(void);

void Graphics_EnableObjLayerAndCallbacks(void) {
  u16 dispcnt;

  Task_SuspendAll(Func_0802386c);
  Task_SuspendAll(ObjectSystem_UpdateCameraFixed);
  BattleEffect_ApplyColorToTargetBufferFar(0x10000, 1);
  BattleEffect_StartBufferInterpolationFar(1);
  wait(1);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

void ActorSystem_ResumeProjectionAndHideLayers(void) {
  u16 dispcnt;

  Task_ResumeAll(Func_0802386c);
  Task_ResumeAll(ObjectSystem_UpdateCameraFixed);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = dispcnt & 0xe1ff;
}

s32 Actor_ReturnTrue(void) { return 1; }

#include "ram_buffer.h"
#include "dma.h"
#include "map.h"
#include "iwram_call.h"
#include "actor.h"
#include "field/sprite.h"
#include "field/field.h"

struct PlacedSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry; ///< bit 0
  u8 unk12[7];
  u8 dirty;
  u8 unk1a[0x0b];
  u8 low25 : 2;
  u8 shadowPriority : 2;
  u8 high25 : 4;
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct MapPriorityWork {
  u8 unk000[0x101];
  s8 basePriority;
};

// The decoder's block: its code, tables and work area (ld_script.ld). Only
// the code and tables are copied.
extern u8 Render_DecodePictureCode[];
extern u8 Render_DecodePictureCopyEnd[];
extern u8 Render_DecodePictureBlockSize[];

s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void Func_08022318(struct PlacedSprite *sprite, struct SpritePlacement *point,
                   s32 *scale, s32 facing, s32 depth);
void Func_08021a84(struct PlacedSprite *sprite, s32 facing);

#define BASE_PRIORITY(map) (((struct MapPriorityWork *)(map))->basePriority)
// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, z)                                             \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 16) >> 30)
// The layer field (bits 13..12) of a cell's word: the layer plus one.
#define CELL_LAYER(cells, x, z)                                                \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 18) >> 30)

void Func_0802386c(void) {
  // Unused: gives the stack frame its 36 bytes, as The Broken Seal's.
  struct CameraPoint unusedPoints[3];
  struct SpritePlacement point;
  struct SpriteScale scale;
  s32 objectIndex;
  struct MapWork *map;
  s32 *view;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct Actor *object;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  s32 screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  u32 *cells;
  s32 feetZ;
  s32 objectHeight;
  s32 probeX;
  s32 priority;
  s32 leftPriority;
  s32 middlePriority;
  s32 rightPriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  system = Ram_HeapSlots->objectSystem;
  originY = map->origin[1];
  // The decoder's copy in a once-only block, as in
  // ObjectSystem_UpdateCameraFixed, keeps the first call's arguments after the
  // followed point's height; as a plain call, 6 of 606 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  {
    // The zero is set before the objects are read.
    u32 zero = 0;

    object = Ram_HeapSlots->scriptObjects;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->script == NULL)
      continue;
    if (object->x != 0 || object->z != 0) {
      kind = object->animationKind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if ((u16)system->suspended != 0 && !(object->unk5c[0] & 1)) {
        struct PlacedSprite *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = object->sprite;
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000 &&
          (priorityFlags = &object->priorityFlags, !(*priorityFlags & 16))) {
        terrainId = &object->terrainId;
        layer = *terrainId & 3;
        cells = (u32 *)map->layers[layer].cells;
        if (*priorityFlags & 1) {
          probeX = object->x - 0x70000;
          feetZ = object->z + 0x20000;
          objectHeight = object->terrainHeight >> 16;
          priority = 0;
          leftPriority = 0;
          middlePriority = 0;
          if (!(*priorityFlags & 32)) {
            /* FAKEMATCH(temporary): the base priority read through a
             * function-scope pointer, which gives the two spills at sp+12 and
             * sp+16 their order; as priority = BASE_PRIORITY(map), 8 of 606
             * instructions differ in tla-en. */
            basePriority = &BASE_PRIORITY(map);
            priority = *basePriority;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise =
                  (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX += 0xd0000;
            if (Func_0802d45c(layer, probeX, feetZ) == object->terrainHeight) {
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
              height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
              rise =
                  (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            height = Func_0802d45c(layer, probeX, feetZ + 0x100000) >> 16;
            rise = (Func_0802d45c(layer, probeX, feetZ + 0x200000) >> 16) - 16;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = BASE_PRIORITY(map) + 1;
          } else {
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += 0xd0000;
            if (object->terrainHeight == Func_0802d45c(layer, probeX, feetZ))
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
          }
          // An odd priority is preferred, left first.
          if (leftPriority != 0 || middlePriority != 0 || rightPriority != 0) {
            priority = leftPriority;
            if (!(priority & 1)) {
              if (middlePriority != 0)
                priority = middlePriority;
              if (!(priority & 1) && rightPriority != 0)
                priority = rightPriority;
            }
          }
          if (priority != 0 && object->animationKind == 1) {
            struct PlacedSprite *placed = object->sprite;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, object->x, object->z);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        point.x = dx;
        point.height = object->y - originY;
        point.y = dz - originY;
        point.groundHeight = object->terrainHeight - originY;
        if (*priorityFlags & 2) {
          point.height += 0xfec00000;
          point.y += 0xfec00000;
          point.groundHeight += 0xfec00000;
        }
        if (*priorityFlags & 4) {
          point.height += 0x1400000;
          point.y += 0x1400000;
          point.groundHeight += 0x1400000;
        }
        if (*priorityFlags & 8)
          Func_08022318(sprite, &point, scale.values, object->facing,
                        ((u8 *)object)[0x63]);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, object->facing);
        continue;
      }
      if (object->unk5c[0] & 2) {
        Func_08021a84(sprite, object->facing);
      } else if (!(object->unk5c[0] & 1) && !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = object->animationKind & 15;
      if (kind == 1) {
        struct PlacedSprite *frozen = object->sprite;

        if (object->unk5c[0] == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(80);
}

#include "graphics.h"
#include "transform.h"

// The fixed camera: each frame it places every object's sprites, facing them
// by the angle from the eye to the target. The Lost Age places each object in
// a function of its own, the body of ☀️'s ObjectSystem_UpdateCameraFixed
// loop.

struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    ///< when set, used as the eye
  struct CameraPoint *targetOverride; ///< when set, used as the target
};

struct PlacedObject {
  u8 unk00[2];
  u16 active;
  u8 unk04[2];
  u16 angle;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *target;
  u8 kind;
  u8 unk55[0x2b];
};

extern const s32 gData_0802eb90[2];
extern const s32 gData_0802eb98[12];
s32 ArcTan2(s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void Func_0802254c(void *sprite, s32 *point, const s32 *scale, s32 facing,
                   s32 depth);

s32 ObjectCamera_ReturnTrue(void) { return 1; }

/// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
/// group's four sprites (kind 2), turned by angle. While battle commands are
/// chosen (flag 363) the sprites are drawn at 1.0 scale.
void Func_08023d80(struct PlacedObject *object, s32 angle) {
  const s32 *scale = object->scale;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Test(363))
    scale = gData_0802eb90;
  switch (object->kind & 15) {
  case 1:
    point = object->position;
    sprite = object->target;
    if (!(object->flags & 0x10))
      Func_0802254c(sprite, point, scale, object->angle + angle, object->layer);
    break;
  case 2:
    sprites = object->target;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Func_0802254c(sprite, object->position, scale, object->angle + angle,
                      object->layer);
    }
    break;
  case 0:
    break;
  }
}

/// Each frame, with the fixed camera: places every active object, from the
/// 64th down. While battle commands are chosen (flag 363) it uses the fixed
/// view matrix and turns the objects by an eighth of a turn less.
void ObjectSystem_UpdateCameraFixed(void) {
  struct FixedCamera *camera = Ram_HeapSlots->cameraWork;
  struct ObjectSystemWork *system = Ram_HeapSlots->objectSystem;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct PlacedObject *object;
  s32 angle;
  s32 i;
  // Unused: gives the stack frame its 12 bytes.
  struct CameraPoint unused;

  // The decoder's copy in a once-only block makes the stack frame before the
  // first call's arguments; as a plain call, 6 of 93 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  if (GameFlag_Test(363)) {
    Render_ResetTransformState();
    angle += 0xffffe000;
    Iwram_TransformMatrixEntry(gData_0802eb98);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    SceneTransform_RewindStack();
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  object = Ram_HeapSlots->scriptObjects;
  object += 63;
  for (i = 63; i >= 0; i--, object--) {
    if (object->active != 0)
      Func_08023d80(object, angle);
  }
  HeapSlot_Release(80);
}

// Object scripts: move the instruction cursor and control byte54.

#include "script.h"

/// Jumps to the script the next word points to and returns 1 to continue
/// with its first command.
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc) {
  s32 zero;

  // Jump to the script pointed to by the following word.
  sc->script = (const s32 *)sc->script[sc->cursor + 1];
  zero = 0;
  sc->cursor = zero;
  return 1;
}

/// Clears byte54 and advances to the next command.
s32 Script_ClearByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 0;
  sc->cursor++;
  return 1;
}

/// Sets byte54 to 1 and advances to the next command.
s32 Script_SetByte54(struct ScriptInterpreter *sc) {
  sc->byte54 = 1;
  sc->cursor++;
  return 1;
}

#include "debug.h"
#include "game.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"

/// Waits up to 600 frames for a field object's movement target to be unset.
void Script_WaitForEventTimeout(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Actor_IsTargetUnset(fieldObject) == 0) {
    wait(1);
    waitedFrames++;
  }
}

s32 Actor_IsTargetUnset(struct Actor *object) {
  s32 first;
  s32 second;

  if (object->motionFlags == 0) {
    second = object->targetX;
    if (second != (s32)0x80000000)
      return 0;
    first = object->targetY;
  } else {
    first = object->targetX;
    second = (s32)0x80000000;
  }
  if (first != second || object->targetZ != first)
    return 0;
  return 1;
}

s32 Func_08023f94(struct Actor *object) { return Actor_IsTargetUnset(object); }

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
};

/// An object as the motion update reads it (struct Actor's layout).
struct MotionObject {
  struct MotionScript *script;
  s16 step;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  s32 groundHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 unk4c;
  void *animation;
  u8 animationKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 unk57;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 actionFlags;
  u8 paused;
  u8 unk5c[2];
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 unk62[0x0a];
  void (*update)(struct MotionObject *object);
  s32 moved[3];
  struct MotionObject *standingOn;
};

typedef s32 (*MotionCommand)(struct MotionObject *object);

extern const MotionCommand gData_0802f204[];
extern const s32 gData_0802ebc8[];

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));
s32 ScriptObject_CheckOverlap(struct MotionObject *object, s32 *values);

#define TARGET_UNSET ((s32)0x80000000)

/// A whole-pixel distance squared.
static __inline__ s32 Object_Square(s32 value) { return value * value; }

void Func_08023f9c(void) {
  struct MotionObject *object;
  s32 objectsRemaining;
  s32 x;
  s32 y;
  s32 z;
  s32 ground;
  s32 arrivedAtTarget;
  s32 pos[3];
  s32 dx;
  s32 dy;
  s32 dz;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 distance;
  s32 speed;
  s32 reducedSpeed;
  s32 ratio;
  s32 maximumChange;
  s32 standing;

  object = Ram_HeapSlots->scriptObjects;
  for (objectsRemaining = 63; objectsRemaining >= 0;
       objectsRemaining--, object++) {
    struct MotionScript *script;

    arrivedAtTarget = 0;
    script = object->script;
    if (script == NULL)
      continue;
    if (object->update != NULL) {
      object->update(object);
      script = object->script;
    }
    if (script == NULL)
      continue;
    if (object->paused != 0) {
      object->moved[0] = arrivedAtTarget;
      object->moved[1] = arrivedAtTarget;
      object->moved[2] = arrivedAtTarget;
      continue;
    }

    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
        u32 command = script->word[object->step];

        if (command <= 63) {
          if (gData_0802f204[command](object) == 0)
            break;
        } else {
          object->step++;
        }
        script = object->script;
      }
      if (object->script == NULL)
        continue;
    }

    x = object->x;
    y = object->y;
    z = object->z;
    if (object->frozen == 0) {
      if (object->motionFlags == 0) {
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dy = (object->targetY - y) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dy) +
                                Object_Square(dz));
          if (distance == 0) {
            x = object->targetX;
            y = object->targetY;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance << 16, object->acceleration);
            object->velocityX += dx * ratio;
            object->velocityY += dy * ratio;
            object->velocityZ += dz * ratio;
            velocityX = object->velocityX;
            velocityY = object->velocityY;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityY, velocityY) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityY = Iwram_MulQ16(velocityY, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dy = object->velocityY;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                            Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityY = Iwram_MulQ16(dy, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityY = speed;
            object->velocityZ = speed;
          }
        }
      } else {
        // On the ground: steer in the XZ plane the same way.
        if (object->targetX != TARGET_UNSET) {
          dx = (object->targetX - x) / 65536;
          dz = (object->targetZ - z) / 65536;
          distance = Iwram_Sqrt(Object_Square(dx) + Object_Square(dz)) << 16;
          if (distance <= 0x00ffffff) {
            dx = object->targetX - x;
            dz = object->targetZ - z;
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          }
          if (distance == 0) {
            x = object->targetX;
            z = object->targetZ;
          } else {
            ratio = Iwram_RatioMulQ14(distance, object->acceleration);
            object->velocityX += Iwram_MulQ16(dx, ratio);
            object->velocityZ += Iwram_MulQ16(dz, ratio);
            velocityX = object->velocityX;
            velocityZ = object->velocityZ;
            speed = FixedSqrt(Iwram_MulQ16(velocityX, velocityX) +
                              Iwram_MulQ16(velocityZ, velocityZ));
            if (speed > object->speedLimit) {
              ratio = Iwram_RatioMulQ14(speed, object->speedLimit);
              object->velocityX = Iwram_MulQ16(velocityX, ratio);
              object->velocityZ = Iwram_MulQ16(velocityZ, ratio);
            }
          }
        } else {
          dx = object->velocityX;
          dz = object->velocityZ;
          speed = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
          if (speed != 0) {
            reducedSpeed = speed - object->acceleration;
            if (reducedSpeed < 0)
              reducedSpeed = 0;
            ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
            object->velocityX = Iwram_MulQ16(dx, ratio);
            object->velocityZ = Iwram_MulQ16(dz, ratio);
          } else {
            object->velocityX = speed;
            object->velocityZ = speed;
          }
        }

        // Follow the ground, or the top of an object 16 pixels below.
        if (object->motionFlags & 1) {
          velocityX = x + object->velocityX;
          velocityZ = z + object->velocityZ;
          standing = 0;
          object->standingOn = NULL;
          if (object->priorityFlags & 0x40) {
            struct MotionObject *other = Ram_HeapSlots->scriptObjects;
            s32 i;

            for (i = 0; i < 64; i++, other++) {
              s32 dy;

              if (other->script == NULL)
                continue;
              if (!(other->priorityFlags & 0x80))
                continue;
              if (other == object)
                continue;
              if (((velocityX - other->x + 0x80000) >> 20) != 0)
                continue;
              if (((velocityZ - other->z + 0x80000) >> 20) != 0)
                continue;
              dy = y - other->y;
              if ((dy - 0x100000 < 0 ? -(dy - 0x100000) : dy - 0x100000) >
                  0x7ffff)
                continue;
              standing = 1;
              ground = other->y + 0x100000;
              object->standingOn = other;
              break;
            }
          }
          if (!standing)
            ground = Func_0802d45c(object->terrainId, velocityX, velocityZ);
          velocityY = ground - object->groundHeight;
          if (ground - y > -0x40000)
            y += velocityY;
          if (velocityY < 0)
            velocityY = -velocityY;
          maximumChange = object->acceleration / 2;
          if (velocityY > maximumChange)
            velocityY = maximumChange;
          velocityY *= 3;
          // This slowdown and the landing bounce below call IwramMulQ16 by its
          // linked name, so each call reads the routine's address from the
          // literal pool after the call before it.
          if (velocityY != 0 && !(object->motionFlags & 16) && !standing) {
            dx = object->velocityX;
            dy = object->velocityY;
            dz = object->velocityZ;
            speed = FixedSqrt(IwramMulQ16(dx, dx) + IwramMulQ16(dy, dy) +
                              IwramMulQ16(dz, dz));
            if (speed != 0) {
              reducedSpeed = speed - velocityY;
              if (reducedSpeed < 0)
                reducedSpeed = 0;
              ratio = Iwram_RatioMulQ14(speed, reducedSpeed);
              object->velocityX = IwramMulQ16(dx, ratio);
              object->velocityY = IwramMulQ16(dy, ratio);
              object->velocityZ = IwramMulQ16(dz, ratio);
            }
          }
          object->groundHeight = ground;
        }

        // Gravity, and a bounce that loses energy on landing.
        if (object->motionFlags & 2) {
          ground = object->groundHeight;
          if (y > ground)
            object->velocityY -= object->gravity;
          if (y + object->velocityY <= ground) {
            if (object->velocityY < 0) {
              s32 rebound = -IwramMulQ16(object->velocityY, object->bounce);

              object->velocityY = rebound;
              y = ground - rebound;
              if ((rebound < 0 ? -rebound : rebound) <= object->gravity) {
                y = ground;
                object->velocityY = 0;
              }
            } else {
              y = ground;
            }
          }
        }

        // Float: bob up and down on a triangle wave.
        if (object->motionFlags & 4) {
          u32 phase = object->bounce & 63;

          if (object->motionFlags & 8)
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 16;
          else
            object->velocityY =
                gData_0802ebc8[phase >> 1] * object->gravity / 64;
          object->bounce++;
        }
      }
    }

    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;
    if (object->collisionFlags & 0x80) {
      pos[0] = x;
      pos[1] = y;
      pos[2] = z;
      if (ScriptObject_CheckOverlap(object, pos) != 0) {
        object->refused++;
        continue;
      }
      object->refused = 0;
    }

    switch (object->arrivalAxis) {
    case 16:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrivedAtTarget = 1;
      break;
    case 17:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrivedAtTarget = 1;
      break;
    case 18:
      if (z == object->targetZ ||
          ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
        arrivedAtTarget = 1;
      break;
    }
    if (arrivedAtTarget != 0) {
      if (object->snapToTarget != 0) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }

    object->moved[0] = x - object->x;
    object->moved[1] = y - object->y;
    object->moved[2] = z - object->z;
    object->x = x;
    object->y = y;
    object->z = z;

    // Turn to face the direction of travel, at most a sixteenth of a turn a
    // frame.
    if (object->actionFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        s32 facingStep;
        s32 maximumFacingStep;

        facingStep = (s16)(ArcTan2(z, x) - object->angle);

        maximumFacingStep = 0x1000;
        if (facingStep > maximumFacingStep)
          facingStep = maximumFacingStep;
        maximumFacingStep = -0x1000;
        if (facingStep < maximumFacingStep)
          facingStep = maximumFacingStep;
        object->angle += facingStep;
      }
    }
  }
}

#include "macros.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void Func_080246b8(void) {
  struct Actor *object = Ram_HeapSlots->scriptObjects;
  s32 i;

  for (i = 63; i >= 0; i--, object++) {
    if (object->script != NULL) {
      s32 x = object->x;
      s32 y = object->y;
      s32 z = object->z;

      if (object->priorityFlags & 64) {
        struct Actor *carrier = (struct Actor *)WORD_AT(object, 0x7c);

        if (carrier != NULL) {
          if (WORD_AT(carrier, 0x74) != 0) {
            if (y == object->terrainHeight) {
              object->terrainHeight = carrier->y + 0x100000;
              y = object->terrainHeight;
            } else {
              object->terrainHeight += WORD_AT(carrier, 0x74);
              y += WORD_AT(carrier, 0x74);
            }
          }
          x += WORD_AT(carrier, 0x70);
          z += WORD_AT(carrier, 0x78);
        }
      }
      SET_POSITION(object, x, y, z);
    }
  }
}

// Objects: set position and cancel motion.

/// Sets the position, clears the destination and zeroes the velocity.
/// obj: object.
/// x, y, z: 16.16 fixed-point position.
void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z) {
  obj->z = z;
  obj->targetX = 0x80000000; // No destination.
  obj->targetY = 0x80000000;
  obj->targetZ = 0x80000000;
  obj->x = x;
  obj->y = y;
  SET_VELOCITY(obj, 0, 0, 0);
}

void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  s32 dx;
  s32 distance;
  s32 dz;
  s32 squaredZ;
  s32 dy;
  u8 *arrivalAxis;

  dx = (x - object->x) / Q16_ONE;
  dy = (y - object->y) / Q16_ONE;
  dz = (z - object->z) / Q16_ONE;
  squaredZ = dz * dz;
  distance = Iwram_Sqrt(dx * dx + dy * dy + squaredZ) << 16;
  if (distance < 0x100000) {
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                         Iwram_MulQ16(dz, dz));
  }
  if (distance < 0x10000) {
    object->x = x;
    object->y = y;
    object->z = z;
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    return;
  }
  if (object->snapToTarget == 0) {
    s32 brakingDistance;
    s32 ratio;

    brakingDistance =
        Iwram_RatioMulQ14(object->acceleration,
                          Iwram_MulQ16(object->speedLimit, object->speedLimit));
    if (distance > brakingDistance)
      brakingDistance = distance - brakingDistance / 2;
    else
      brakingDistance = distance / 2;
    ratio = Iwram_RatioMulQ14(distance, brakingDistance);
    x = object->x + Iwram_MulQ16(x - object->x, ratio);
    y = object->y + Iwram_MulQ16(y - object->y, ratio);
    z = object->z + Iwram_MulQ16(z - object->z, ratio);
  }
  object->targetX = x;
  object->targetY = y;
  object->targetZ = z;
  dx = x - object->x;
  dy = y - object->y;
  dz = z - object->z;
  arrivalAxis = &object->unk56;
  *arrivalAxis = 16;
  if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
    *arrivalAxis = 18;
    dx = dz;
  }
  if (object->motionFlags == 0) {
    if (dx < 0)
      dx = -dx;
    if (dx < (dy < 0 ? -dy : dy))
      *arrivalAxis = 17;
  }
}

void ObjectList_CollectActive(u8 *object, u8 **out) {
  u8 *end = object + 0x700;

  do {
    if (*(u16 *)(object + 2) != 0)
      *out++ = object;
    object += 128;
  } while (object != end);
  *out = NULL;
}
