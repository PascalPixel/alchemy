// Draft, differ 2 of 73: while flag 0x8de is set, clears ownerOverride and
// finds the first gParty_PairResolveRules row (eight halfwords) whose return
// scene and entrance (-1 for any, on either side) match, and stores its
// position (x and z in pixels as 16.16, and a third value) in gPartyState's
// savedX, savedZ and unk280. Left: only the literal pool's order (the
// native's gPartyState entry sits before the -1 mask's, the draft's after).
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);

extern const s16 gParty_PairResolveRules[];

void Func_080ca3f4(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (!GameFlag_Test(0x8de))
    return;
  gPartyState.ownerOverride = 0;
  while (rule[0] != -1) {
    if (rule[2] == scene &&
        (rule[3] == -1 || entrance == -1 || rule[3] == entrance)) {
      gPartyState.savedX = rule[4] << 16;
      gPartyState.savedZ = rule[5] << 16;
      gPartyState.unk280 = rule[6];
      return;
    }
    rule += 8;
  }
}
