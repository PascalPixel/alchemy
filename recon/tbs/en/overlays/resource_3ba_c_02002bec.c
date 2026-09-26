#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3ba:02002bec):
 *   Engine_MathDivide = 0x0200bb00 (thumb)
 *   Engine_OamSubmitRecord = 0x0200bb90 (thumb)
 *   Korosseo_LinkedZero = 0x00000000 (data)
 *   gOamTiles = 0x03001b10 (data)
 *   Korosseo_MarkerIndex = 0x0200c6a6 (data)
 *   Korosseo_MarkerSteps = 0x0200c78c (data)
 *   Korosseo_MarkerStep = 0x0200c750 (data)
 *   Korosseo_MarkerY = 0x0200c7f4 (data)
 *   Korosseo_MarkerStartY = 0x0200c7a4 (data)
 *   Korosseo_MarkerEndY = 0x0200c760 (data)
 *   Korosseo_MarkerX = 0x0200c780 (data)
 *   Korosseo_MarkerStartX = 0x0200c7bc (data)
 *   Korosseo_MarkerEndX = 0x0200c800 (data)
 *   Korosseo_MarkerBlink = 0x0200c774 (data)
 *   Korosseo_MarkerPriority = 0x0200c758 (data)
 *   Korosseo_MarkerOam = 0x0200c7b0 (data)
 */

s32 Engine_MathDivide(s32 dividend, s32 divisor);
void Engine_OamSubmitRecord(s32 *record, s32 priority);

extern u8 Korosseo_LinkedZero[];

struct OamTile {
    u16 attr;
    u16 tile;
};

union MarkerCoordinate {
    s16 signed_value;
    u16 value;
};

extern struct OamTile gOamTiles[];

extern s16 Korosseo_MarkerIndex;
extern s16 Korosseo_MarkerSteps;
extern s16 Korosseo_MarkerStep;
extern s16 Korosseo_MarkerY;
extern union MarkerCoordinate Korosseo_MarkerStartY;
extern s16 Korosseo_MarkerEndY;
extern s16 Korosseo_MarkerX;
extern union MarkerCoordinate Korosseo_MarkerStartX;
extern s16 Korosseo_MarkerEndX;
extern s16 Korosseo_MarkerBlink;
extern s16 Korosseo_MarkerPriority;
extern s32 Korosseo_MarkerOam[3];

/* NONMATCHING: candidate 312, full reference 316 bytes, 126 differing
 * halfwords / 65 alignment edits (2026-09-26). Owner extent is
 * resource_3ba:[02002bec,02002d28), including both literal pools.
 * Interpolates and blinks the Colosso marker through the OAM queue.
 * Typed table, reused destination pointer, advancing OAM pointer and reused
 * base reduced the earlier 96-edit draft. Remaining: interpolation multiply
 * operand and pointer-load order, priority/zero pool placement, four bytes
 * short. The signed/unsigned union and allocator-guided declaration reorder
 * did not change bytes; a one-pass setup block worsened the score to 66 edits.
 * Complete byte comparison proves only call relocations and local pool
 * addresses differ in resource_3bb:02002e84 and resource_3bc:0200391c.
 * They are not adopted from this draft. */
void Korosseo_UpdateMarker(void)
{
    s32 tile;
    s32 total;
    s32 step;
    s32 base;
    s32 *oam;
    s32 diff;
    s32 zero;
    s16 *steps;
    s16 *pos;
    s32 *dst;
    s32 x;
    s32 y;
    s32 word;
    union MarkerCoordinate *start;
    s16 *end;
    s16 *priority;

    tile = gOamTiles[Korosseo_MarkerIndex].tile >> 5;
    steps = &Korosseo_MarkerSteps;
    total = *steps;
    oam = Korosseo_MarkerOam;
    if (total != 0) {
        step = ++Korosseo_MarkerStep;
        pos = &Korosseo_MarkerY;
        start = &Korosseo_MarkerStartY;
        end = &Korosseo_MarkerEndY;
        diff = *end - start->signed_value;
        base = start->value;
        base += Engine_MathDivide(step * diff, total);
        *pos = base;
        pos = &Korosseo_MarkerX;
        end = &Korosseo_MarkerEndX;
        start = &Korosseo_MarkerStartX;
        diff = *end - start->signed_value;
        base = start->value;
        base += Engine_MathDivide(step * diff, total);
        *pos = base;
        if (step >= total) {
            zero = (u16)(u32)Korosseo_LinkedZero;
            *steps = zero;
        }
        zero = (u16)(u32)Korosseo_LinkedZero;
        Korosseo_MarkerBlink = zero;
    }
    if (++Korosseo_MarkerBlink <= 13) {
        y = Korosseo_MarkerY;
        x = Korosseo_MarkerX;
        dst = oam;
        *dst++ = 0;
        priority = &Korosseo_MarkerPriority;
        word = (x - 8) | ((y - 8) << 16);
        word |= 0x40000000;
        word |= *priority << 28;
        *dst++ = word;
        *dst = tile | 0x400;
        Engine_OamSubmitRecord(oam, 255);
    } else if (Korosseo_MarkerBlink > 19) {
        *(u16 *)&Korosseo_MarkerBlink = 0;
    }
}
