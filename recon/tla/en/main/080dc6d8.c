// Under -mthumb-slow-loads (2026-10-26): differ 4 of 109 in tla-en, instruction
// order only. An empty asm before `object->velocityY = 0;` lowers it to 2; no
// second one closes it.
// Near miss, differ 4 of 109, instruction order only. The one-pass block
// round the six target and velocity stores keeps the y and z velocities
// ahead of the sound byte's load (differ 8 without). Left: the native stores
// the x velocity between the y load and its store (str r2, [r6, #36]) and
// copies the zero to r8 after the sound byte's load; here the copy comes
// first. A zero local set after the block, empty asm barriers between the
// pairs and volatile asm after them measured 4, 10 and 73 (2026-10-18).
// Finishes an effect object's run (heap slot 0xe0 effect work): the
// object takes mode 20 and holds still where it is; with the work's sound
// byte set it plays cue 212 and updates in mode 1. When the quantity byte is
// set, the others act while MsgBlank2 (3522) shows the work's quantity. Flag
// 320 then sends the object to mode 21 (update mode 2 with the sound byte
// set), otherwise the current effect object stops; the event work's byte
// 0xcb7 is set either way.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct FinishWork {
  u8 unk000[16];
  struct ObjectRuntime *object;
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
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excludedObject,
                                    s32 groupMode, s32 action);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);
void EffectRuntime_StopCurrentObject(void);

void Func_080dc6d8(void) {
  struct FinishWork *work = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *object = work->object;
  s32 quantity = work->quantity;

  Object_SetMode(object, 20);
  /* FAKEMATCH: the one-pass block ends the stores before the sound test. */
  do {
    object->targetX = object->x;
    object->velocityX = 0;
    object->targetY = object->y;
    object->velocityY = 0;
    object->targetZ = object->z;
    object->velocityZ = 0;
  } while (0);
  if (work->sound != 0) {
    Audio_PlayCue(212);
    BattleEffect_SetObjectUpdateMode(object, 1);
  }
  if (work->showQuantity != 0) {
    work->messageShown = 1;
    ObjectGroup_SetActionForOthers(object, 1, 0);
    UiText_DrawQuantity(quantity, 4);
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgBlank2, work->messageRow);
    ObjectGroup_SetActionForOthers(object, 0, 16);
    work->messageShown = 0;
  }
  if (GameFlag_Test(320)) {
    if (work->sound != 0)
      BattleEffect_SetObjectUpdateMode(object, 2);
    Object_SetMode(object, 21);
  } else {
    EffectRuntime_StopCurrentObject();
  }
  event[0xcb7] = 1;
}
