#include "unit/system/scheduler/mathtbl.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime9.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "input.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "layout_guard.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"

u32 Random16(void)
{
	u32 value = gRandomSeed * 0x41c64e6d + 0x3039;

	gRandomSeed = value;
	return (value << 8) >> 16;
}

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
	*position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
	position++;
	*position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

s32 ArcTan2(s32 x, s32 y)
{
	const u16 *table;
	s32		   value;
	s32		   ratio;
	s32		   result;

	if (x == 0)
	{
		result = 0;
	}
	else if (y == 0)
	{
		result = 0x4000;
	}
	else
	{
		ratio = y;
		if (ratio < 0)
			ratio = -ratio;
		value = x;
		if (value < 0)
			value = -value;

		ratio = (value << 8) / ratio;
		result = 0x4000;
		if (ratio <= 0xFB6A)
		{
			table = &Math_TanTable[127];
			result = 0;

			value = *table;
			table -= 0x40;
			if (ratio > value)
			{
				result = 0x2000;
				table += 0x80;
			}
			value = *table;
			table -= 0x20;
			if (ratio > value)
			{
				result |= 0x1000;
				table += 0x40;
			}
			value = *table;
			table -= 0x10;
			if (ratio > value)
			{
				result |= 0x800;
				table += 0x20;
			}
			value = *table;
			table -= 8;
			if (ratio > value)
			{
				result |= 0x400;
				table += 0x10;
			}
			value = *table;
			table -= 4;
			if (ratio > value)
			{
				result |= 0x200;
				table += 8;
			}
			value = *table;
			table -= 2;
			if (ratio > value)
			{
				result |= 0x100;
				table += 4;
			}
			value = *table;
			table--;
			if (ratio > value)
			{
				result |= 0x80;
				table += 2;
			}
			value = *table;
			if (ratio > value)
				result |= 0x40;
		}
	}

	if (y < 0)
		result = 0x8000 - result;
	if (x < 0)
		result = -result;

	return (u16)result;
}

s32 Math_IntegerSqrt(s32 value)
{
	s32 trial;
	s32 remainder;
	s32 bit;
	s32 result;

	remainder = value;
	result = 0;
	bit = 0xf;
	do
	{
		trial = (result << (bit + 1)) + (1 << (bit * 2));
		if (trial <= remainder)
		{
			result |= 1 << bit;
			remainder -= trial;
		}
		bit -= 1;
	} while (bit >= 0);
	return result;
}

s32 FixedSqrt(s32 value)
{
	return Iwram_Sqrt(value) << 8;
}
