// Tables of field Psynergy after Burst's palette: what Scoop digs up, Hover's glow, the Summon
// tablet's squash and the World Map markers' blend.

#include "item_ids.h"
#include "field/actor_script.h"

// clang-format off

// What fdScoopDigUpItem can find, one of the 16 at random.
const Uint16 ScoopDugItems[16] = {
    ITEM_HERB, ITEM_HERB, ITEM_HERB, ITEM_HERB,
    ITEM_ANTIDOTE, ITEM_ANTIDOTE, ITEM_ANTIDOTE,
    ITEM_SMOKE_BOMB, ITEM_SMOKE_BOMB,
    ITEM_SLEEP_BOMB, ITEM_SLEEP_BOMB,
    ITEM_GAME_TICKET, ITEM_GAME_TICKET, ITEM_GAME_TICKET,
    ITEM_NUT, ITEM_NUT,
};

// Hover's glow level by tick (fdHoverGlowGetColor): 32 steps down from 31, 32 ticks of 0 and 32 steps back up.
const Uint8 HoverGlowLevels[96] = {
    31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
};

// clang-format on

// Sets full size, then squashes and stretches for ever, as the variant motion scripts
// (ActorVariantScripts) do (fdSummonTabletPlayRunes).
const Sint32 SummonTabletSquashScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

// The World Map markers' blend weight in sixteenths, against the picture's 16 less it ((TickCount
// >> 1) & 31, fdMapScreenUpdateMarkers): solid for 32 ticks, then down to half and back over the
// next 32; as The Broken Seal's.
const Uint8 WorldMapMarkerBlendCycle[32] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 15, 15, 14, 12, 10, 9,  9,  8,  9,  9,  10, 12, 14, 14, 15,
};
