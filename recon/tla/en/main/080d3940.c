// Near miss, differ 12 of 231 (tla-en, 2026-10-31; diff 080d3940,080d3a10):
// Func_080d3940 differ 4 of 99, Func_080d3a10 differ 8 of 132, each measured
// on a copy that keeps one function. Every difference is a store the native
// issues one slot earlier: in Func_080d3940 the store of object->z
// (str r2, [r5, #8]) before the ands, and the strb of the zero into
// collisionFlags before mov r1, r8; in Func_080d3a10 the same two, plus the
// store of object->x (str r1, [r5]) before lsls r3, r3, #6. The sched2 dump
// ranks each store one priority below the insn the native puts after it
// (z store 9 against the ands and mov r9, r3 at 10; zero strb 8 against
// mov r1, r8 at 9): its memory dependence on the next load costs 1 here, and
// a tie would go to the store, which has more dependents. Scheduler order the
// C does not move; the permuter (4 minutes each) found nothing better.
// Func_080d3b28, once a third function here, is adopted in
// field/object/active.c.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "dma.h"

struct Actor *Actor_GetById(u32);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           u32 value);
void SetMapCellCollisionFar(s32 terrain, s32 x, s32 z, s32 value);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
u8 *HeapSlot_Alloc(s32 block, s32 size);
void HeapSlot_Release(s32 block);
void Func_08038248(s32 resource, u8 *buffer);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void ResourceMetadata_ClearRecordFar(void *record);

struct ObjectSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 vramBlock;
  u8 unk11;
  u16 rotation;
  u8 unk14[2];
  s8 offsetX;
  s8 offsetY;
  u8 frame;
  u8 frameTimer;
  u8 nextFrame;
  u8 unk1b;
  u8 unk1c[0xc];
  void *record;
};

void Func_080d3940(u32 id, u32 angle) {
  s32 position[3];
  struct Actor *object = Actor_GetById(id);
  struct ObjectSprite *sprite = object->animation;

  if (object != NULL && sprite != NULL) {
    Actor_SetMode(object, 0);
    angle = (angle + 0x2000) & 0xc000;
    position[0] = object->x;
    position[2] = object->z;
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    Vector_AddPolarOffset(0x100000, angle, position);
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    switch (angle) {
    case 0:
      sprite->offsetX -= 3;
      sprite->offsetY += 5;
      break;
    case 0x8000:
      sprite->offsetX += 2;
      sprite->offsetY += 4;
      break;
    case 0xc000:
      sprite->offsetY += 1;
      break;
    case 0x4000:
      sprite->offsetY += 5;
      break;
    }
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    sprite->nextFrame = 0;
    sprite->rotation = angle + 0x4000;
  }
}

void Func_080d3a10(u32 id, u32 angle, s32 mode) {
  s32 position[3];
  struct Actor *object = Actor_GetById(id);
  struct ObjectSprite *sprite = object->animation;

  if (object != NULL && sprite != NULL) {
    if (mode != -1)
      Actor_SetMode(object, mode);
    position[0] = object->x;
    angle = (angle + 0x2000) & 0xc000;
    position[2] = object->z;
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    Vector_AddPolarOffset(0x100000, angle, position);
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    switch (angle) {
    case 0:
      object->x += 0x80000;
      object->z += 0x10000;
      sprite->offsetX -= 4;
      sprite->offsetY += 15;
      break;
    case 0x8000:
      object->x += -0x80000;
      object->z += 0x10000;
      sprite->offsetX += 3;
      sprite->offsetY += 16;
      break;
    case 0xc000:
      sprite->offsetY += 1;
      break;
    case 0x4000:
      object->z += 0x80000;
      sprite->offsetY += 13;
      break;
    }
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    sprite->nextFrame = 0;
    sprite->rotation = angle + 0x4000;
  }
}
