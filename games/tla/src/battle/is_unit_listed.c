//--------------------------------------------------------------------------
// is_unit_listed.c
// バトル：ユニットが戦闘中の並びにいるかの判定
//--------------------------------------------------------------------------

#include "types.h"
#include "battle_party.h"

//--------------------------------------------------------------------------
// ユニットが自分の側の並びにいれば 1 を返す
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
u32 BattleParty_IsUnitListed(u32 id) {
  u16 list[8];
  s32 side;
  s32 cnt;
  s32 i;

  side = BATTLE_SIDE_PARTY;
  if (id > 7)
    side = BATTLE_SIDE_ENEMIES;
  cnt = BattleParty_ListActorIds(side, list);

  for (i = 0; i < cnt; i++) {
    if (list[i] == id)
      break;
  }

  return i != cnt;
}
