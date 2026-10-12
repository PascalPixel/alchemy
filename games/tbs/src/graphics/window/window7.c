#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window2.h"
#include "unit/graphics/window/window3.h"
#include "unit/graphics/window/window5.h"
#include "unit/graphics/window/window6.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/battle/effect/object2.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime9.h"
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

struct RenderOutput *RenderOutput_Create(
	s32 slot, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y)
{
	s32					 x;
	struct RenderOutput *output;
	s32					 y;

	output = RenderOutput_AcquireFree();
	if (output == NULL)
	{
		Resource_ResetEntry(slot);
		return 0;
	}
	x = offset_x + (input->x * 8) + 8;
	y = offset_y + (input->y * 8) + 8;
	x &= 0x1ff;
	y &= 0xff;
	*(u32 *)&output->oam = (x << 16) | y | flags;
	((u32 *)&output->oam)[1] = ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
	output->sentinel = 0xff;
	output->next = 0;
	output->x = x;
	output->y = y;
	output->index = slot;
	output->kind = 1;
	output->active = 1;
	/* FAKEMATCH: RenderOutput_AppendToList is called as s32 (*)(); as void (*)(struct RenderOutputList *, struct RenderOutput *), its prototype, RenderOutput_Create has 9 of 136 bytes different */
	((s32 (*)())RenderOutput_AppendToList)(input, output);
	return output;
}

struct RenderOutput *RenderOutput_CreateFromResource(
	s32 mode, s32 resource, struct RenderInput *input, s32 x, s32 y)
{
	s32 no;

	no = Resource_LoadByMode(mode, resource);
	if (no < 0)
	{
		return NULL;
	}
	return RenderOutput_Create(no, 0x40000000, input, x, y);
}

struct RenderOutput *RenderOutput_CreateLoaded(
	s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y)
{
	s32					 no;
	struct RenderOutput *result;

	no = Resource_FindFreeEntry();
	result = NULL;
	if (no != 0x60)
	{
		UiIcon_CopyResourceToSlot(code, layers, no);
		result = RenderOutput_Create(no, 0x40000000, input, x, y);
	}
	return result;
}

struct RenderOutput *RenderOutput_CreateWithTransform(
	s32 icon, struct RenderInput *input, s32 x, s32 y)
{
	s32					 count;
	s32					 unused;
	struct RenderOutput *result;

	count = Resource_FindFreeEntry();
	if (count == 0x60)
	{
		return NULL;
	}
	Ui_BuildPairedPatternsToSlot(icon, 1, &count, &unused, 1);
	result = RenderOutput_Create(count, 0x40000000, input, x, y);
	result->sentinel = 251;
	return result;
}

struct RenderOutput *RenderOutput_CreateFromTable(
	s32 table_entry, struct RenderInput *input, s32 x, s32 y)
{
	s32					 slot;
	struct RenderOutput *output;

	slot = Resource_FindFreeEntry();
	output = NULL;
	if (slot != 0x60)
	{
		RenderResource_LoadTableEntry(table_entry, 0, slot);
		output = RenderOutput_Create(slot, 0x40000000, input, x, y);
	}
	return output;
}

struct RenderOutput *CreateSideObject(
	s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
	struct UiRenderWork *state = UI_WORK;
	struct RenderOutput *object = 0;
	s32					 first;
	s32					 second;
	s32					 id;
	s32					 slot;

	if (GameFlag_TestFar(32) != 0)
	{
		if (object_kind == 0)
			object_kind = 18;
		if (object_kind == 1)
			object_kind = 19;
	}

	id = Localization_LookupEntryId(object_kind);
	if (id == -1)
		return object;

	if ((u32)side > 1)
	{
		side = 1;
		if (state->resource_ids[1] != 999)
		{
			side = 0;
			if (state->resource_ids[0] != 999)
				return object;
		}
	}

	slot = 14 + side;
	UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
	object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
	if (object != 0)
	{
		object->oam.palette = slot;
		object->kind = 2;
	}

	state->resource_ids[side] = id;
	state->output_ids[side] = first;
	return object;
}

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
	s32					 result;
	s32					 current;
	u32					 character_id;
	struct UiRenderWork *state;

	state = UI_WORK;

	if (GameFlag_TestFar(0x20) != 0)
	{
		if (character == 0)
			character = 0x12;
		if (character == 1)
			character = 0x13;
	}

	character_id = Localization_LookupEntryId(character);
	if (character_id != -1U)
	{
		if (slot > 1U)
		{
			if (state->resource_ids[1] == character_id)
			{
				slot = 1;
			}
			else if (state->resource_ids[0] == character_id)
			{
				slot = 0;
			}
			else
			{
				return;
			}
		}
		current = state->output_ids[slot];
		UiGlyph_LoadEntryWithPalette(character_id, value, &current, &result, slot + 0xe, 1);
	}
}

//--------------------------------------------------------------------------
// runtime_set_byte5_and_reset_halfword_c.c
// 表示：スプライト出力のアニメを切りかえる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// active を mode にして、アニメの段（anim_step）を 0 に戻す
// out : スプライト出力（NULL なら何もしない）
//--------------------------------------------------------------------------
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode)
{
	if (out != NULL)
	{
		out->active = mode;
		out->anim_step = 0;
	}
}

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
	if (arg0 != 0)
		((struct RenderOutput *)arg0)->sentinel = ~arg1;
}

typedef void (*RamRoutine)(void *dst, void *scene);

void UiWindow_FillFromScene(void *dst)
{
	void *scene = gWindowWork[0];

	if (scene == 0)
	{
		volatile u16 fill = 0xe0e0;

		Dma_Set((void *)&fill, dst, 0x810000a0, REG_DMA3);
	}
	else
	{
		RamRoutine code;

		RamCode_Load(code, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
		code(dst, scene);
		MemFree(code);
	}
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height, s32 value)
{
	u32	 row = 0;
	s16 *dst = (s16 *)0x06002000;

	if (row < height)
	{
		do
		{
			u32 column = 0;
			if (column < width)
			{
				do
				{
					column++;
					*dst = value;
					dst++;
				} while (column < width);
			}
			row++;
			dst += 32 - width;
		} while (row < height);
	}
}
