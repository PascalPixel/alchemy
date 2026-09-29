/* Colosso: the marker task. While a move is running it eases the marker
 * from its start toward its end position, one step of the move's length per
 * frame; then it blinks the marker, drawn through the OAM queue for 14 of
 * every 20 frames. The same function sits in each of the three Colosso
 * trial overlays, with its variables in each overlay's own work. */
#include "TYPES.H"

struct VramBlock {
    u16 pad;
    u16 base;
};

/* The marker's start is read both as a signed coordinate and as the raw
 * halfword the position is rebuilt from. */
union MarkerCoordinate {
    s16 signed_value;
    u16 value;
};

extern struct VramBlock gVramBlockCache[];

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
extern s32 Korosseo_MarkerOam[3];

void Runtime_PushSlotEntry(void *record, s32 priority);

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
    tile = gVramBlockCache[Korosseo_MarkerSlot].base >> 5;
    steps = &Korosseo_MarkerSteps;
    total = *steps;
    if (total != 0) {
        /* FAKEMATCH: the step is kept as the raw halfword and read signed,
         * which keeps it out of the interpolation's registers. */
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
        /* FAKEMATCH: the size bit in a local puts the priority after it. */
        s32 size = 0x40000000;

        x = Korosseo_MarkerX;
        y = Korosseo_MarkerY;
        dst = oam;
        *dst++ = 0;
        *dst++ = (y - 8) | ((x - 8) << 16) | size | (Korosseo_MarkerPriority << 28);
        *dst = tile | 0x400;
        Runtime_PushSlotEntry(oam, 255);
    } else if (Korosseo_MarkerBlink > 19) {
        *(u16 *)&Korosseo_MarkerBlink = 0;
    }
}
