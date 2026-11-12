#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

#include "io_reg.h"
#include "event_inline.h"

void Func_0200191c(void);
void Func_02001914(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001abc(s32 value);

void Func_02000044(void) {
  volatile u16 *ime = &REG_IME;
  u32 saved = *ime;

  *ime = (u16)(u32)ime;
  Func_0200191c();
  Func_02001914();
  *ime = saved;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "serial.h"

struct LinkPeer {
  s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

s32 Func_0200006c(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & 3) == 3) {
    id = (REG_SIOCNT << 26) >> 30;
    GameFlag_SetBit(0x303);
  } else {
    GameFlag_ClearBit(0x303);
  }
  /* FAKEMATCH(dead): a once-only loop, which places the test after the body;
   * as a plain if with no break, the test comes first, 43 of 67 instructions
   * differ in tla-en. */
  while (id >= 0 && GameFlag_Test(0x303)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (id != 0)
      GameFlag_SetBit(0x302);
    else
      GameFlag_ClearBit(0x302);
    {
      struct LinkPeer *row = &gLinkPeerSignatures[GameFlag_Test(0x302) ^ 1];

      if (row->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}
