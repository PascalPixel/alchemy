#include "types.h"
#include "party_state.h"

extern char SceneId_Ov694Room1;

struct PartyState2c2 {
  u8 unk000[0x2c2];
  u8 value;
};

void FieldEvent_WaitTicks(s32 ticks);
void Func_020000f4(void);
void Func_02000158(void);
void Func_02003448(s32 scene, s32 entrance);
void Func_020034b0(void);

void Func_02001c60(void) {
  Func_020000f4();
  Func_020034b0();
  {
    struct PartyState2c2 *party = (struct PartyState2c2 *)&gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->value = 2;
  }
  Func_02003448((s32)&SceneId_Ov694Room1, 99);
  FieldEvent_WaitTicks(40);
  Func_02000158();
}
