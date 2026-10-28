// BattleUnit_RefreshPlacement: The Broken Seal's source plus the reserve
// owners; the second fill loop needs its pointer set inside the for
// initialiser.
#include "types.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_action.h"

extern const s8 gBattlePlacement_StepPairs[];
void BattleObject_Make(struct BattleObj *actor, s32 unit, s32 x, s32 y);
s32 BattleParty_PrepareActiveOwners(u16 *ids);
s32 BattleParty_PrepareReserveOwners(u16 *ids);
void Summon_LayoutPositions(u16 *unit_ids, s32 count, s32 *x, s32 *z);
struct BattleObj *BattleObject_Get(s32 id);

void BattleUnit_RefreshPlacement(void) {
  u16 ids[14];
  s32 x[6];
  s32 z[6];
  s32 count;
  s32 i;
  s32 place;
  s32 stand;
  s32 n;
  s32 value;
  u8 *p;
  struct BattleWork *work;

  place = 0;
  stand = 0;
  work = Ram_HeapSlots->battleWork;
  count = BattleParty_PrepareActiveOwners(ids);
  count += BattleParty_PrepareReserveOwners(ids + count);
  value = 0xff;
  for (i = 13; i >= 0; i--)
    work->placement[i] = value;
  for (n = 5, p = &work->placement[13], value = 13; n >= 0; n--, value--)
    *p-- = value;
  for (i = 0; i < count; place++, i++) {
    s32 id = ids[i];

    work->placement[id] = place;
    BattleObject_Make(BattleObject_Get(id), id,
                      gBattlePlacement_StepPairs[place * 2],
                      gBattlePlacement_StepPairs[place * 2 + 1]);
  }
  for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++)
    ids[i] = work->enemyIds[i];
  count = i;
  Summon_LayoutPositions(ids, count, x, z);
  for (i = 0; i < count; i++, stand++) {
    s32 id = work->enemyIds[i];

    if (id != 0xfe)
      BattleObject_Make(BattleObject_Get(id), id, x[stand], z[stand]);
  }
}
