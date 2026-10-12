#include "unit/graphics/font/mark_codes.h"
#include "unit/graphics/window/scales.h"
#include "unit/graphics/window/sideobjt.h"
#include "unit/graphics/window/spranim.h"
#include "unit/graphics/window/textspd.h"
#include "unit/menu/optdata.h"
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window2.h"
#include "unit/graphics/window/window3.h"
#include "unit/graphics/window/window4.h"
#include "unit/graphics/window/window5.h"
#include "unit/graphics/window/window6.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "btlwork.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "btlpty.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "btlsys.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "btltype.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resmenu.h"
#include "btlsmn.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "disptran.h"
#include "frame.h"

s32 Ui_CountIconTableEntries(void)
{
	return UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers;
}

s32 Ui_CountSecondTableEntries(void)
{
	return UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers;
}

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	work->encoded = UiIcon_FramePointerTable[no1];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, 0);
	work->encoded = UiIcon_OverlayPointerTable[no0];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, 1);
	if (flag == 0)
	{
		*slot = Resource_FindFreeEntry();
	}
	*ret = VramBlock_LoadCached(*slot, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
	UiIcon_BuildItemIconTiles(BtlActGet(action)->unknown_04[0], with_base, slot, tile, reuse);
}

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse)
{
	GlyphTransfer *work;
	s32			   slot;

	slot = 0;
	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

	if (glyph >= Ui_CountIconTableEntries())
		glyph = 0;

	if (with_base != 0)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		slot = 1;
	}

	work->encoded = UiIcon_ItemIconPointers[glyph];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, slot);

	if (reuse == 0)
		*src = Resource_FindFreeEntry();

	*dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

extern GlyphTransfer *gGlyphWork;

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers)
{
	s32					   overlay;
	struct ItemDefinition *item;
	GlyphTransfer		  *work;
	s32					   count;

	overlay = 0;
	count = 0;
	item = Item_GetDirectFar(code & 0x1ff);
	work = gGlyphWork;
	if (work == NULL)
		return -1;
	if (layers & ICON_FRAME)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		overlay = 1;
	}
	work->encoded = UiIcon_ItemIconPointers[item->icon];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, overlay);
	if ((layers & ICON_EQUIPPED) && (code & 0x400))
	{
		work->encoded = UiIcon_MarkPointers[1];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if ((layers & ICON_BROKEN) && (code & 0x200))
	{
		work->encoded = UiIcon_MarkPointers[0];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2))
	{
		work->encoded = UiIcon_MarkPointers[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if (layers & ICON_COUNT_ABOVE_ONE)
	{
		count = (code & 0xf800) >> 11;
		count++;
		if (count <= 1)
			count = 0;
	}
	if (layers & ICON_COUNT)
	{
		count = (code & 0xf800) >> 11;
		count++;
	}
	if (count != 0 && count <= 30)
	{
		s32 ones = count % 10;

		work->encoded = UiIcon_DigitPointers[ones];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
		if (count / 10 != 0)
		{
			work->encoded = UiIcon_DigitPointers[count / 10 + 9];
			work->width = 2;
			work->height = 2;
			UiGlyph_DecodeWithHeapRoutines(work, 1);
		}
	}
	return 256;
}

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	ItemIcon_Compose(arg0, arg1);
	VramBlock_LoadCached(arg2, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return 1;
}

s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	Ui_PrepareTransferFromTableEntry(icon);
	VramBlock_LoadCached(slot, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return 1;
}
extern const u8 *RenderResource_PairSourceTable[];

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot)
{
	/* FAKEMATCH: three more names of the one table keep each case's own literal slot; one name folds the switch to a single load. */
	extern const u8 *PairSource1[] __asm__("RenderResource_PairSourceTable"), *PairSource2[] __asm__("RenderResource_PairSourceTable"), *PairSource3[] __asm__("RenderResource_PairSourceTable");
	void *source;
	switch (value)
	{
	case 1:
		source = PairSource1;
		break;
	case 2:
		source = PairSource2;
		break;
	case 3:
		source = PairSource3;
		break;
	case 0:
	default:
		source = RenderResource_PairSourceTable;
		break;
	}
	VramBlock_LoadCached(slot, 32, source);
	return 1;
}

void Ui_PrepareTransferForItem(s32 code)
{
	GlyphTransfer		  *state = gGlyphWork;
	struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

	if (code != 0)
	{
		state->encoded = UiIcon_ItemIconPointers[item->icon];
	}
	else
	{
		state->encoded = UiIcon_ItemIconPointers[0];
	}
	state->width = 2;
	state->height = 2;
	UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
	UiIcon_BuildAbilityIconTiles(BtlActGet(action)->unknown_04[0], with_base, slot, tile, reuse);
}

#define ROUTINE_BLOCK 49

void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse)
{
	GlyphTransfer *work;
	s32			   slot;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	slot = 0;

	if (glyph >= Ui_CountSecondTableEntries())
		glyph = 0;

	if (with_base != 0)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		slot = 1;
	}

	work->encoded = UiIcon_PsynergyIconPointers[glyph];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, slot);

	if (reuse == 0)
		*src = Resource_FindFreeEntry();

	*dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

void Ui_PrepareTransferFromTableEntry(u32 index)
{
	GlyphTransfer *state = gGlyphWork;

	state->encoded = UiIcon_MiscIconPointers[index];
	state->width = 2;
	state->height = 2;
	UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void UiGlyph_LoadEntryWithPalette(
	u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
	GlyphTransfer *work;
	u16			  *table;
	u8			  *entry;
	u32			   index;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	table = Resource_GetTableEntry((s32)&ResourceId_Icons);
	if (icon <= 127)
		index = icon;
	else
		index = icon - 112;
	entry = (u8 *)table + table[index];
	work->encoded = entry + 32;
	work->width = 4;
	work->height = 4;
	UiGlyph_DecodeWithHeapRoutines(work, 0);
	if (reuse == 0)
		*slot = Resource_FindFreeEntry();
	*tile = VramBlock_LoadCached(*slot, 0x200, work->tiles);
	Runtime_ReleaseHeapBlock(17);
	Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0), 0x80000010, REG_DMA3);
}

void UiGlyph_ReservedNoOp(void)
{
}

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined)
{
	RamCode_LoadSlot(ROUTINE_BLOCK, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
	((void (*)(const void *, u8 *))gWorkSlot[ROUTINE_BLOCK])(glyph->encoded, glyph->input);
	Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
	if (outlined)
		RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandMasked, Tile_ExpandMaskedCodeSize);
	else
		RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandOpaque, Tile_ExpandOpaqueCodeSize);
	((void (*)(u8 *, u8 *, u32, u32))gWorkSlot[ROUTINE_BLOCK])(
		glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
	Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}
