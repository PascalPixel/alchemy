#include "types.h"
#include "ram_buffer.h"

struct ChampaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

s32 ObjectTable_ReadActiveValue(s32 id);
s32 BattleEffect_GetResourceId(s32 object);
void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
void Func_02002a2c(s32 object, s32 a, s32 b, s32 c);
s32 Message_AllTyped(void);
void wait(s32 frames);

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
      wait(frames);
  }
  /* FAKEMATCH(volatile): the counter's second store and the second x both
   * volatile, so x is loaded after the store; as work->message++ and s32
   * secondX, 4 of 79 instructions differ in tla-en. */
  Message_OpenWindow((*(volatile s16 *)&work->message)++, secondX, secondY,
                     BattleEffect_GetResourceId(second) << 16);
  Func_02002a2c(second, 0, secondA, secondB);
  while (!Message_AllTyped())
    wait(1);
  wait(1);
}
