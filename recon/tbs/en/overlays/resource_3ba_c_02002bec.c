#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3ba:02002bec):
 *   Local_03000380 = 0x03008380 (thumb)
 *   Main_080001e8 = 0x0200bb90 (thumb)
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

s32 Local_03000380();
void Main_080001e8();

extern u8 Data_00000000[];

struct OamTile {
    u16 attr;
    u16 tile;
};

extern s16 Korosseo_MarkerIndex;
extern s16 Korosseo_MarkerSteps;
extern s16 Korosseo_MarkerStep;
extern s16 Korosseo_MarkerY;
extern s16 Korosseo_MarkerStartY;
extern s16 Korosseo_MarkerEndY;
extern s16 Korosseo_MarkerX;
extern s16 Korosseo_MarkerStartX;
extern s16 Korosseo_MarkerEndX;
extern s16 Korosseo_MarkerBlink;
extern s16 Korosseo_MarkerPriority;
extern s32 Korosseo_MarkerOam[3];

/* NONMATCHING: 316 of 316 bytes, 96 halfword edits (2026-09-24). Colosso
 * marker: interpolates the marker between two points and blinks it through the
 * OAM buffer. Twins: resource_3bb:02002e84 and resource_3bc:0200391c (their own
 * Korosseo_Marker* addresses). Residual: pointer loads and the total/base
 * registers are ordered differently. */
void Func_02002bec(void)
{
    s32 tile;
    s32 total;
    s32 step;
    s32 base;
    s32 *oam;
    s32 diff;
    s32 zero;

    oam = Korosseo_MarkerOam;
    tile = ((struct OamTile *)0x03001b10)[Korosseo_MarkerIndex].tile >> 5;
    total = Korosseo_MarkerSteps;
    if (total != 0) {
        step = ++Korosseo_MarkerStep;
        diff = Korosseo_MarkerEndY - Korosseo_MarkerStartY;
        base = (u16)Korosseo_MarkerStartY;
        Korosseo_MarkerY = base + Local_03000380(step * diff, total);
        diff = Korosseo_MarkerEndX - Korosseo_MarkerStartX;
        base = (u16)Korosseo_MarkerStartX;
        Korosseo_MarkerX = base + Local_03000380(step * diff, total);
        if (step >= total) {
            zero = (u16)(u32)Data_00000000;
            Korosseo_MarkerSteps = zero;
        }
        zero = (u16)(u32)Data_00000000;
            Korosseo_MarkerBlink = zero;
    }
    if (++Korosseo_MarkerBlink <= 13) {
        oam[0] = 0;
        oam[1] = ((Korosseo_MarkerX - 8) | ((Korosseo_MarkerY - 8) << 16)) | 0x40000000 | (Korosseo_MarkerPriority << 28);
        oam[2] = tile | 0x400;
        Main_080001e8(oam, 255);
    } else if (Korosseo_MarkerBlink > 19) {
        zero = (u16)(u32)Data_00000000;
            Korosseo_MarkerBlink = zero;
    }
}
