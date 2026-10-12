#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/system/scheduler/runtime.h"
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
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "btlsys.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "node_chain.h"
#include "game_state.h"
#include "sound_ids.h"
#include "item.h"
#include "fixed_math.h"
#include "far_runtime.h"
#include "lib.h"
#include "fixed_point_position.h"
#include "inn_runtime.h"
#include "event_runtime.h"
#include "btlcalc.h"
#include "fieldobj.h"

s32 EventTable_GetRowLimit(void)
{
	return 35;
}

void EventTable_ApplyRowAbilities(s32 row_no)
{
	s16 *row;
	s16 *entry;
	s32	 value;
	s32	 count;
	s32	 flag;

	flag = row_no + 0x400;
	if (GameFlag_TestFar(flag) == 0)
	{
		GameFlag_SetBitFar(flag);
		count = 0;
		value = Shop_StockTable[row_no][24];
		if (value != 0)
		{
			row = Shop_StockTable[row_no];
			entry = row + 24;
			do
			{
				Item_AdjustCounterFar(value, 1);
				count++;
				if (count > 7)
					break;
				entry++;
				value = *entry;
			} while (value != 0);
		}
	}
}

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
	s16 *src;
	s16 *dst;
	s32	 count;

	count = 0;
	if (Shop_StockTable[row_no][0] != 0)
	{
		dst = output;
		src = Shop_StockTable[row_no];
		do
		{
			/* FAKEMATCH: only the copy read is volatile, so its lifetime stays separate from the signed sentinel read. Without volatile: differ 41; indexed and while forms: 34 to 54. */
			*dst = *(volatile s16 *)src;
			count++;
			src++;
			dst++;
			if (count > 23)
				break;
		} while (*src != 0);
	}
	output[count] = 0;
	return count;
}

s32 EventTable_GetRowType(s32 index)
{
	return Shop_StockTable[index][32];
}
