/* Near miss, differ 4 of 170 (two swapped pairs at the top): The Broken Seal's
 * PaletteGlow_Update on this game's names. The native loads the wave table's address before
 * the step is shifted (ldr r2 before lsls r5, r5, #18) and reads the first wave byte before
 * the brightness offset is started (ldrb r3 before subs r6, #7); here both come one step
 * later. What the bytes show against the twin: each colour passes through a 32-bit
 * temporary inside a one-pass block (the result is widened, lsls r0, #16; lsrs r0, #16, and
 * stored before the next call's constants are started): written directly it is differ 76,
 * with a u32 temporary and no block differ 48, with a u16 temporary the widening is signed
 * (differ 54). Measured for the two pairs left: the offset computed after the first read is
 * differ 87, assigned inside the first sum differ 55, a wave temporary differ 6. */
#include "types.h"
#include "io_reg.h"

extern const u8 gPaletteGlow_WaveTable[];

u16 Func_0803f974(s16 *rgb, s32 scale_r, s32 scale_g, s32 scale_b);

#define GLOW_COLOR_SET(index, value)             \
	do                                         \
	{                                          \
		u32 color_ = (value);                  \
		BG_PLTT_COLOR(15, (index)) = color_;   \
	} while (0)

void PaletteGlow_Update(s32 phase, s32 brightness)
{
	s16 rgb[3];
	s16 step;
	s16 red;
	s16 green;
	s16 blue;
	s16 offset;

	step = ((phase + 12) % 24) * 4;
	offset = brightness - 7;
	red = gPaletteGlow_WaveTable[(step % 96)] + offset;
	green = gPaletteGlow_WaveTable[(step + 32) % 96] + offset;
	blue = gPaletteGlow_WaveTable[(step + 64) % 96] + offset;
	if (red < 0)
		red = 0;
	if (red > 31)
		red = 31;
	if (green < 0)
		green = 0;
	if (green > 31)
		green = 31;
	if (blue < 0)
		blue = 0;
	if (blue > 31)
		blue = 31;
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	GLOW_COLOR_SET(4, Func_0803f974(rgb, 0xeeee, 0xcccc, 0x11110));
	GLOW_COLOR_SET(5, Func_0803f974(rgb, 0xd555, 0xbbbb, 0xeeee));
	GLOW_COLOR_SET(6, Func_0803f974(rgb, 0xbbbb, 0xaaaa, 0xcccc));
	GLOW_COLOR_SET(7, Func_0803f974(rgb, 0xa221, 0x9999, 0xaaaa));
	GLOW_COLOR_SET(8, Func_0803f974(rgb, 0x10888, 0xdddd, 0x13333));
	GLOW_COLOR_SET(9, Func_0803f974(rgb, 0x12221, 0xeeee, 0x15555));
	GLOW_COLOR_SET(10, Func_0803f974(rgb, 0x13bbb, 0x10000, 0x17777));
}
