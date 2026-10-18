// Draft, differ 45 of 63: while flag 0x8de is set, finds the first
// gParty_PairResolveRules row (eight halfwords) whose scene and entrance
// (-1 for any, on either side) match, and stores its position (x and z in
// pixels, shifted to 16.16, and a third value) at Data_020004b8, which is
// gPartyState's savedX, savedZ and unk280 seen from a symbol of their own.
// Func_080ca3f4 is the same search by the rows' return scene into
// gPartyState. Left: the native holds the second and third addresses
// (Data_020004b8 + 4 and + 8) in lr and r8, hoisted from the loop, which
// costs a saved register; the draft stores at [r1, #4] and [r1, #8]. Tried:
// return instead of break (same); three separate symbols (differ 40, but
// three pool loads where the native has one).
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

extern const s16 gParty_PairResolveRules[];
struct SavedPosition {
  s32 x;
  s32 z;
  s32 unk08;
};

extern struct SavedPosition Data_020004b8;

void Func_080ca368(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (!GameFlag_Test(0x8de))
    return;
  while (rule[0] != -1) {
    if (rule[0] == scene &&
        (rule[1] == -1 || entrance == -1 || rule[1] == entrance)) {
      Data_020004b8.x = rule[4] << 16;
      Data_020004b8.z = rule[5] << 16;
      Data_020004b8.unk08 = rule[6];
      break;
    }
    rule += 8;
  }
}
