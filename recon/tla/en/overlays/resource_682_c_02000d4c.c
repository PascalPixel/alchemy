// Under -mthumb-slow-loads (2026-10-26): differ 4 of 79; left: the message
// counter's sign extension (lsls, asrs) is scheduled around its store
// otherwise.
// Near miss of Func_02000d4c of overlay 682 (79 instructions, 4 differ): one
// scheduling row. The fifth argument, volatile, is read from the stack before
// r1 and r2 are set for the first Func_02002a2c call, as the native has it (6
// plain, from alchemy permute). At the second message the native stores the
// incremented counter between the two shifts of the old value and loads x and y
// before the second shift; the draft shifts twice first. Tried for that: x and
// y volatile (4, 6), the message in an s32 or s16 local (52, 50), Call4 or an
// inline for the side call (4), five more minutes of permute (nothing better).
// The frames stay in their stack slot through a tagged memory use (differ 46
// without it). Measured: a local with a barrier that holds r1 and r2 loads the
// stack arguments at entry (54).
#include "types.h"
#include "ram_buffer.h"

struct ChampaEventWork {
  u8 unk000[0x1c4];
  u16 message;
};

s32 ObjectTable_ReadActiveValue(s32 id);
s32 BattleEffect_GetResourceId(s32 object);
void UiText_OpenMessageWindow(s16 message, s32 x, s32 y, s32 resource);
void Func_02002a2c(s32 object, s32 a, s32 b, s32 c);
s32 UiWork_IsComplete(void);
void WaitTicks(s32 ticks);

void Func_02000d4c(
    s32 firstId, s32 firstX, s32 firstY, s32 firstA,
    volatile s32 firstB, /* FAKEMATCH: volatile, the fifth argument is read
                            before the side call's other arguments, as the
                            native does (differ 6 plain). */
    s32 secondId, s32 secondX, s32 secondY, s32 secondA, s32 secondB,
    s32 frames) {
  struct ChampaEventWork *work = Ram_HeapSlots->eventWork;
  s32 first = ObjectTable_ReadActiveValue(firstId);
  s32 second = ObjectTable_ReadActiveValue(secondId);

  UiText_OpenMessageWindow(work->message++, firstX, firstY,
                           BattleEffect_GetResourceId(first) << 16);
  Func_02002a2c(first, 0, firstA, firstB);
  /* FAKEMATCH: naming the frames as memory keeps them in their stack slot, read
   * at each use as the native does; plain, they take r7 and the event work
   * moves to r11 (differ 46). */
  __asm__("" : : "m"(frames));
  if (frames != 0) {
    while (!UiWork_IsComplete())
      WaitTicks(frames);
  }
  UiText_OpenMessageWindow(work->message++, secondX, secondY,
                           BattleEffect_GetResourceId(second) << 16);
  Func_02002a2c(second, 0, secondA, secondB);
  while (!UiWork_IsComplete())
    WaitTicks(1);
  WaitTicks(1);
}
