#include "unit/system/save/savedata.h"
#include "unit/system/scheduler/mathtbl.h"
#include "unit/system/scheduler/numtext.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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

s32 Runtime_GetLowTableAddress(void)
{
	return (s32)System_BasicColorPalette;
}

void Text_FormatHexToWork(u32 value)
{
	u8		 *buffer = gNumberTextBuffer;
	const u8 *digits = Text_HexDigits;
	s32		  index = 7;

	do
	{
		buffer[index] = digits[value & 0xF];
		value >>= 4;
		index--;
	} while (index >= 0);

	{
		u8 *terminator = gNumberTextBuffer;
		terminator[8] = 0;
	}
}

void Text_FormatSignedDecimalToWork(s32 arg0)
{
	u32 *tbl;
	s8	*out;
	s32	 count;
	u32	 word;
	s32	 result;
	s32	 val;
	s8	 sign;

	result = arg0;
	val = result;
	tbl = (u32 *)Text_PowersOfTen;
	sign = 0x20;
	out = gNumberTextBuffer;
	if (val < 0)
	{
		val = -val;
		sign = 0x2D;
	}
	word = *tbl++;
	count = 9;
	if (val < word)
	{
		do
		{
			count -= 1;
			*out++ = 0x20;
			if (count == 0)
				break;
			word = *tbl++;
		} while (val < word);
	}
	*out++ = sign;
	tbl -= 1;
	if (count != 0)
	{
		do
		{
			word = *tbl++;
			result = val / word;
			*out++ = result + 0x30;
			val -= result * word;
			count -= 1;
		} while (count != 0);
	}
	out[0] = val + 0x30;
	out[1] = 0;
}

void Graphics_FillWordStreamWithF000(u32 count)
{
	u16 *dst = gDebugTextCursor;
	u32	 index;

	for (index = 0; index < count; index++)
		*dst++ = 0xf000;
	gDebugTextCursor = dst;
}

#ifdef MODERN
// Camelot's compiler copied the task inline; a modern one calls memcpy,
// and the game links no C library
void *memcpy(void *destination, const void *source, unsigned long size)
{
	u8		 *to = destination;
	const u8 *from = source;

	while (size-- != 0)
		*to++ = *from++;
	return destination;
}
#endif

void Runtime_WriteDebugTextTiles(const u8 *src)
{
	if (gDebugTextEnabled != 0)
	{
		u32	 addr = ((u32)&gDebugTextCursor);
		u32	 c = *src;
		u16 *dst = *(u16 **)addr;
		u32	 cnt = 0;
		src++;

		if (c != 0)
		{
			u32 mask = 0xf000;
			addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
			do
			{
				*dst++ = c | mask;
				if (dst == (u16 *)addr)
					dst = (u16 *)BG_SCREEN_BLOCK(4);
				cnt++;
				if (cnt > 31)
					break;
				c = *src++;
			} while (c != 0);
			addr = ((u32)&gDebugTextCursor);
		}
		*(u16 **)addr = dst;
	}
}
//--------------------------------------------------------------------------
// debug_number.c
// デバッグ：数を右そろえでデバッグ文字にする
//--------------------------------------------------------------------------
// どちらも数を作業域に右づめで作り、その終わりから keta 文字を書く
//--------------------------------------------------------------------------
// 16 進で keta けた（1〜8、範囲外なら 8）
//--------------------------------------------------------------------------
void Text_DrawHexRightAligned(u32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 7U)
	{
		n = 8;
	}
	Text_FormatHexToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

//--------------------------------------------------------------------------
// 符号つきの 10 進で keta けた（1〜10、範囲外なら 10）
//--------------------------------------------------------------------------
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 9U)
	{
		n = 10;
	}
	Text_FormatSignedDecimalToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}

void Bg0_ClearTilemap(void)
{
	volatile u32 fill;

	fill = 0xf000f000;
	Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
	gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
	Io_Write16(0x400, &REG_BG0CNT);
}

void Ui_LoadWindowGraphics(void)
{
	Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), BG_CHAR_BLOCK(0), 0x84000800,
		REG_DMA3);
	Dma_Set(Ui_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0), 0x80000010, REG_DMA3);
	Io_Write16(0, &PLTT_BACKDROP);
	Io_Write16(0x4180, &BG_PLTT_COLOR(15, 4));
	Io_Write16(0x3960, &BG_PLTT_COLOR(15, 5));
	Io_Write16(0x3140, &BG_PLTT_COLOR(15, 6));
	Io_Write16(0x2920, &BG_PLTT_COLOR(15, 7));
	Io_Write16(0x49a0, &BG_PLTT_COLOR(15, 8));
	Io_Write16(0x51c0, &BG_PLTT_COLOR(15, 9));
	Io_Write16(0x59e0, &BG_PLTT_COLOR(15, 10));
	Dma_Set(System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}

void PaletteDma_LoadBlock(void)
{
	Dma_Set(System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}
