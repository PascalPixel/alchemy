/* NONMATCHING: 316 bytes, candidate 296, 155 differing halfwords, 123
 * halfword edits (2026-09-25). KorosseoKabe_Func02002e84, meant for
 * FIELD/KOROSSEO_KABE/F_02E84.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: 155 differing halfwords. Hand-write of lerp+OAM
 * ticker. WALL: copy_versus_rematerialise — whole-function constant
 * sharing after CSE/alloc; pool zeros and VRAM index shape tried. */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x02002f44: 0x00000000  Value_00000000
 *   0x02002f48: 0x0200cb46
 *   0x02002f4c: 0x03001b10
 *   0x02002f50: 0x0200cc2c
 *   0x02002f54: 0x0200cc50
 *   0x02002f58: 0x0200cbf0
 *   0x02002f5c: 0x0200cc94
 *   0x02002f60: 0x0200cc44
 *   0x02002f64: 0x0200cc00
 *   0x02002f68: 0x0200cc20
 *   0x02002f6c: 0x0200cca0
 *   0x02002f70: 0x0200cc5c
 *   0x02002f74: 0x0200cc14
 *   0x02002f78: 0x0200cbf8
 *   0x02002fbc: 0x00000000
 */

extern u8 Value_00000000[];
extern s16 Data_0200cb46;
extern u16 Data_03001b10[];
extern s16 Data_0200cc2c;
extern s32 Data_0200cc50[];
extern u16 Data_0200cbf0;
extern s16 Data_0200cc94;
extern s16 Data_0200cc44;
extern s16 Data_0200cc00;
extern s16 Data_0200cc20;
extern s16 Data_0200cca0;
extern s16 Data_0200cc5c;
extern s16 Data_0200cc14;
extern s16 Data_0200cbf8;

s32 Engine_MathDivide(s32 numerator, s32 denominator);
void Main_080001e8(s32 *buffer, s32 mask);

void KorosseoKabe_Func02002e84(void)
{
    s32 tile;
    s32 duration;
    s32 tick;
    s32 x;
    s32 y;
    s32 frame;

    tile = *(u16 *)((u8 *)Data_03001b10 + (Data_0200cb46 << 2) + 2) >> 5;
    duration = Data_0200cc2c;
    if (duration != 0) {
        Data_0200cbf0 = Data_0200cbf0 + 1;
        tick = (s16)Data_0200cbf0;
        x = (u16)Data_0200cc44;
        Data_0200cc94 = x + Engine_MathDivide(tick * (Data_0200cc00 - Data_0200cc44), duration);
        y = (u16)Data_0200cc5c;
        Data_0200cc20 = y + Engine_MathDivide(tick * (Data_0200cca0 - Data_0200cc5c), duration);
        if (tick >= duration) {
            Data_0200cc2c = (s16)(s32)Value_00000000;
        }
        Data_0200cc14 = (s16)(s32)Value_00000000;
    }
    Data_0200cc14 = Data_0200cc14 + 1;
    frame = (s16)Data_0200cc14;
    if (frame <= 13) {
        Data_0200cc50[0] = 0;
        Data_0200cc50[1] = ((Data_0200cc20 - 8)
            | ((Data_0200cc94 - 8) << 16)
            | 0x40000000
            | (Data_0200cbf8 << 28));
        Data_0200cc50[2] = tile | 0x400;
        Main_080001e8(Data_0200cc50, 255);
    } else if (frame > 19) {
        Data_0200cc14 = (s16)(s32)Value_00000000;
    }
}
