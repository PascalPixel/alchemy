#include "types.h"

struct CanvasJob {
  s32 unk00;
  s32 side;
};

void BattleEffect_StartCanvasLayer(s32 layer);
void Func_0815585c(struct CanvasJob *job, s32 flags, s32 *b, s32 *a);
void BattleEffect_EndCanvasLayer(void);

/// Abilities: Kite (kind 217).
void BattleEffect_RunKite(struct CanvasJob *job) {
  s32 b[3];
  s32 a[3];

  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(job, ((job->side ^ 1) << 4) | 7, b, a);
  BattleEffect_EndCanvasLayer();
}
