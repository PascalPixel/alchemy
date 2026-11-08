#include "battle.h"
#include "serial.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "serial.h"

struct LinkPeer {
  s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

s32 Func_0200006c(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & 3) == 3) {
    id = (REG_SIOCNT << 26) >> 30;
    GameFlag_SetBit(SPLIT_CONSTANT(0x204, 255));
  } else {
    GameFlag_ClearBit(SPLIT_CONSTANT(0x204, 255));
  }
  /* FAKEMATCH: a loop that runs at most once places the test after the body as
   * the ROM does. */
  while (id >= 0 && GameFlag_Test(SPLIT_CONSTANT(0x204, 255))) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (id != 0)
      GameFlag_SetBit(SPLIT_CONSTANT(0x300, 2));
    else
      GameFlag_ClearBit(SPLIT_CONSTANT(0x300, 2));
    {
      struct LinkPeer *row = &gLinkPeerSignatures[GameFlag_Test(SPLIT_CONSTANT(0x300, 2)) ^ 1];

      if (row->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}
