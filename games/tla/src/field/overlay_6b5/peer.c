#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "serial_runtime.h"

struct LinkPeer {
    s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

s32 Func_0200006c(s32 slot)
{
    s32 id = -1;

    if ((gLinkStatus & 3) == 3) {
        id = (REG_SIOCNT << 26) >> 30;
        GameFlag_SetBit(0x303);
    } else {
        GameFlag_ClearBit(0x303);
    }
    /* FAKEMATCH: a loop that runs at most once places the test after the body as the ROM does. */
    while (id >= 0 && GameFlag_Test(0x303)) {
        const s32 *expected = &LinkLobby_SlotValues[slot];

        if (id != 0)
            GameFlag_SetBit(0x302);
        else
            GameFlag_ClearBit(0x302);
        {
            struct LinkPeer *row = &gLinkPeerSignatures[GameFlag_Test(0x302) ^ 1];

            if (row->values[LinkLobby_SlotColumns[slot]] == *expected)
                return 1;
            break;
        }
    }
    return 0;
}
