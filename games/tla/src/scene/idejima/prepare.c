#include "types.h"
#include "field_scene.h"
#include "callback_scheduler.h"
#include "party_state.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void Func_0200153c(void);
void Func_02001558(void);
void Func_020015a0(void);
void Func_020016d8(void);
void Func_02001ee0(void);
void Func_02002304(void);
void Func_02002fd4(void);
void Func_020031a8(s32 a);

/// The scene's start entry: the two scene callbacks, then by entrance the
/// scene that runs (2 and 4 also set the saved scene and entrance).
s32 Func_02002ec8(void) {
  s32 entrance;

  Func_0200153c();
  Scheduler_AddOrUpdateCallback(Func_02001558, 0xc80);
  Scheduler_AddOrUpdateCallback(Func_020015a0, 0x480);
  {
    /* FAKEMATCH: the party address pinned to r3 for the switch's read.
     * Unpinned, gcse's PRE merges it with case 4's (inserting it on case 2's
     * two edges) and the one pseudo takes r1 (differ 8; a one-pass block around
     * the switch only reorders, permute found nothing below 8). */
    register struct PartyState *party asm("r3") = &gPartyState;

    entrance = party->entrance;
  }
  switch (entrance) {
  case 2:
    if (GameFlag_Test(0x109) == 0)
      Func_020031a8(1);
  case 4: {
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block after the party address: the native loads the
     * scene id before it builds the saved scene's offset; without it the
     * offset's first half comes first (differ 2, also with the id in a local).
     */
    do {
    } while (0);
    party->savedScene = (s32)&SceneId_DailaInteriors2;
    party->savedEntrance = 1;
  }
    Func_02002fd4();
    break;
  case 11:
    Func_020016d8();
    break;
  case 12:
    Func_02001ee0();
    break;
  case 13:
    Func_02002304();
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
