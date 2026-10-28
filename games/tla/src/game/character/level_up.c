#include "owner_state.h"
#include "mem.h"
#include "types.h"

struct OwnerLevelState {
  u8 unk000[0x0f];
  u8 level;
  u16 baseHp;
  u16 basePp;
  u8 unk014[4];
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseTurns;
  u8 base20;
  u8 base21;
  u8 unk022[0x102];
  u32 experience;
  u8 character;
  u8 classId;
};

struct OwnerGrowth {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
};

struct LevelUpResult {
  s16 level;
  u16 ability;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
};

struct LevelUpWork {
  s32 classId;
  s32 level;
  struct OwnerGrowth *growth;
  u8 unused0c[0x20];
};

void Heap_Release(void *buffer);
u32 Unit_GetExperienceForLevel(s32 owner, s32 level);
struct OwnerGrowth *Character_GetDefinition(s32 owner);
s32 Unit_RefreshPsynergy(s32 owner);
void Unit_Calculate(s32 owner);
u32 Random16(void);
s32 __divsi3(s32 numerator, s32 denominator);
u32 __udivsi3(u32 numerator, u32 denominator);

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res) {
  struct OwnerLevelState *st;
  struct LevelUpWork *work;
  u32 threshold;
  s16 band;
  s32 diff;
  s32 level;

  st = Unit_Get(owner);
  work = Heap_AllocEwram(sizeof(struct LevelUpWork));
  work->classId = st->classId;
  level = st->level;
  work->level = level;
  res->level = level;
  res->ability = 0xffff;
  res->hp = 0;
  res->pp = 0;
  res->atk = 0;
  res->def = 0;
  res->agi = 0;
  res->luck = 0;
  if (level < 99) {
    st->level++;
    res->level = level + 1;
    threshold = Unit_GetExperienceForLevel(owner, st->level);
    if (threshold != (u32)-1 && st->experience < threshold)
      st->experience = threshold;
    work->growth = Character_GetDefinition(owner);
    if (res->level == 1) {
      res->hp += work->growth->hp[0];
      res->pp += work->growth->pp[0];
      res->atk += work->growth->atk[0];
      res->def += work->growth->def[0];
      res->agi += work->growth->agi[0];
      res->luck += work->growth->luck[0];
    }
    band = __divsi3(res->level, 20);
    if (band < 0)
      band = 0;
    if (band > 4)
      band = 4;
    diff = work->growth->hp[band + 1] - work->growth->hp[band];
    res->hp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->pp[band + 1] - work->growth->pp[band];
    res->pp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->atk[band + 1] - work->growth->atk[band];
    res->atk += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->def[band + 1] - work->growth->def[band];
    res->def += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->agi[band + 1] - work->growth->agi[band];
    res->agi += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    diff = work->growth->luck[band + 1] - work->growth->luck[band];
    res->luck += __udivsi3((Random16() * 20 >> 16) + diff, 20);
    st->baseHp += res->hp;
    st->basePp += res->pp;
    st->baseAttack += res->atk;
    st->baseDefense += res->def;
    st->baseAgility += res->agi;
    st->baseLuck += res->luck;
    st->baseTurns = 1;
    st->base20 = 0;
    st->base21 = 0;
    Unit_RefreshPsynergy(owner);
    Unit_Calculate(owner);
  }
  Heap_Release(work);
  return res;
}
