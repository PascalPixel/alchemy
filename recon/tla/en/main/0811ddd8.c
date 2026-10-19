// Draft of Func_0811ddd8: turns a target list into an action plan. 8
// instructions differ (10 before), four swapped pairs in the opening: the
// argument copy and the 0xfff mask, the side load and the counter copy. The
// loop is a guarded do-while over an id pointer whose count is read before
// i steps behind a one-pass block (FAKEMATCH, as Func_08124cc4's), which
// fixes the loop end; five minutes of permute found only a block around the
// whole body (6, not kept).

#include "types.h"
#include "owner_state.h"

struct ActionTargets {
  u8 side;
  s8 count;
  u8 unk02;
  u8 ids[14];
  u8 stateA[14];
  u8 stateB[14];
  u8 stateC[14];
  u8 stateD[14];
  u16 value4a;
  s32 message;
  s32 value50;
  s32 value54;
  u32 cue;
  s32 unk5c;
  s32 extra;
  s32 unk64;
};

struct ActionPlan {
  s32 cue;
  s32 unk04;
  s32 side;
  s32 first;
  s32 one;
  s32 count;
  s32 kind;
  s32 zero;
  s32 unk20;
  u16 ids[14];
};

u32 BattleParty_IsUnitListed(u32 id);

void Func_0811ddd8(struct ActionTargets *targets, struct ActionPlan *plan) {
  s32 count = 0;
  s32 i;
  s32 n;
  u8 *id;

  plan->zero = 0;
  plan->cue = targets->cue & 0xfff;
  plan->kind = (targets->cue & 0x3000) >> 12;
  i = 0;
  plan->side = targets->side;
  if (i < targets->count) {
    id = targets->ids;
    do {
      if (BattleParty_IsUnitListed(*id)) {
        struct BattleUnit *unit = UnitGet(*id);

        if (unit->hp != 0 || (targets->cue & 0x10000)) {
          plan->ids[count] = *id;
          count++;
        }
      }
      n = targets->count;
      /* FAKEMATCH(one-pass block): the count is read before i steps; as a for
       * loop over targets->ids[i], 10 of 78 instructions differ in tla-en. */
      do {
      } while (0);
      i++;
      id++;
    } while (i < n);
  }
  if (count == 0) {
    plan->ids[0] = targets->ids[0];
    count = 1;
  }
  plan->first = targets->ids[0];
  plan->count = count;
  plan->one = 1;
}
