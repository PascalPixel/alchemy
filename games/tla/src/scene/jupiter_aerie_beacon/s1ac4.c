#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "party_state.h"
#include "owner_state.h"
s32 Func_020059c4();
s32 Object_GetById();
s32 Func_020059dc();
s32 Func_020059f4();
s32 FieldEvent_WaitFrames();
s32 Func_020059d4();
s32 Func_020059cc();
s32 Object_SetModeById();

void Func_02001ac4(s32 a0, s32 a1) {
  Func_020059c4();
  Call2(Func_020059dc, Value1(Object_GetById, a0), 2);
  Call1(Func_020059f4, 0xc9);
  Call1(FieldEvent_WaitFrames, 0x3c);
  Call2(Func_020059dc, Value1(Object_GetById, a0), 0);
  Func_020059d4();
  Func_020059cc();
  Call2(Object_SetModeById, a1, 1);
}

// Restores each active member's HP to its maximum and recomputes both
// gauges, as ☀️'s Party_RestoreHpPp does with HP and PP; with
// clearStatus 1 it also clears poison and haunt. Nested inside
// Func_02001bc4, which calls it with its frame in r9.
void Func_02001b04(s32 clearStatus) {
  /* FAKEMATCH: a nested function's static chain spill. Func_02001bc4 passes its
   * frame base in r9, and function.c stores it to the first frame slot before
   * the parameters are copied; read through the two pins, r9 is stored before
   * r0 is copied, as the native does (a plain clearStatus copies r0 between the
   * chain's load and its store: differ 2). The pins go when the parent is
   * adopted with it. */
  register s32 incoming asm("r0");
  register u32 chain asm("r9");
  volatile u32 frame;
  s32 clear;
  s32 count;
  s32 n;
  struct BattleUnit *member;
  s16 maxHp;

  frame = chain;
  clear = incoming;
  count = Party_CountActiveOwners();
  for (n = 0; n < count; n++) {
    {
      struct PartyState *party = &gPartyState;

      /* FAKEMATCH: the barrier loads the party state before the owner list's
       * offset, as in the main image's Func_080ae16c (differ 2 without). */
      asm volatile("");
      member = UnitGet(party->activeOwners[n]);
    }
    maxHp = member->hp = member->maxHp;
    member->hpGauge = Vitals_Clamp((maxHp << 14) / maxHp);
    if (member->hpGauge == 0 && member->hp != 0)
      member->hpGauge = 1;
    member->ppGauge = Vitals_Clamp((member->pp << 14) / member->maxPp);
    if (member->ppGauge == 0 && member->pp != 0)
      member->ppGauge = 1;
    if (clear == 1) {
      member->poison = 0;
      member->haunt = 0;
    }
  }
}
