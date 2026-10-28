#include "types.h"
#include "main_routine_work.h"

s32 GameFlag_Test(s32 flag);

extern const s16 gParty_PairResolveRules[];
extern u8 Data_020004b8[];

/// With flag 0x8de set, finds the first rule of gParty_PairResolveRules for
/// scene and entrance (-1 on either side matches any entrance) and copies its
/// position, x and z in 16.16 and the third value as is, to Data_020004b8.
void Func_080ca368(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (GameFlag_Test(0x8de) == 0)
    return;
  while (rule[0] != -1) {
    if (rule[0] == scene &&
        (rule[1] == -1 || entrance == -1 || rule[1] == entrance)) {
      *(s32 *)(Data_020004b8 + 0) = rule[4] << 16;
      *(s32 *)(Data_020004b8 + 4) = rule[5] << 16;
      *(s32 *)(Data_020004b8 + 8) = rule[6];
      return;
    }
    rule += 8;
  }
}

/// Func_080ca368's twin for the rule's return scene and entrance (its third
/// and fourth values): with flag 0x8de set, clears ownerOverride and copies
/// the rule's position to the party's saved position.
void Func_080ca3f4(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;
  struct MainRoutineWork *party;

  if (GameFlag_Test(0x8de) == 0)
    return;
  party = &gMainRoutineWork;
  /* FAKEMATCH(one-pass block): the empty block loads gMainRoutineWork's address
   * before ownerOverride's offset, as native; as plain statements, 2 of 73
   * instructions differ in tla-en. */
  do {
  } while (0);
  party->ownerOverride = 0;
  while (rule[0] != -1) {
    if (rule[2] == scene &&
        (rule[3] == -1 || entrance == -1 || rule[3] == entrance)) {
      party->savedX = rule[4] << 16;
      party->savedZ = rule[5] << 16;
      party->unk280 = rule[6];
      return;
    }
    rule += 8;
  }
}
