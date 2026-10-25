#include "types.h"
#include "ram_buffer.h"
#include "battle_effect_work.h"

struct BattleFxWorkTail {
  u8 unk0000[0x78e4];
  s32 active;
  s32 unk78e8;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);

// Midpoint between the first and last targets.
void Func_0815e22c(struct EffectPosition *position) {
  struct BattleFxWorkTail *work;
  struct EffectPosition first;
  struct EffectPosition last;

  work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  EffectPosition_ProjectBattler(work->effect->actors[0], &first);
  EffectPosition_ProjectBattler(work->effect->actors[work->effect->count - 1],
                                &last);
  position->x = first.x + (last.x - first.x) / 2;
  position->y = first.y;
  position->depth = first.depth;
}
