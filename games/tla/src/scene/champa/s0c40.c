/// @file
/// Scene code of overlay 0x682, in ROM order, from s0c40's offset up to the next object the overlay links.

#include "types.h"
#include "field_event.h"
#include "event_inline.h"
#include "ram_buffer.h"

extern const u8 gData_02003620[];

void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct FieldActor *object;
  struct FieldSprite *sprite;

  object = FieldActor_Create(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Func_020029dc(object, 1);
    Func_020029e4(object, gData_02003620);
    object->motionFlags = 0;
    ((struct {
      u8 unk00[94];
      u16 frames;
    } *)object)
        ->frames = frames;

    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

void Func_02000c90(s32 firstId, s32 secondId) {
  struct FieldActor *first = ((struct FieldActor *)Actor_Get(firstId));
  struct FieldActor *second = ((struct FieldActor *)Actor_Get(secondId));
  s32 rise = 0x1c0000;
  s32 y;

  y = first->y;
  Func_02000c40(first->x, y + rise, first->z, 0x1000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  y = second->y;
  Func_02000c40(second->x, y + rise, second->z, 0xf000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02000c40(first->x, first->y + rise, first->z, 0x1000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02000c40(second->x, second->y + rise, second->z, 0xf000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
}

struct ChampaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

s32 ObjectTable_ReadActiveValue(s32 id);
s32 BattleEffect_GetResourceId(s32 object);
void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
void Func_02002a2c(s32 object, s32 a, s32 b, s32 c);
s32 Message_AllTyped(void);

void Func_02000d4c(s32 firstId, s32 firstX, s32 firstY, s32 firstA, s32 firstB,
                   s32 secondId, volatile s32 secondX, s32 secondY, s32 secondA,
                   s32 secondB, volatile s32 frames) {
  /* FAKEMATCH(volatile): the frames volatile, read from their stack slot at
   * each use; as s32 frames, 44 of 79 instructions differ in tla-en. */
  struct ChampaEventWork *work = Ram_HeapSlots->eventWork;
  s32 first = ObjectTable_ReadActiveValue(firstId);
  s32 second = ObjectTable_ReadActiveValue(secondId);

  Message_OpenWindow(work->message++, firstX, firstY,
                     BattleEffect_GetResourceId(first) << 16);
  Func_02002a2c(first, 0, firstA, firstB);
  if (frames != 0) {
    while (!Message_AllTyped())
      System_Wait(frames);
  }
  /* FAKEMATCH(volatile): the counter's second store and the second x both
   * volatile, so x is loaded after the store; as work->message++ and s32
   * secondX, 4 of 79 instructions differ in tla-en. */
  Message_OpenWindow((*(volatile s16 *)&work->message)++, secondX, secondY,
                     BattleEffect_GetResourceId(second) << 16);
  Func_02002a2c(second, 0, secondA, secondB);
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
}
