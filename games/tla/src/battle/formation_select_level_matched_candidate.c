#include "types.h"
#include "battle_party.h"
#include "battle_unit.h"
#include "mem.h"
#include "system.h"

/// One kept formation: its record and its average level, -1 while empty.
struct FormationCandidate {
  s16 recordId;
  s32 score : 16;
};

/// The enemy flags (0x600 + id) cleared before every search.
extern const u16 Data_08130c5c[];

struct BattleUnit *Unit_Get(s32 unitId);
u8 GameFlag_GetByte(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Func_08127588(s32 formation);

/// Keeps up to thirty-two eligible formations near the party's adjusted
/// average level, chooses one randomly and returns its level adjustment
/// through outMargin. It first sets flags from others, clears the enemy flags
/// of Data_08130c5c, sets flags 71, 230 and 117, and searches 660 formations.
// As The Broken Seal's function of the same name, which has no flag setup.
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin) {
  s32 matchCount = 0;
  struct FormationCandidate *pool = Heap_AllocEwram(128);
  u16 party[8];
  s32 targetLevel = 0;
  s32 unitCount = BattleParty_PrepareActiveOwners(party);
  s32 candidateOrUnitIndex;
  s32 poolIndex;
  s32 result;

  if (unitCount > 0) {
    u16 *unitCursor = party;

    candidateOrUnitIndex = unitCount;
    do {
      u8 level = Unit_Get(*unitCursor)->level;

      candidateOrUnitIndex--;
      unitCursor++;
      targetLevel += level;
    } while (candidateOrUnitIndex != 0);
  }

  targetLevel = targetLevel / unitCount;
  targetLevel += (s8)GameFlag_GetByte(0x3f8);
  if (targetLevel <= 0)
    targetLevel = 1;
  if (targetLevel > 99)
    targetLevel = 99;

  for (candidateOrUnitIndex = 0; candidateOrUnitIndex <= 31;
       candidateOrUnitIndex++)
    pool[candidateOrUnitIndex].score = -1;

  if (GameFlag_Test(0x65b))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65c))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65d))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x65e))
    GameFlag_SetBit(0x65f);
  if (GameFlag_Test(0x66b))
    GameFlag_SetBit(0x66c);
  if (GameFlag_Test(0x668))
    GameFlag_SetBit(0x66a);
  if (GameFlag_Test(0x669))
    GameFlag_SetBit(0x66a);
  GameFlag_ClearBit(0x771);
  if (GameFlag_Test(0x778))
    GameFlag_SetBit(0x771);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex <= 86;
       candidateOrUnitIndex++)
    GameFlag_ClearBit(Data_08130c5c[candidateOrUnitIndex] + 0x600);
  GameFlag_SetBit(71);
  GameFlag_SetBit(230);
  GameFlag_SetBit(117);

  for (candidateOrUnitIndex = 0; (u32)candidateOrUnitIndex < 660;
       candidateOrUnitIndex++) {
    s32 score = Func_08127588(candidateOrUnitIndex);

    if (score >= 0 && score <= targetLevel + 3) {
      s32 lowestIndex = -1;
      s32 lowestLevel;

      for (lowestLevel = 999, poolIndex = 0; poolIndex <= 31; poolIndex++) {
        if (pool[poolIndex].score < lowestLevel) {
          lowestLevel = pool[poolIndex].score;
          lowestIndex = poolIndex;
        }
      }

      if (lowestIndex >= 0) {
        /* FAKEMATCH(one-pass): the candidate's two stores in a one-pass block,
         * which keeps the count's 1 after them; as the plain stores, 2 of 238
         * instructions differ in tla-en. */
        do {
          pool[lowestIndex].score = score;
          pool[lowestIndex].recordId = candidateOrUnitIndex;
        } while (0);
        matchCount++;
      }
    }
  }

  if (matchCount > 32)
    matchCount = 32;

  if (matchCount != 0) {
    struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

    result = chosen->recordId;
    *outMargin = targetLevel - chosen->score;
  } else {
    *outMargin = matchCount;
    result = 1;
  }

  Heap_Release(pool);
  return result;
}
