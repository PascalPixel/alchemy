//--------------------------------------------------------------------------
// classify_entry_kind.c
// バトル：わざの対象選びの種類
//--------------------------------------------------------------------------

#include "types.h"
#include "battle_system.h"
#include "scene.h"

//--------------------------------------------------------------------------
// わざの効果から、対象選びで使う種類を返す
// 0 その他、1 HPかいふく、2 PPかいふく、3 毒がきえる、4 状態異常をなおす、
// 5 ふっき、6 状態異常をすべてなおす
// act : わざのデータ
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->targetFlag & 15;
  if (type == 1) // HPかいふく
    kind = 1;
  if (type == 11) // PPかいふく
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // 毒がきえる
    kind = 3;
  if (effect == 4) // 状態異常をなおす
    kind = 4;
  if (effect == 64) // 状態異常をすべてなおす
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // ふっき
    kind = 5;
  return kind;
}
