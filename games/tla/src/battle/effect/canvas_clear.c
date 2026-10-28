#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[0] + 0x78dc, 8);
}

void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  u8 *base;
  s32 i;
  s32 index;

  base = Ram_HeapSlots->battleFxWork[0];
  i = 0;
  do {
    if (base[0x78dc + i] != 0) {
      if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
        index = i * 2 + 36;
        Func_0814cd48(*(s16 *)(*(u8 **)(base + 0x78f0) + index), 0, -1, -1, 0);
      }
    }
    i += 1;
  } while (i != 8);
}
