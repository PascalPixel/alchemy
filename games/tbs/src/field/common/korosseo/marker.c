#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "types.h"
#include "vram_block.h"

union MarkerCoordinate {
    s16 signed_value;
    u16 value;
};

extern s16 Korosseo_MarkerSlot;
extern s16 Korosseo_MarkerSteps;
extern s16 Korosseo_MarkerStep;
extern s16 Korosseo_MarkerX;
extern union MarkerCoordinate Korosseo_MarkerStartX;
extern s16 Korosseo_MarkerEndX;
extern s16 Korosseo_MarkerY;
extern union MarkerCoordinate Korosseo_MarkerStartY;
extern s16 Korosseo_MarkerEndY;
extern s16 Korosseo_MarkerBlink;
extern s16 Korosseo_MarkerPriority;

void Korosseo_UpdateMarker(void)
{
    s32 tile;
    s32 total;
    s32 step;
    s32 *oam;
    s16 *steps;
    s32 *dst;
    s32 x;
    s32 y;
    s32 diff;
    s32 base;
    s16 *py;
    union MarkerCoordinate *sy;

    oam = Korosseo_MarkerOam;
    tile = gVramBlockCache[Korosseo_MarkerSlot].offset >> 5;
    steps = &Korosseo_MarkerSteps;
    total = *steps;
    if (total != 0) {
        /* FAKEMATCH: needed to match: the signed read of the raw halfword keeps the step out of the interpolation registers. */
        step = (u16)++Korosseo_MarkerStep;
        py = &Korosseo_MarkerX;
        sy = &Korosseo_MarkerStartX;
        diff = Korosseo_MarkerEndX - sy->signed_value;
        base = sy->value;
        *py = base + diff * (s16)step / total;
        Korosseo_MarkerY = Korosseo_MarkerStartY.value
            + (Korosseo_MarkerEndY - Korosseo_MarkerStartY.signed_value) * (s16)step / total;
        if ((s16)step >= total)
            *steps = 0;
        Korosseo_MarkerBlink = 0;
    }
    if (++Korosseo_MarkerBlink <= 13) {
        /* FAKEMATCH: needed to match: the size bit in a local puts the priority after it. */
        s32 size = 0x40000000;

        x = Korosseo_MarkerX;
        y = Korosseo_MarkerY;
        dst = oam;
        *dst++ = 0;
        *dst++ = (y - 8) | ((x - 8) << 16) | size | (Korosseo_MarkerPriority << 28);
        *dst = tile | 0x400;
        Runtime_PushSlotEntry(oam, 255);
    } else if (Korosseo_MarkerBlink > 19) {
        Korosseo_MarkerBlink = 0;
    }
}
