#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window2.h"
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
#include "unit/battle/effect/object11.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim11.h"
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

extern const u8 *RenderResource_PairSourceTable[];

void *Text_FormatPlayTime(s32 value, u8 *out)
{
	u8	buf[64];
	u32 time;
	u32 minutes;
	s32 seconds;
	u8 *s;
	u8 *p;

	time = (u32)value / 0xe10;
	if (time > 0xea5f)
		time = 0xea5f;

	minutes = time / 60;
	seconds = time % 60;

	s = UiText_FormatNumber(buf, minutes, 3);
	*out = *s;
	s++;
	p = out + 1;
	*p = *s;
	p++;
	*p = s[1];
	seconds += 100;
	p++;
	*p = ':';

	s = UiText_FormatNumber(buf, seconds, 2);
	p++;
	*p = s[0];
	p++;
	*p = s[1];
	p[1] = 0;

	return out;
}

u32 SaveState_FindFreeSummarySlot(void)
{
	u32					i;
	struct SaveSummary *p;

	p = gSaveWorkspace->summary;
	for (i = 0; i < 3; i++, p++)
	{
		if (p->level == 0)
			return i;
	}
	return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
	s32					i;
	s32					cnt;
	struct SaveSummary *summary;

	if (SaveState_InitializeWorkspace() != 0)
	{
		cnt = -9;
	}
	else
	{
		cnt = SaveState_LoadSummaryRecords();
		if (flag != 0)
		{
			summary = gSaveWorkspace->summary;
			for (i = 0; i < 3; i++)
			{
				if ((s8)summary[i].send_flag != 0)
					cnt--;
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
	/* FAKEMATCH: the signed cursor from the party tail preserves the literal base changed by direct flag fields. */
	s32 err;
	s32 cnt;
	s32 ret;

	err = SaveState_InitializeWorkspace();
	cnt = 0;
	ret = -9;
	if (err == 0)
	{
		s32 i;
		s8 *flags;

		ret = SaveState_LoadSummaryRecords();
		flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
		gTitleSendOptionEnabled = 0;
		(*(s16 *)&gTitleExtraOptionEnabled) = 0;
		for (i = 0; i < 3; i++)
		{
			if (flags[i * sizeof(struct SaveSummary)
					  + ((u32) & ((struct SaveSummary *)0)->send_flag - (u32)
						  & ((struct SaveSummary *)0)->party[4])]
				!= 0)
			{
				gTitleSendOptionEnabled = 1;
				cnt++;
			}
			if (flags[i * sizeof(struct SaveSummary)
					  + ((u32) & ((struct SaveSummary *)0)->flag_count - (u32)
						  & ((struct SaveSummary *)0)->party[4])]
				!= 0)
			{
				(*(s16 *)&gTitleExtraOptionEnabled) = 1;
			}
		}

		if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT))
		{
			gTitleSendOptionEnabled = 0;
		}
	}
	SaveState_ReleaseWorkspace();
	if (ret != 0 && cnt == ret)
	{
		return ret + 100;
	}
	return ret;
}

u32 SaveState_BuildSummaryHeader(void)
{
	u16					owners[14];
	struct SaveSummary *summary;
	struct BtlUnit	   *owner;
	u8				   *source;
	u8				   *destination;
	u32				   *word;
	s32					i;
	u32					sum = 0;

	summary = (struct SaveSummary *)(gSaveBuffer - 16);
	/* FAKEMATCH: Runtime_GetBuildStampTimeFar is called as u32 (*)(s32); as u32 (*)(void), its prototype, SaveState_BuildSummaryHeader does not compile */
	gGameState.build_stamp = ((u32 (*)(s32))Runtime_GetBuildStampTimeFar)(0);
	gGameState.play_time = gLoadedStateWord;
	{
		u32 *copy = (u32 *)gSceneState;
		copy[64] = gLoadedStateWord;
	}
	gGameState.saved_options = gOptionMirror;
	owner = UnitGetFar(gGameState.selected_actor);
	destination = summary->name;
	source = owner->name;
	for (i = 11; i >= 0; i--)
		*destination++ = *source++;
	summary->level = owner->level;
	summary->play_time = gGameState.play_time;
	summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
	summary->class_id = owner->class_index;
	summary->coins = gGameState.coins;
	summary->djinn[0] = Party_SumDjinnCountsFar(0);
	summary->djinn[1] = Party_SumDjinnCountsFar(1);
	summary->djinn[2] = Party_SumDjinnCountsFar(2);
	summary->djinn[3] = Party_SumDjinnCountsFar(3);
	Party_ListActiveOwnersFar(owners);
	for (i = 0; i <= 3 && owners[i] != 255; i++)
		summary->party[i] = owners[i];
	summary->party[i] = -1;
	summary->palette_glow = gGameState.palette_glow[0];
	summary->palette_glow_sub = gGameState.palette_glow[1];
	summary->send_flag = gGameState.unknown_20f;
	summary->flag_count = 0;
	for (i = 48; i <= 127; i++)
	{
		if (GameFlag_TestFar(i))
			summary->flag_count++;
	}
	summary->has_flag_20 = GameFlag_TestFar(32) != 0;
	{
		s32	 n;
		u32 *frame = &gGameState.build_stamp;
		u32	 frames;

		n = 242;
		/* FAKEMATCH: the one-pass null arm and duplicate frame read end
         * CSE with the bound constant ahead of the frame load. Folding
         * the arm reverses two rows; the conditional and duplicate read
         * disappear, preserving all 174 instructions in every edition. */
		if (!frame)
		{
			do
			{
				n = 242;
			} while (0);
		}
		else
		{
			n = 242;
			frames = *frame;
		}
		frames = *frame;
		word = (u32 *)GameFlagBytes;
		i = 0;
		n <<= 2;
		summary->frames = frames;
		goto test;
		do
		{
			sum += *word++;
			i++;
		test:;
		} while (i < n);
	}
	summary->checksum = sum;
	return sum;
}

s16 SaveState_WriteCurrentSlotPair(void)
{
	s16 value;
	s16 result;
	s32 found;

	result = 0;
	value = gSaveSlot;
	if (value != -1)
	{
		found = SaveState_InitializeWorkspace();
		if (found != 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
			result = -9;
		}
		else
		{
			SaveState_BuildSummaryHeader();
			{
				void *base = &gSaveBuffer;
				s32	  next;

				found = SaveState_WriteRecord(gSaveSlot, base);
				next = gSaveSlot;
				base = (char *)base + 0x1000;
				found |= SaveState_WriteRecord(next + 3, base);
				if (found != 0)
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
					result = -3;
				}
			}
		}
		SaveState_ReleaseWorkspace();
		value = result;
	}
	return value;
}

s32 SaveState_WriteSlotPair(s32 arg0)
{
	s32 found;
	s16 result = 0;

	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		void *base = &gSaveBuffer;

		found = SaveState_WriteRecord(arg0, base);
		base = (char *)base + 0x1000;
		found |= SaveState_WriteRecord(arg0 + 3, base);
		if (found != 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
			result = -3;
		}
	}
	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_ProcessSelectedSlot(void)
{
	void *buffer;
	s32	  value;
	s32	  result;
	s32	  found;
	s32	  error;

	buffer = Runtime_BumpAllocateAlternatePool(0x1000);
	result = 0;
	value = gSaveSlot;
	if (value != -1)
	{
		found = SaveState_InitializeWorkspace();
		if (found != 0)
		{
			error = 9;
			UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
			/* FAKEMATCH: this eliminated self-assignment ends CSE before
             * the shared negate block. Without it, the unbound error uses
             * r6 for both constants and needs a copy to r7 (differ 6).
             * Scoping, a slot pointer and an inline return leave that
             * allocation; duplicating the negate tail still differs 4. */
			gSaveSlot += 0;
			goto negate;
		}
		else
		{
			u8 *dst;

			found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				result = -2;
			}
			dst = (u8 *)buffer + (s32)gSaveStamp;
			dst -= (s32)gSaveBuffer;
			Iwram_CopyWords(dst, gSaveStamp, 16);
			found = SaveState_WriteRecord(gSaveSlot, buffer);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				error = 3;
			negate:
				result = -error;
			}
		}
		SaveState_ReleaseWorkspace();
		MemFree(buffer);
		value = result;
	}
	return value;
}

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
	s32 result;

	UiText_OpenMessageWindow((s32)MsgOverwriteConfirm, 8, 12, 2);
	while (UiWork_IsComplete() == 0)
	{
		WaitFrames(1);
	}
	if (Menu_RunConfirmSelection(1, 0, 0, 1))
	{
		UiWork_FinalizePendingCore();
	}
	else
	{
		UiWork_FinalizePendingCore();
		AudioCommand_PlayFar(85);
		result = SaveState_ProcessSelectedSlot();
		if (result >= 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 1);
		}
	}
	return result;
}

s32 SaveState_CopySlotPair(void)
{
	u32 found;
	s32 value;
	s32 result;

	result = 0;
	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(0, 2);
		if (value == -1)
		{
			result = value;
		}
		else
		{
			void *lower = &gSaveBuffer;
			void *upper;

			found = SaveState_ReadRecordPayload(value, lower);
			upper = (u8 *)lower + 0x1000;
			found |= SaveState_ReadRecordPayload(value + 3, upper);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
				result = -2;
			}
			else
			{
				value = SaveState_FindFreeSummarySlot();
				if (value == 999)
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
					result = -5;
				}
				else
				{
					found = SaveState_WriteRecord(value, lower);
					found |= SaveState_WriteRecord(value + 3, upper);
					if (found != 0)
					{
						UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
						result = -3;
					}
					else
					{
						UiText_ShowPositionedMessageAndWait((s32)MsgFileCopied, 1);
					}
				}
			}
		}
	}

	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_DeleteSelectedSlot(void)
{
	s32 found;
	s32 value;
	s32 result = 0;

	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(0, 3);
		if (value == -1)
		{
			result = value;
		}
		else
		{
			UiText_OpenMessageWindow((s32)MsgEraseConfirm, 8, 1, 2);
			while (UiWork_IsComplete() == 0)
			{
				WaitFrames(1);
			}
			if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0)
			{
				UiWork_FinalizePendingCore();
			}
			else
			{
				UiWork_FinalizePendingCore();
				found = SaveState_DeleteRecord(value);
				found |= SaveState_DeleteRecord(value + 3);
				if (found != 0)
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
					result = -4;
				}
				else
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgFileErased, 1);
				}
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return result;
}

//--------------------------------------------------------------------------

// glow_update_sine.c

// パレット：BG パレット 14 の色 8〜11 を sin で明滅させる

//--------------------------------------------------------------------------

#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、4 色を位相をずらした sin で明滅させる
// s = sin / 16384 として RGB555 で 赤 22 + 2s・緑 16 + s・青 20 + s
//--------------------------------------------------------------------------
void PaletteGlow_UpdateSine(void)
{
	s32			 *tick;
	volatile u16 *pal;
	s32			  col;
	s32			  s;
	s32			  i;

	tick = (s32 *)&gFrameTick;
	pal = (volatile u16 *)0x050001D0; // BG パレット 14 の色 8
	i = 0;
	do
	{
		s = Trig_Sin((*tick + i * 8) * 0x300); // 1 色ごとに 8 フレームずらす
		col = s / 16384;
		{
			u32 r = col << 1;
			u32 g = col;

			r += 22;
			g += 16;
			col += 20;
			/* FAKEMATCH: the do-while fixes the shift's place in the schedule. */
			do
			{
				col <<= 10;
			} while (0);
			g <<= 5;
			col |= g;
			col |= r;
		}
		/* FAKEMATCH: likewise for the store. */
		do
		{
			*pal++ = col;
		} while (0);
	} while (++i <= 3);
}

#endif

#if EDITION_INTERNATIONAL

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
	Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
	u16 *map = (u16 *)gWindowWork[0];
	s32	 sum;

	sum = x + window->x;
	x = sum + 1;
	sum = y + ((struct UiWindow *)window)->y;
	y = sum + 1;

	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
		width = 30 - x;

	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
		height = 20 - y;

	if (width > 0 && height > 0)
	{
		s32 y_ofs = y << 6;
		s32 x_ofs = x << 1;
		s32 offset = y_ofs + x_ofs;
		do
		{
			s32	 remaining = width;
			u16 *cell = (u16 *)(offset + (s32)map);
			if (remaining != 0)
			{
				do
				{
					remaining--;
					*cell++ = 0xE006;
				} while (remaining != 0);
			}
			height--;
			offset += 64;
		} while (height != 0);
		((struct UiRenderWork *)map)->dirty = 1;
	}
}

#endif

void ObjectPlacement_CreateGroup(
	struct RenderInput *origin, s32 x, s32 y, struct SaveSummary *descriptor)
{
	struct ShopRuntime *state = gMenuWork;
	s32					i;
	s32					duration;

	for (i = 0; i < 4 && descriptor->party[i] != -1; i++)
	{
		struct AnimationObject *object = ResourceObject_CreateFar(
			Party_RemapCharacterIdIfEnabledFar(descriptor->party[i], (s8)descriptor->has_flag_20));

		if (object != 0)
		{
			AnimationObjects_SelectAnimationFar(object, 1);
			((struct FieldSprite *)object)->flags = 0;
			((struct FieldSprite *)object)->priority = 0;
		}

		state->party_member_icons[i] = object;
		state->party_member_x[i] = (x + origin->x + i * 3) * 8 + 0x10;
		state->party_member_y[i] = (y + origin->y) * 8 + 0x10;
		state->party_member_scale[i] = 0x10000;
	}

	duration = 200;
	duration <<= 4;
	Scheduler_AddOrUpdateCallback(Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
	struct ShopRuntime *state = gMenuWork;
	s32					i;

	Scheduler_RemoveCallback(Menu_UpdateFirstObjectRowPositions);
	for (i = 0; i < 4; i++)
	{
		if (state->party_member_icons[i] != 0)
		{
			ResourceObject_ReleaseFar(state->party_member_icons[i]);
			state->party_member_icons[i] = 0;
		}
	}
}

void Menu_UpdateFirstObjectRowPositions(void)
{
	struct ShopRuntime *state = gMenuWork;
	s32					source[2];
	s32					request[4];
	s32					index = 0;
	s32					handle;

	while (1)
	{
		handle = (s32)state->party_member_icons[index];
		if (handle != 0)
		{
			source[0] = state->party_member_scale[index];
			source[1] = state->party_member_scale[index];
			request[0] = state->party_member_x[index] << 16;
			request[1] = 0x01F40000;
			request[2] = (state->party_member_y[index] << 16) + 0x01F40000;
			request[3] = 0;
			Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
		}
		index += 1;
		if (index > 3)
		{
			break;
		}
	}
}

void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  i;

	if (origin != 0)
	{
		for (i = 0; i < 4; i++)
		{
			struct AnimationObject *object =
				(struct AnimationObject *)ResourceObject_CreateFar(Menu_PartySpriteResourceIds[i]);

			if (object != 0)
			{
				AnimationObjects_SelectAnimationFar(object, 2);
				((struct FieldSprite *)object)->flags = 0;
				((struct FieldSprite *)object)->priority = 0;
			}

			state->objects[i] = object;
			state->positions_x[i] = (origin->x + x + i * 3) * 8 + 0x10;
			state->positions_y[i] = (origin->y + y) * 8 + 0x10;
		}

		Scheduler_AddOrUpdateCallback(Menu_UpdateSecondObjectRowPositions, 200 << 4);
	}
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  i;

	Scheduler_RemoveCallback(Menu_UpdateSecondObjectRowPositions);
	for (i = 0; i < 4; i++)
	{
		if (state->objects[i] != 0)
		{
			ResourceObject_ReleaseFar(state->objects[i]);
			state->objects[i] = 0;
		}
	}
}

void Menu_UpdateSecondObjectRowPositions(void)
{
	struct FourObjectMotionState *root = gMenuWork;
	s32							  scale[2];
	s32							  position[4];
	s32							  index;

	index = 0;
	do
	{
		void *object = root->objects[index];

		if (object != 0)
		{
			s32 unit = 0x10000;
			s32 bias = 0x1f40000;

			scale[0] = unit;
			scale[1] = unit;
			position[0] = root->positions_x[index] << 16;
			position[1] = bias;
			position[2] = (root->positions_y[index] << 16) + bias;
			position[3] = 0;
			Object_ApplyProjectedPlacementFar((s32)object, position, scale, 0x4000);
		}
		index++;
	} while (index <= 3);
}

//--------------------------------------------------------------------------
// draw_four_numbers_in_row.c
// セーブ：記録のジンの数を属性ごとに横に並べて書く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct SaveSummary *row)
{
	s32 i;

	if (win != 0)
	{
		for (i = 0; i < 4; i++)
			UiText_DrawNumberAtOffset((s8)row->djinn[i], 2, win, i * 24, 16);
	}
}

//--------------------------------------------------------------------------
// draw_character_summary.c
// セーブ：記録の名前・レベル・クラス・コイン・プレイ時間を書く
//--------------------------------------------------------------------------

// 版ごとの「Lv」と「コイン」の x（ドット）
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

//--------------------------------------------------------------------------
// win : 書き込み先（0 なら何もしない）
// rec : セーブの記録
//--------------------------------------------------------------------------
void StatusMenu_DrawCharacterSummary(s32 win, struct SaveSummary *rec)
{
	u32 buf[4]; // プレイ時間の文字列
	s32 y;

	if (win != 0)
	{
		RenderOutput_PrepareForRedraw((struct UiWindow *)win);
		UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
		UiText_DrawStringAtOffset(rec->name, win, 0, 0);
		y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR)
		UiText_DrawStringAtOffset(StatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
		UiText_DrawStringInWindow((s32)StatusMenu_LevelLetterString, win, 72, 0);
#endif
		UiText_DrawNumberAtOffset(rec->level, 2, win, 80, y);
		UiText_DrawCharacterAtOffset(rec->class_id + (s32)MsgClassName, win, 0, 16);
#if !EDITION_INTERNATIONAL
		y = 32;
		UiText_DrawNumberInWindow(rec->coins, 6, win, 24, y);
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
		UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->play_time, (u8 *)buf), win, 48, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
		UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->play_time, (u8 *)buf), win, 48, 40);
		y = 48;
		UiText_DrawNumberInWindow(rec->coins, 6, win, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
		// フランス語版は 1 枚以下なら単数形
		if (rec->coins > 1)
			UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			UiText_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

#if EDITION_INTERNATIONAL

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
	struct UiWindow		 *list;
	struct SaveWorkspace *save;
	struct UiWindow		 *status;
	struct UiWindow		 *djinn;
	struct UiWindow		 *counts;
	struct SaveSummary	 *summary;
	struct RenderOutput	 *cursor;
	s32					  redraw;
	struct UiRenderWork	 *render;
	u32					  stamp;
	void				 *work;
	volatile u32		  zero;
	s32					  result;
	s32					  i;
	s32					  y;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	save = *(gWorkSlot + HEAP_SLOT_SAVE);
	status = NULL;
	djinn = NULL;
	counts = NULL;
	redraw = 1;
	render = *(gWorkSlot + HEAP_SLOT_WINDOW);
	stamp = Runtime_GetBuildStampTimeFar();
	if (slot < 0)
		slot = 0;
	if (mode == 1)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode == 4)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode == 5)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode != 0)
	{
		for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}

	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
	Scheduler_ScheduleCallbackAAfterFrames();
	list = UiWindow_Create(1, 2, 28, 7, 2);
	summary = save->summary;
	y = 0;
	for (i = 0; i < 3; i++, y += 16, summary++)
	{
		if (summary->level == 0)
		{
			UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
		}
		else if (summary->frames < stamp)
		{
			UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
		}
		else if (summary->play_time != *(u32 *)summary->unknown_38)
		{
			UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
		}
		else if (mode == 5 && (s8)summary->send_flag == 0)
		{
			UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
		}
		else
		{
			UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
			UiText_DrawResource(summary->area + (s32)MsgDebugEntryName, list, 62, y);
			render->dirty = 1;
		}
	}
	UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
	UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
	cursor = RenderResource_CreatePair(mode, list, 72, -24);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			if (save->summary[slot].level != 0)
			{
				PaletteGlow_Update(
					save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
				if (status == NULL)
					status = UiWindow_Create(1, 10, 14, 9, 2);
				StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
				WaitFrames(1);
				if (djinn == NULL)
					djinn = UiWindow_Create(16, 10, 13, 3, 2);
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
				WaitFrames(1);
				if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
						+ (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
					!= 0)
				{
					if (counts == NULL)
						counts = UiWindow_Create(16, 14, 13, 5, 2);
					UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
				}
				else
				{
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					UiWork_Finalize(counts, 2);
					counts = NULL;
				}
			}
			else
			{
				PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
				Menu_ClearSecondObjectRowAndScheduleUpdate();
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				UiWork_Finalize(counts, 2);
				UiWork_Finalize(djinn, 2);
				UiWork_Finalize(status, 2);
				counts = NULL;
				djinn = NULL;
				status = NULL;
			}
			RenderOutput_RedrawSavedRect(list);
			UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
			UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
			UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
		}
		Ui_ApplyTableOffsetToPair(cursor);
		WaitFrames(1);
		if (gKeysRepeat & 0x40)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 2) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeysRepeat & 0x80)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 4) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeyState & 2)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		else if (gKeyState & 1)
		{
			AudioCommand_PlayFar(112);
			result = slot;
			break;
		}
	}
	Menu_ClearSecondObjectRowAndScheduleUpdate();
	Menu_ClearFirstObjectRowAndScheduleUpdate();
	UiWork_Finalize(counts, 2);
	UiWork_Finalize(djinn, 2);
	UiWork_Finalize(status, 2);
	UiWork_Finalize(list, 2);
	Scheduler_ScheduleCallbackA();
	Runtime_ReleaseHeapBlock(55);
	PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	WaitFrames(1);
done:
	return result;
}
#elif defined(TBS_EDITION_JA)

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
	struct UiWindow		 *list;
	struct UiWindow		 *status;
	struct SaveWorkspace *save;
	struct UiWindow		 *djinn;
	struct UiWindow		 *counts;
	struct SaveSummary	 *summary;
	struct RenderOutput	 *cursor;
	s32					  kind;
	s32					  redraw;
	struct UiRenderWork	 *render;
	u32					  stamp;
	void				 *work;
	struct ShopCursor	  marker;
	volatile u32		  zero;
	s32					  result;
	s32					  i;
	s32					  x;
	s32					  y;
	s32					  tile;
	struct RenderOutput	 *anchor;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	save = *(gWorkSlot + HEAP_SLOT_SAVE);
	status = NULL;
	djinn = NULL;
	counts = NULL;
	redraw = 1;
	render = *(gWorkSlot + HEAP_SLOT_WINDOW);
	stamp = Runtime_GetBuildStampTimeFar();
	if (slot < 0)
		slot = 0;
	if (mode != 0)
	{
		for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 1)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag != 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 4)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].flag_count == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 5)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}

	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
	list = UiWindow_Create(1, 2, 28, 7, 2);

	for (i = 0, summary = save->summary; i < 3; i++, summary++)
	{
		if (save->summary[i].level == 0)
		{
			UiText_DrawResource((s32)MsgSaveNoData, list, 10, i * 16);
		}
		else if (save->summary[i].frames < stamp)
		{
			UiText_DrawResource((s32)MsgSaveContinue, list, 10, i * 16);
		}
		else if (save->summary[i].play_time != *(u32 *)save->summary[i].unknown_38)
		{
			UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, i * 16);
		}
		else if (mode == 5 && (s8)save->summary[i].send_flag == 0)
		{
			UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, i * 16);
		}
		else
		{
			y = i * 16;
			UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
			UiText_DrawResource(save->summary[i].area + (s32)MsgDebugEntryName, list, 72, y);
			render->dirty = 1;
		}
	}
	UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
	UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
	tile = Resource_FindFreeEntry();
	if (tile < 96)
	{
		VramBlock_LoadCached(tile, 128, Resource_FixedBlockBTiles);
		x = -12;
		y = slot * 16 + 4;
		anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, x, y);
		anchor->active = 1;
		marker.anchor = anchor;
	}
	kind = 1;
	cursor = RenderResource_CreatePair(mode, list, 72, -24);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			if (save->summary[slot].level != 0)
			{
				PaletteGlow_Update(
					save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
				if (status == NULL)
					status = UiWindow_Create(1, 10, 14, 9, 2);
				StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
				WaitFrames(1);
				if (djinn == NULL)
					djinn = UiWindow_Create(16, 10, 13, 3, 2);
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
				WaitFrames(1);
				if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
						+ (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
					!= 0)
				{
					if (counts == NULL)
						counts = UiWindow_Create(16, 14, 13, 5, 2);
					UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
				}
				else
				{
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					UiWork_Finalize(counts, 2);
					counts = NULL;
				}
			}
			else
			{
				PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
				Menu_ClearSecondObjectRowAndScheduleUpdate();
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				UiWork_Finalize(counts, 2);
				UiWork_Finalize(djinn, 2);
				UiWork_Finalize(status, 2);
				counts = NULL;
				djinn = NULL;
				status = NULL;
			}
			x = list->x * 8 - 4;
			y = (list->y << 3) + slot * 16 + 12;
			Shop_SetCursorFar(&marker, x, y, kind);
			kind = 3;
		}
		ShopCursor_AdvanceFar(&marker);
		Ui_ApplyTableOffsetToPair(cursor);
		WaitFrames(1);
		if (gKeysRepeat & 0x40)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 2) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeysRepeat & 0x80)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 4) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeyState & 2)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		else if (gKeyState & 1)
		{
			AudioCommand_PlayFar(112);
			result = slot;
			break;
		}
	}
	Menu_ClearSecondObjectRowAndScheduleUpdate();
	Menu_ClearFirstObjectRowAndScheduleUpdate();
	UiWork_Finalize(counts, 2);
	UiWork_Finalize(djinn, 2);
	UiWork_Finalize(status, 2);
	UiWork_Finalize(list, 2);
	Runtime_ReleaseHeapBlock(55);
	PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	WaitFrames(1);
done:
	return result;
}
#endif

s32 Save_WriteSelectedSlot(void)
{
	s32					  result;
	s32					  slot;
	s32					  flag;
	struct SaveWorkspace *work;

	result = 0;
	flag = SaveState_InitializeWorkspace();
	if (flag != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		work = gSaveWorkspace;
		slot = SaveMenu_SelectSlot(gSaveSlot, 0);
		if (slot == -1)
		{
			result = slot;
		}
		else
		{
			if (work->summary[slot].level != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgOverwriteConfirm, 13);
				while (UiWork_IsComplete() == 0)
				{
					WaitFrames(1);
				}
				if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0)
				{
					UiWork_FinalizePendingCore();
					goto skip;
				}
				UiWork_FinalizePendingCore();
			}
			gSaveSlot = slot;
			AudioCommand_PlayFar(85);
			UiText_ShowPositionedMessageAndWait((s32)MsgSaving, 13);
			while (UiWork_IsComplete() == 0)
			{
				WaitFrames(1);
			}
			SaveState_BuildSummaryHeader();
			SaveState_CaptureObjectTableFar();
			flag = SaveState_WriteRecord(slot, gSaveBuffer);
			flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
			UiWork_FinalizePendingCore();
			if (flag != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				result = -3;
			}
			else
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 9);
			}
		}
	}
skip:
	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
	s32		   ret = 0;
	s32		   err = SaveState_InitializeWorkspace();

	if (err != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		ret = -9;
	}
	else
	{
		s32 value;

		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(gSaveSlot, arg);
		if (value == -1)
		{
			ret = value;
		}
		else
		{
			void *base = &gSaveBuffer;

			err = SaveState_ReadRecordPayload(value, base);
			base = (char *)base + 0x1000;
			err |= SaveState_ReadRecordPayload(value + 3, base);
			if (err != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
				ret = -2;
			}
			else
			{
				gLoadedStateWord = gGameState.play_time;
				gOptionMirror = gGameState.saved_options;
				gPostLoadCounter = 0;
				gSaveSlot = value;
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return ret;
}

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	VramBlock_LoadCached(slot, 0x80, Resource_FixedBlockBTiles);
	return slot;
}

#if EDITION_INTERNATIONAL
s32 Resource_ReturnTrue(void)
{
	return 1;
}
#else

s32 NameEntry_AcceptsMark(s32 mark, s32 letter)
{
	const u8 *range;

	if (mark < 0xde || mark > 223)
		return 1;
	if (mark == 0xde)
		range = NameEntry_DakutenRanges;
	else
		range = NameEntry_HandakutenRanges;
	for (; *range != 0; range += 2)
	{
		if (letter >= range[0] && letter <= range[1])
			return 1;
	}
	return 0;
}

s32 NameEntry_CountLetters(const u8 *name)
{
	s32 count;

	count = 0;
	for (; *name != 0; name++)
	{
		if ((u8)(*name - 0xde) > 1)
			count++;
	}
	return count;
}
#endif

void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source)
{
	struct UiRenderWork *work = UI_WORK;
	s16					*mirror = (s16 *)work->tilemap;
	s16					*buffer = Runtime_BumpAllocateAlternatePool(0x300);
	s16					*input = buffer;
	u32					 cell;
	s16					*vram;
	s32					 row;

	Resource_DecodeType01(source, buffer);
	cell = window->y * 32 + window->x;
	vram = (s16 *)0x06002000 + cell;
	mirror += cell;

	for (row = 0; row < window->height; row++)
	{
		s32 column;

		for (column = 0; column < window->width; column++)
		{
			s16 value = *input++;

			*vram++ = value;
			*mirror++ = value;
		}
		vram += 32 - window->width;
		mirror += 32 - window->width;
	}
	MemFree(buffer);
}

void UiWindow_SetTileAttributeRect(
	const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
	u8 *base = gWindowWork[0];

	x += window->x + 1;
	y += window->y + 1;
	field <<= 12;
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
	{
		width = 30 - x;
	}
	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
	{
		height = 20 - y;
	}
	if (width > 0 && height > 0)
	{
		y <<= 6;
		x = y + (x << 1);
		do
		{
			u16 *cell = (u16 *)(x + (u32)base);
			s32	 remaining = width;
			while (remaining != 0)
			{
				u32 value = *cell;
				value &= 0xFFFFEFFF;
				value |= field;
				remaining--;
				*cell = value;
				cell++;
			}
			height--;
			x += 64;
		} while (height != 0);
		((struct UiRenderWork *)base)->dirty = 1;
	}
}

void Resource_CopyFixedBlockB(s32 arg0)
{
	VramBlock_LoadCached(arg0, 0x80, Resource_FixedBlockBTiles);
}

void Resource_CopyFixedBlockA(s32 arg0)
{
	VramBlock_LoadCached(arg0, 0x80, Resource_FixedBlockATiles);
}

#if EDITION_INTERNATIONAL
s32 UiText_SetRenderString(const u8 *str)
{
	struct UiRenderWork *work;
	u16					*dst;
	s32					 count;
	u32					 width;
	u32					 height;

	work = UI_WORK;
	count = 0;
	if (*str != 0)
	{
		dst = work->entries;
		do
		{
			*dst = *str;
			str++;
			dst++;
			count++;
		} while (*str != 0);
	}
	work->entries[count] = 0;
	UiText_MeasureEntryDimensions(0, &width, &height, 0);
	return width;
}

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
	u8	text[20];
	s32 length = 0;

	if (*input != 0)
	{
		do
		{
			text[length] = *input;
			input++;
			length++;
		} while (*input != 0);
	}

	text[length++] = 8;
	text[length++] = 2;

	while (length <= 6)
	{
		text[length++] = 95;
	}

	text[length++] = 8;
	text[length++] = 15;
	text[length] = 0;
	UiText_DrawString(text, output, 0, -2);
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if defined(TBS_EDITION_DE)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

s32 NameEntry_EditOwnerName(s32 entry)
{
	struct RenderInput	*label;
	s32					 result = 0;
	s32					 original_length = 0;
	s32					 length = 0;
	u8					 text[16];
	u8					*input = text + 1;
	u8					*saved = (u8 *)UnitGetFar(entry);
	u8					*work = gWindowWork[0];
	s32					 text_dirty = 1;
	s32					 cursor_dirty = 1;
	struct RenderInput	*window;
	union MenuCursor	 cursor;
	union MenuCursor	 caret;
	s32					 column, row, slot;
	struct RenderOutput *output;
	u8					*end;
	u16					*pal;

	Ui_LoadWindowGraphics();
	window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 9, 2);
	label = (struct RenderInput *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
	UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
	UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
	UiText_DrawStringInWindow(NameEntry_DeleteKeyLabel, window, 144, 32);
	UiText_DrawStringInWindow(NameEntry_EndKeyLabel, window, 144, 40);
#endif
	UiWindow_DrawDividerLine(window, 18, 0, 18, 7);
	((struct UiRenderWork *)work)->dirty = text_dirty;
	text[0] = result;
	{
		s32 i;

		for (i = 0; i < 14; i++)
		{
			u32 c = saved[i];

			input[i] = c;
			if (c != 0)
			{
				original_length++;
				length++;
			}
		}
	}
	input[14] = 0;
	UiText_DrawPaddedLabel((s32)label, saved);
	slot = Resource_FindFreeEntry();
	column = 18;
	row = 5;
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		cursor.output = output;
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 52);
	}
	slot = Resource_FindFreeEntry();
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		caret.output = output;
		output->sentinel = 255;
		((struct FieldSprite *)output->unknown_10)->priority = 0;
		ShopCursor_SetPositionImmediateFar(&caret, UiText_SetRenderString(input) + 70, 22);
	}
	pal = (u16 *)0x050001c0;
	Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
	pal[4] = 0x6318;
	end = (u8 *)(original_length + (s32)input);
	for (;;)
	{
		s32 width = 1;

		if (column == 18)
		{
			if (row == 4)
				width = 3;
			if (row == 5)
				width = NAME_END_KEY_WIDTH;
		}
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
		if (cursor_dirty)
		{
			cursor_dirty = 0;
			Shop_SetCursorFar(&cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
		}
		if (text_dirty)
		{
			text_dirty = 0;
			Shop_SetCursorFar(&caret, UiText_SetRenderString(input) + 70, 22, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		ShopCursor_MoveTowardTargetFar(&caret);
		{
			u32 frame = (gFrameTick >> 1) & 7;

			output = caret.output;
			((struct FieldSprite *)output->unknown_10)->x = output->x + NameEntry_CaretWave[frame];
			((struct FieldSprite *)output->unknown_10)->y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row--;
			if (column != 18)
			{
				if (row == -1)
					row = 5;
			}
			else
			{
				row = (row != 3);
				row = 5 - row;
			}
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row++;
			if (column != 18)
			{
				if (row == 6)
					row = 0;
			}
			else
			{
				row = (row != 6);
				row += 4;
			}
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column--;
			if (column == -1)
			{
				column = 18;
				if (row < 4 || row > 5)
					column = 16;
			}
			else if (column == 5 || column == 11 || column == 17)
			{
				column--;
			}
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column++;
			if (column == 19)
				column = 0;
			else if (column == 5 || column == 11 || column == 17)
				column++;
			if (column == 18 && (row < 4 || row > 5))
				column = 0;
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column = 18;
			row = 5;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
		remove_character:
			if (length != 0)
			{
				length--;
				*--end = 0;
				RenderOutput_PrepareForRedraw(label);
				UiText_DrawPaddedLabel((s32)label, input);
				text_dirty = 1;
				continue;
			}
			result = -1;
			goto finish;
		}
		if (!(gKeysRepeat & KEY_A))
			continue;
		AudioCommand_PlayFar(112);
		if (column == 18)
		{
			if (row == 5)
			{
				if (length == 0)
				{
					caret.output->active = 13;
					RenderOutput_PrepareForRedraw(label);
					UiText_DrawPaddedLabel((s32)label, saved);
					WaitFrames(10);
				}
				else
				{
					u8 *dst = saved;
					u8 *src = input;
					s32 i = 0;
					u8 *dst_save = dst;
					u8 *src_save = src;

					/* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r2/r1/r0; removing the barrier alone
                     * changes them to r1/r0/r2 (14 instructions differ). */
					if (saved)
						src = NULL;
					if (saved)
						src = src_save;
					if (saved)
						dst = NULL;
					if (saved)
						dst = dst_save;
					while (i < 15)
					{
						*dst++ = *src++;
						i++;
					}
				}
				goto finish;
			}
			if (row == 4)
				goto remove_character;
			continue;
		}
		{
			u32 tx = window->x + column + 1;
			u32 cell;
			u32 character;

			cell = ((window->y + row + 1) << 5) + tx;
			character = work[cell * 2];
			if (length == 5)
				continue;
			*end++ = character;
			*end = 0;
			length++;
			if (length == 5)
			{
				column = 18;
				row = 5;
				cursor_dirty = 1;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawPaddedLabel((s32)label, input);
			text_dirty = 1;
		}
	}
finish:
	UiWork_Finalize((struct UiWindow *)window, 2);
	UiWork_Finalize((struct UiWindow *)label, 2);
	UiWork_FinalizeEntityMatchingLocalizedId(entry);
	WaitFrames(1);
	return result;
}
#else

s32 NameEntry_EditOwnerName(s32 entry)
{
	struct RenderInput	*label;
	s32					 result = 0;
	s32					 keyboard = 0;
	s32					 bytes = 0;
	s32					 length = 0;
	u8					 text[16];
	u8					*input = text + 1;
	u8					*saved = (u8 *)UnitGetFar(entry);
	u8					*work = gWindowWork[0];
	s32					 text_dirty = 1;
	s32					 cursor_dirty = 1;
	struct RenderInput	*window;
	union MenuCursor	 cursor;
	union MenuCursor	 caret;
	s32					 column, row, slot, x;
	struct RenderOutput *output;
	u32					 character;
	u16					*pal;

	Ui_LoadWindowGraphics();
	window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 13, 2);
	label = (struct RenderInput *)UiWindow_Create(8, 3, 8, 3, 2);
	UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
	UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
	UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
	((struct UiRenderWork *)work)->dirty = text_dirty;
	text[0] = result;
	{
		s32 i;

		for (i = 0; i < 14; i++)
		{
			character = saved[i];

			input[i] = character;
			if (character != 0)
			{
				bytes++;
				length++;
				if ((u8)(character - 0xde) <= 1)
					length--;
			}
		}
	}
	input[14] = 0;
	UiText_DrawString(saved, (s32)label, 0, 0);
	column = 18;
	slot = Resource_FindFreeEntry();
	row = 10;
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		cursor.output = output;
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 92);
	}
	slot = Resource_FindFreeEntry();
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		caret.output = output;
		output->sentinel = 255;
		((struct FieldSprite *)output->unknown_10)->priority = 0;
		x = 72 + NameEntry_CountLetters(input) * 10;
		ShopCursor_SetPositionImmediateFar(&caret, x, 22);
	}
	pal = (u16 *)0x050001c0;
	Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
	pal[4] = 0x6318;
	for (;;)
	{
		s32 width = 1;

		if (column == 18)
		{
			if (row == 0)
				width = 4;
			if (row == 8)
				width = 3;
			if (row == 10)
				width = 3;
		}
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
		if (cursor_dirty)
		{
			cursor_dirty = 0;
			Shop_SetCursorFar(&cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
		}
		if (text_dirty)
		{
			text_dirty = 0;
			x = 72 + NameEntry_CountLetters(input) * 10;
			Shop_SetCursorFar(&caret, x, 22, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		ShopCursor_MoveTowardTargetFar(&caret);
		{
			u32 frame = (gFrameTick >> 1) & 7;

			output = caret.output;
			((struct FieldSprite *)output->unknown_10)->x = output->x + NameEntry_CaretWave[frame];
			((struct FieldSprite *)output->unknown_10)->y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row--;
			if (column != 18)
			{
				if (row == -1)
					row = 10;
				else if (row == 5)
					row = 4;
			}
			else
			{
				if (row == -1)
				{
					row = 10;
				}
				else
				{
					row = (row != 7);
					row *= 8;
				}
			}
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row++;
			if (column != 18)
			{
				if (row == 11)
					row = 0;
				if (row == 5)
					row = 6;
			}
			else
			{
				if (row == 1)
					row = 8;
				else if (row == 9)
					row = 10;
				else
					row = 0;
			}
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column--;
			if (column == -1)
			{
				if (row == 0 || row == 8 || row == 10)
					column = 18;
				else
					column = 16;
			}
			else if (column == 5 || column == 11 || column == 17)
			{
				column--;
			}
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column++;
			if (column == 19)
				column = 0;
			else if (column == 5 || column == 11 || column == 17)
				column++;
			if (column == 18)
			{
				if (row == 0 || row == 8 || row == 10)
					column = 18;
				else
					column = 0;
			}
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column = 18;
			row = 10;
		}
		if (gKeyState & KEY_SELECT)
		{
			AudioCommand_PlayFar(111);
		change_keyboard:
			keyboard ^= 1;
			if (keyboard)
				UiWindow_CopyTilemapRegion(window, NameEntry_AlternateKeyboardTilemap);
			else
				UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
			UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
			((struct UiRenderWork *)work)->dirty = 1;
			continue;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
		remove_character:
			if (length != 0)
			{
				length--;
				bytes--;
				character = input[bytes];
				input[bytes] = 0;
				if ((u8)(character - 0xde) <= 1)
				{
					bytes--;
					input[bytes] = 0;
				}
			}
			else
			{
				result = -1;
				goto finish;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawString(input, (s32)label, 0, 0);
			text_dirty = 1;
			continue;
		}
		if (!(gKeysRepeat & KEY_A))
			continue;
		AudioCommand_PlayFar(112);
		if (column == 18)
		{
			if (row == 10)
			{
				if (length == 0)
				{
					caret.output->active = 13;
					RenderOutput_PrepareForRedraw(label);
					UiText_DrawString(saved, (s32)label, 0, 0);
					WaitFrames(10);
				}
				else
				{
					u8 *dst;
					u8 *src;
					s32 i;
					u8 *dst_save;
					u8 *src_save;

					dst = saved;
					src = input;
					i = 0;
					/* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r1/r2/r0; removing the barrier alone
                     * changes them to r2/r0/r1 (14 instructions differ). */
					dst_save = dst;
					src_save = src;
					if (window->height)
						dst = NULL;
					if (window->height)
						dst = dst_save;
					if (window->height)
						src = NULL;
					if (window->height)
						src = src_save;
					while (i < 15)
					{
						*dst++ = *src++;
						i++;
					}
				}
				goto finish;
			}
			if (row == 8)
				goto remove_character;
			if (row == 0)
				goto change_keyboard;
			continue;
		}
		{
			u32 tx = window->x + column + 1;
			u32 cell;
			u8 *dst;

			cell = ((window->y + row + 1) << 5) + tx;
			character = work[cell * 2];
			if (character == 0xfe)
				character = 0xde;
			if (character == 0xff)
				character = 0xdf;
			if (length == 5 && (u8)(character - 0xde) > 1)
				continue;
			dst = input + bytes;
			if (!NameEntry_AcceptsMark(character, input[bytes - 1]))
			{
				AudioCommand_PlayFar(113);
				continue;
			}
			*dst = character;
			bytes++;
			input[bytes] = 0;
			length++;
			if ((u8)(character - 0xde) <= 1)
				length--;
			if (length == 5)
			{
				column = 18;
				row = 10;
				cursor_dirty = 1;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawString(input, (s32)label, 0, 0);
			text_dirty = 1;
		}
	}
finish:
	UiWork_Finalize((struct UiWindow *)window, 2);
	UiWork_Finalize((struct UiWindow *)label, 2);
	UiWork_FinalizeEntityMatchingLocalizedId(entry);
	WaitFrames(1);
	return result;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif defined(TBS_EDITION_ES)
#define TALK_WINDOW_X 9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X 8
#define TALK_WINDOW_WIDTH 16
#endif

s32 PartyTalkMenu_Choose(s32 unused)
{
	/* FAKEMATCH: the shared cursor union preserves pointer-store ordering; an equal-size struct changes stack-address construction in all six editions. */
	struct RenderInput *window;
	union MenuCursor	cursor;
	s32					slot;
	s32					row;
	s32					moved;
	u16				   *palette;

	moved = 1;
	window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice, window, 8, 0);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 1, window, 8, 16);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 2, window, 8, 32);
	slot = Resource_FindFreeEntry();
	row = 0;
	if (slot < 96)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 - 3, window->y * 8 + 9);
	}
	palette = (u16 *)0x050001c0;
	Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
	palette[4] = 0x6318;
	do
	{
		UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 15);
		if (moved)
		{
			moved = 0;
			Shop_SetCursorFar(&cursor, window->x * 8 - 3, (window->y + row * 2) * 8 + 9, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			row--;
			moved = 1;
			if (row == -1)
				row = 2;
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			row++;
			moved = 1;
			if (row == 3)
				row = 0;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
			row = -1;
			goto close;
		}
	} while (!(gKeysRepeat & KEY_A));
	AudioCommand_PlayFar(112);
close:
	UiWork_Finalize(window, 2);
	WaitFrames(1);
	Resource_ResetEntry(slot);
	return row;
}

void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn)
{
	struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
	s32					 name;
	struct UiWindow		*window;
	s32					 sprite[3];
	s32					*entry;
	s32					 palette, handle;
	s32					 id;
	s32					 text;
	u32					 zero;
	s32					*p;

	window = NULL;
	zero = 0;
	name = Djinn_ElementSprites[element & 3];
	entry = sprite;
	window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

		work->dirty = 1;

		id = Localization_LookupEntryId(name);
		UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

		p = entry;
		*p++ = zero;
		*p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
		*p = palette | 0xe000;

		work->result[0] = zero;
		work->result[1] = zero;

		UiWork_PushValueSlot(pc, 1);
		UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

		text = UiText_BuildRenderEntriesMode1(element + (s32)MsgVenusDjinnJoined);

		UiText_QueueRenderEntries(window, text, 36, 2, zero);

		AudioCommand_PlayFar(81);

		do
		{
			Runtime_PushSlotEntry(entry, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle);
	}
}

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
	if (index > 8)
	{
		return 0;
	}
	if (GameFlag_TestFar(32) == 0)
	{
		return Character_SpriteTable[index];
	}
	return Character_ChildSpriteTable[index];
}

void Party_ShowJoinedMessage(s32 member)
{
	struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
	struct UiWindow		*window = NULL;
	s32					 sprite[3];
	s32					*entry = sprite;
	s32					 palette, handle;
	s32					*p;

	window = UiWindow_Create(2, 1, 26, 5, 0);
	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
		work->dirty = 1;
		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)), 0, &handle,
			&palette, 14, 0);
		p = entry;
		*p++ = 0;
		*p++ = 0x8000000c | ((2 * 8 + 4) << 16);
		*p = palette | 0xe000;
		work->result[0] = 0;
		work->result[1] = 0;
		UiWork_PushValueSlot(member, 1);
		UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)MsgJoinedParty), 36, 2, 0);
		AudioCommand_PlayFar(81);
		do
		{
			Runtime_PushSlotEntry(entry, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle);
	}
}

void Party_ShowPairJoinedMessage(s32 first, s32 second)
{
	u8				*work = gWindowWork[0];
	struct UiWindow *window;
	s32				 sprite2[3];
	s32				 sprite1[3];
	s32				*entry1;
	s32				*entry2;
	s32				 handle1, palette, handle2;
	u32				 zero;
	s32				*p;

	// the window starts as NULL, though it is assigned before it is read; the same zero is the style argument below
	window = NULL;
	entry1 = sprite1;
	window = UiWindow_Create(1, 1, 28, 5, 0);
	zero = 0;

	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

		((struct UiRenderWork *)work)->dirty = 1;

		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(first)), 0, &handle1,
			&palette, 14, zero);
		p = entry1;
		*p++ = zero;
		*p++ = 0x800c000c;
		*p = palette | 0xe000;

		entry2 = sprite2;
		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(second)), 0, &handle2,
			&palette, 15, zero);
		p = entry2;
		*p++ = zero;
		*p++ = 0x802c000c;
		*p = palette | 0xf000;

		((struct UiRenderWork *)work)->result[0] = zero;
		((struct UiRenderWork *)work)->result[1] = zero;

		UiWork_PushValueSlot(first, 1);
		UiWork_PushValueSlot(second, 1);

		UiText_QueueRenderEntries(window, UiText_BuildRenderEntriesMode1((s32)MsgPairJoinedParty), 68, 2, zero);

		AudioCommand_PlayFar(81);

		do
		{
			Runtime_PushSlotEntry(entry1, 250);
			Runtime_PushSlotEntry(entry2, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle1);
		Resource_ResetEntry(handle2);
	}
}

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
	void	 *staging_buffer = Runtime_AllocateBlock(14, 0x400);
	const u8 *resource = RenderResource_PairSourceTable[group_index];

	if (resource_index <= 0x5F)
	{
		Resource_DecodeByteLz(resource, staging_buffer);
		VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
		Runtime_ReleaseHeapBlock(14);
	}
}

struct RenderOutput *RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y)
{
	s32					 index;
	struct RenderOutput *first;
	struct RenderOutput *second;

	index = Resource_FindFreeEntry();
	if (index > 95)
		return NULL;

	RenderResource_LoadPair(group, index);
	first = RenderOutput_Create(index, 0x80004000, input, x, y);
	first->sentinel = 0xFD;
	second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
	second->sentinel = 0xFD;
	second->oam.index += 8;
	return first;
}

void Ui_ApplyTableOffsetToPair(struct RenderOutput *obj)
{
	s32 value;

	value = (u8)obj->y + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
	obj->oam.y = value;
	obj = obj->next;
	value = (u8)obj->y + Ui_PairBobOffsets[(*(volatile u32 *)&gFrameTick >> 2) & 7];
	obj->oam.y = value;
}

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
	s32	  size = 1024;
	void *buffer = Runtime_AllocateBlock(14, size);
	u16	 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

	if (value <= 95)
	{
		Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
		if (flag != 0)
			Runtime_RemapBytesByTableFar(buffer, 768);
		VramBlock_LoadCached(value, size, buffer);
		Runtime_ReleaseHeapBlock(14);
	}
}

struct RenderOutput *RenderResource_CreateFrame(
	s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4)
{
	s32					 index;
	struct RenderOutput *entity;

	index = Resource_FindFreeEntry();
	entity = NULL;
	if (index != 0x60)
	{
		RenderResource_LoadFrame(arg0, index, arg1);
		entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
		entity->oam.palette_256 = 1;
		entity->sentinel = 0xfb;
	}
	return entity;
}

void Ui_ApplyTableScaleToObject(struct RenderOutput *obj)
{
	s32					   v = Ui_ObjectPulseScales[(gFrameTick >> 1) & 7];
	struct AffineTransform efx;

	if (v < 0)
		v += 255;
	v >>= 8;

	if (obj != 0)
	{
		efx.scale_x = v;
		efx.scale_y = v;
		efx.angle = 0;
		obj->oam.matrix = AffineMatrix_BuildForEffect(&efx);
		obj->oam.affine_mode = 3;
		obj->oam.x = obj->x + 0xfff0;
		obj->oam.y = (u8)obj->y + 0xf0;
		obj->sentinel = 0xfc;
	}
}
