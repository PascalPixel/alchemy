#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct FinishWork {
  u8 unk000[16];
  struct Actor *object;
  u8 unk014[8];
  s16 quantity;
  u8 unk01e[4];
  s8 sound;
  s8 showQuantity;
  u8 unk024[0x6ee];
  s8 messageRow;
  u8 messageShown;
};

extern char MsgBlank2;

s32 GameFlag_Test(s32 flag);
void BattleEffect_SetObjectUpdateMode(void *object, s32 mode);
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action);
void MessageArgument_PushFar(s32, s32);
void FieldMessage_ShowFar(s32 message, s32 flags);
void BattleEffect_StopMainActor(void);

void Func_080dc6d8(void) {
  struct FinishWork *work = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object = work->object;
  s32 quantity = work->quantity;
  s32 zero;
  s8 sound;

  Actor_SetMode(object, 20);
  // The stores in a once-only block keep them all ahead of the sound's read;
  // as plain statements, 8 of 109 instructions differ in tla-en.
  do {
    object->targetX = object->x;
    object->velocityX = 0;
    object->targetY = object->y;
    object->velocityY = 0;
    object->targetZ = object->z;
    object->velocityZ = 0;
  } while (0);
  sound = work->sound;
  /* FAKEMATCH(barrier): the zero set through an empty asm after the sound's
   * read, so its copy to r8 comes after that read; as zero = 0, 4 of 109
   * instructions differ in tla-en. */
  asm("" : "=r"(zero) : "0"(0));
  if (sound != 0) {
    Sound_PlayFar(212);
    BattleEffect_SetObjectUpdateMode(object, 1);
  }
  if (work->showQuantity != 0) {
    work->messageShown = 1;
    Actor_SetOthersPaused(object, 1, 0);
    MessageArgument_PushFar(quantity, 4);
    FieldMessage_ShowFar((s32)&MsgBlank2, work->messageRow);
    Actor_SetOthersPaused(object, 0, 16);
    work->messageShown = zero;
  }
  if (GameFlag_Test(320)) {
    if (work->sound != 0)
      BattleEffect_SetObjectUpdateMode(object, 2);
    Actor_SetMode(object, 21);
  } else {
    BattleEffect_StopMainActor();
  }
  event[0xcb7] = 1;
}
