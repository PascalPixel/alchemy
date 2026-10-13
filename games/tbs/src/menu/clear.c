#include "scene_ids.h"
#include "unit/menu/clear.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evmes.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/game/chara.h"
#include "unit/game/flag.h"
#include "unit/game/party.h"
#include "unit/game/invsave.h"
#include "unit/window/window.h"
#include "unit/window/config.h"
#include "unit/window/wintile.h"
#include "unit/window/output.h"
#include "unit/window/ptystat.h"
#include "unit/window/save.h"
#include "unit/window/btlcmd.h"
#include "unit/window/cmdicon.h"
#include "unit/window/debug.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/basepal.h"
#include "unit/runtime/heap.h"
#include "unit/runtime/serial.h"
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
#include "resource.h"
#include "global_cells.h"
#include "edition.h"
#include "tbs_edition.h"
#include "text_font.h"
#include "types.h"
#include "scene.h"
#include "dma.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "resource_ids.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "field_scene.h"
#include "serial_runtime.h"
#include "mem.h"
#include "window.h"
#include "item.h"
#include "game_state.h"
#include "uiwindow.h"
#include "call.h"
#include "text/msg_ids.h"
#include "frame.h"
#include "map_scroll.h"

// 前方宣言
static s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out);
static s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out);
static u16 SceneData_GetBufferCrc16(u32 count, u8 *p);

struct ClearWork
{
	u8	unknown_00[20];
	u16 mode;
};

u8 *SceneData_GetScriptTable(void)
{
	return Clear_ScriptTable;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return Clear_MessageTable;
}

u8 *SceneData_GetActorTable(void)
{
	return Clear_ActorTable;
}

u8 *SceneData_GetEffectTable(void)
{
	return Clear_EffectTable;
}

static void Clear_LoadBackground(void)
{
	u8				  *res;
	s32				   id;
	u16				  *map;
	u32				   x;
	u32				   y;
	u16				   tile;
	struct ScrollPair *scroll;
	s32				   blank;

	id = (s32)&ResourceId_GoldenSunLogo;
	Engine_BlendSetDarkenTarget16(0);
	REG_BG2CNT = 0x681;
	gBgScroll[2].y = 0;
	blank = 0x1ff;
	res = Engine_ResourceGetTableEntry(id);
	Dma_Set(res, (void *)0x05000000, 0x84000070, REG_DMA3);
	res += 0x1c0;
	Value2(Resource_DecodeType01, (s32)res, (s32)Ram_MapCellBuffer);
	Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
	map = (u16 *)0x06003000;
	tile = 0x1a0;
	for (y = 0; y < 20; y++)
	{
		for (x = 0; x < 30; x++)
			*map++ = tile++;
		*map++ = blank;
		*map++ = blank;
	}
	scroll = (struct ScrollPair *)gBgScroll;
	for (y = 0; y <= 3; y++)
	{
		scroll->y = 0;
		scroll->x = 0;
		scroll++;
	}
	Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
	((struct ClearWork *)gMapWork[0])->mode = 0x1400;
	{
		struct FieldActor *leader;

		leader = Object_GetById(gGameState.selected_actor);
		leader->motion_flags = 0;
	}
}

static void Clear_UpdateBlend(void)
{
	s32 frame;
	s32 level;

	frame = *(u16 *)&Clear_BlendFrame + 1;
	*(u16 *)&Clear_BlendFrame = frame;
	level = (u16)frame >> 1;
	IO_WRITE_QUEUE_PUSH(0x4000050, 0x2e51, 0x20000);
	IO_WRITE_QUEUE_PUSH(0x4000052, ((16 - (u16)level) << 8) | (u16)level, 0x20000);
	if ((u16)level > 15)
		Engine_TaskRemoveCallback(Clear_UpdateBlend);
}

// 隠しコマンド: 押したキーの並び、0 で終わり
const u16 Clear_CodeSequence[] = {
	KEY_SELECT, KEY_SELECT, KEY_SELECT, 0,
};

const u16 Clear_ExtraCodeSequence[] = {
	KEY_UP, KEY_DOWN, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT,
	KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT, KEY_UP, KEY_SELECT, 0,
};

static void Clear_CheckButtonCodes(void)
{
	if (Clear_CodeUnlocked == 0)
	{
		if (Clear_CodeHeld != 0)
		{
			if (gKeysHeld == 0)
				Clear_CodeHeld = 0;
		}
		else if (gKeysHeld != 0)
		{
			if (gKeysHeld
				== ((struct { u16 key[1]; } *)Clear_CodeSequence)->key[Clear_CodeProgress])
			{
				Clear_CodeProgress++;
				Clear_CodeHeld = 1;
				if (((struct { u16 key[1]; } *)Clear_CodeSequence)->key[Clear_CodeProgress] == 0)
				{
					Clear_CodeUnlocked = 1;
					Engine_AudioPlayCue(110);
				}
			}
			else
			{
				Clear_CodeProgress = 0;
			}
		}
	}
	if (Clear_ExtraCodeUnlocked == 0)
	{
		if (*(s16 *)&Clear_ExtraCodeHeld != 0)
		{
			if (gKeysHeld == 0)
				*(s16 *)&Clear_ExtraCodeHeld = 0;
		}
		else if (gKeysHeld != 0)
		{
			if (gKeysHeld
				== ((struct { u16 key[1]; } *)Clear_ExtraCodeSequence)
					->key[Clear_ExtraCodeProgress])
			{
				Clear_ExtraCodeProgress++;
				*(s16 *)&Clear_ExtraCodeHeld = 1;
				if (((struct { u16 key[1]; } *)Clear_ExtraCodeSequence)
						->key[Clear_ExtraCodeProgress]
					== 0)
				{
					Clear_ExtraCodeUnlocked = 1;
					Engine_AudioPlayCue(110);
				}
			}
			else
			{
				Clear_ExtraCodeProgress = 0;
			}
		}
	}
}

static void Clear_NameEntryCharacter(s32 cell, u8 *text)
{
	text[1] = 0;
	text[2] = 0;
#if EDITION_INTERNATIONAL
	if (cell <= 7)
		text[0] = cell + 'A';
	else if (cell <= 12)
		text[0] = cell + 'A' + 1;
	else if (cell <= 23)
		text[0] = cell + 'A' + 2;
	else if (cell <= 31)
		text[0] = cell + 26;
	else if (cell <= 42)
		text[0] = cell + 'A';
	else if (cell <= 44)
		text[0] = cell + 'A' + 1;
	else if (cell <= 55)
		text[0] = cell + 'A' + 2;
	else if (cell == 56)
		text[0] = '!';
	else if (cell == 57)
		text[0] = '?';
	else if (cell == 58)
		text[0] = '#';
	else if (cell == 59)
		text[0] = '&';
	else if (cell == 60)
		text[0] = '$';
	else if (cell == 61)
		text[0] = '%';
	else if (cell == 62)
		text[0] = '+';
	else
		text[0] = '=';
#else
	if (cell <= 14)
		text[0] = cell - 111;
	else if (cell <= 44)
		text[0] = cell - 47;
	else if (cell == 45)
		text[0] = 0x86;
	else
	{
		if (cell <= 55)
			text[0] = cell + 104;
		else if (cell == 56)
			text[0] = 0xe0;
		else if (cell == 57)
			text[0] = 0xe3;
		else if (cell == 58)
			text[0] = 0xe4;
		else
			text[0] = cell - 81;
		text[1] = TEXT_MARK_DAKUTEN;
	}
#endif
}

static s32 Scene_GetModeMask(void)
{
	s32 mode;

	if (Engine_GameFlagIsSet(324) == 0)
	{
		return 0;
	}
	if (gGameState.scene_variant == 2)
	{
		return 0;
	}
	mode = gGameState.scene ^ (s32)&SceneId_WorldMap;
	return -((u32)(-mode | mode) >> 31);
}

struct SaveMenuSceneState
{
	u8	unknown_000[0x100];
	s32 word_100;
};

struct SaveLinkGreeting
{
	u16 code[4];
	u8	reserved[16];
};

s32							   __divsi3(s32, s32);

s32 SaveMenu_Run(void)
{
	s32 page_count;
	s32 encoded_size;
	s32 page_window;
	s32 rec5;
	s32 rec7;
	s32 record;
	s32 value;
	s32 v5;
	s32 v7;
	s32 v6;
	s32 link_message;
	s32 v1;
	s32 v4;
	s32 n;
	s32 page_message;
	s32 page;
	s32 started;
	s32 help_window;
	s32 heading_window;
	u8	save_data[200];
#if EDITION_INTERNATIONAL
	u8 password[320];
#else
	u8 password[260];
#endif
	u8 glyph[4];

	started = 0;
	Clear_LoadBackground();
	Engine_TaskAddCallback((s32)Clear_CheckButtonCodes, 0xc80);
	gEventWork->start_transition = started;
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	if (gGameState.entrance == 2)
	{
	L_ask:
		UiText_ShowPositionedMessageAndWait((s32)MsgSaveAdventureQuestion, 5);
		v7 = Object_CallSpawnRoutineAtOrigin(1);
		UiWork_FinalizePendingCore();
		if (v7 == 0)
		{
			UiText_ShowPositionedMessageAndWait(((s32)MsgSaveAdventureQuestion + 1), 1);
			gGameState.unknown_20f = 1;
			v7 = Save_WriteSelectedSlot();
			if (v7 == -1)
			{
				goto L_ask;
			}
		}
		Engine_EventWait(60);
		Engine_EventCloseScreen();
		Engine_AudioPlayCue(17);
		Engine_EventWait(0x12c);
		Event_SetDestination((s32)&SceneId_WorldMap, 72);
		return 0;
	}
	gSleepDisabled = started;
menu_top:;
	v6 = SaveState_ScanRecordFlags();
	if (v6 < 0)
	{
		if (gDebugMode != 0)
		{
			gGameState.saved_options = 1;
			gOptionMirror = 1;
			UiText_ShowCenteredMessage((s32)MsgNoBackupMemory, 1, 8);
		}
	}
	if (v6 == 0)
	{
		if (started == 0)
		{
			Engine_TaskWait(30);
			Engine_TaskAddCallback((s32)Clear_UpdateBlend, 0xc80);
			Engine_TaskWait(1);
			started = 1;
		}
	}
	if (v6 > 0)
	{
		v6 = Menu_SelectSaveSlotAction();
	}
	else
	{
		v6 = 0;
	}
	if (v6 == 0)
	{
		GameState_InitDefaults();
		PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
		v5 = 1;
		v7 = 0;
	edit_name:;
		Engine_TaskWait(6);
		v6 = NameEntry_EditOwnerName(v7);
		if (v6 == -1)
		{
			if (v7 == 0)
			{
				goto menu_top;
			}
			v7--;
			goto edit_name;
		}
		if (Clear_CodeUnlocked != 0)
		{
			v5 = 4;
		}
		if (Clear_ExtraCodeUnlocked != 0)
		{
			v5 = 7;
		}
		v7 = (v7 + 1);
		if (v7 < v5)
		{
			goto edit_name;
		}
		Party_SetFlag32AndRefreshMembers();
		gGameState.scene = (s32)&SceneId_HaidiaBabi;
		gGameState.entrance = 20;
		goto leave;
	}
	if (v6 == 1)
	{
		struct GameState *state;

		v6 = SaveState_LoadRecordIntoWork(1);
		if (v6 == -1)
		{
			goto menu_top;
		}
		GameFlag_Set(0x109);
		state = &gGameState;
		PaletteGlow_Update(state->palette_glow[0], state->palette_glow[1]);
		if (state->build_stamp != Runtime_GetBuildStampTime())
		{
			state->scene = state->saved_scene;
			state->entrance = state->saved_entrance;
			GameFlag_ClearBit(0x109);
		}
		else if ((gKeysHeld & 0x208) == 0x208)
		{
			if (Scene_GetModeMask() != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveRecoveryFailed, 9);
				goto menu_top;
			}
			state->scene = state->saved_scene;
			state->entrance = state->saved_entrance;
			GameFlag_ClearBit(0x109);
			Engine_GameFlagSet(0x13e);
		}
		else if (state->play_time != ((struct SaveMenuSceneState *)gSceneState)->word_100)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted, 9);
			UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 1, 13);
			if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0)
			{
				UiWork_FinalizePendingCore();
				goto menu_top;
			}
			UiWork_FinalizePendingCore();
			if (Scene_GetModeMask() != 0)
			{
				/* FAKEMATCH: MsgSaveRecoveryFailed as MsgSaveDataCorrupted + 2, the native adds 2 to the register holding message 4 (186 by name). */
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 2, 9);
				goto menu_top;
			}
			state->scene = state->saved_scene;
			state->entrance = state->saved_entrance;
			GameFlag_ClearBit(0x109);
			Engine_GameFlagSet(0x13f);
		}
		GameFlag_ClearBit(0x106);
		goto leave;
	}
	if (v6 == 2)
	{
		SaveState_CopySlotPair();
		goto menu_top;
	}
	if (v6 == 3)
	{
		SaveState_DeleteSelectedSlot();
		goto menu_top;
	}
	if (v6 == 4)
	{
		v6 = SaveState_LoadRecordIntoWork(4);
		if (v6 == -1)
		{
			goto menu_top;
		}
		gGameState.selected_actor = 0;
		if (GameFlag_IsSet(0x952) != 0)
		{
			InventorySnapshot_Restore();
			Party_RemoveActiveOwner(0);
			Party_RemoveActiveOwner(1);
			Party_RemoveActiveOwner(2);
			Party_RemoveActiveOwner(3);
			Party_AddActiveOwner(0);
			Party_AddActiveOwner(1);
			Party_AddActiveOwner(2);
			Party_AddActiveOwner(3);
		}
		PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
		GameFlag_ClearBit(0x109);
		GameFlag_ClearBit(0x106);
		Engine_GameFlagSet(0x17e);
		gSleepDisabled = 1;
		Event_SetDestination((s32)&SceneId_LinkLobby, 1);
		goto leave;
	}
	else
	{
		if (v6 != 5)
		{
			goto menu_top;
		}
	link_load:;
		v6 = SaveState_LoadRecordIntoWork(5);
		if (v6 == -1)
		{
			goto menu_top;
		}
		PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	link_select:;
		v6 = Menu_SelectResourceLayout(0);
		if (v6 == -1)
		{
			goto link_load;
		}
		if (v6 == 1)
		{
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
			v6 = UiWindow_Create(4, 5, 22, 8, 2);
#else
			v6 = UiWindow_Create(6, 5, 18, 8, 2);
#endif
			link_message = (s32)MsgClearSaveLabel;

			UiText_DrawResource(link_message, v6, 0, 4);
			UiText_DrawResource((link_message + 1), v6, 0, 16);
			UiText_DrawResource((link_message + 3), v6, 0, 36);
			SerialRuntime_Initialize();
			Engine_TaskWait(10);
			{
				u16 *sync = (u16 *)gSerialTransfer.payload;

				sync[0] = 0x30;
				sync[1] = 0x30;
				sync[2] = 0x30;
				sync[3] = 0x30;
			}
			v7 = 0;
			v5 = 3;
			for (v1 = 0; v1 != 4; v1++)
			{
				u16 *greeting = ((struct SaveLinkGreeting *)gLinkPeerSignatures)[v1].code;

				greeting[0] = 0x30;
				greeting[1] = 0x30;
				greeting[2] = 0x30;
				greeting[3] = 0x30;
			}
			while ((gKeyState & KEY_B) == 0)
			{
				if ((v5 & gLinkStatus) == v5)
				{
					u32	 control = REG_SIOCNT;
					u32	 peer = ((control << 26) >> 30) ^ 1;
					u16 *received = ((struct SaveLinkGreeting *)gLinkPeerSignatures)[peer].code;

#if defined(TBS_EDITION_DE)
					if (received[0] == 69)
					{
#elif defined(TBS_EDITION_FR)
					if (received[0] == 70)
					{
#elif defined(TBS_EDITION_ES)
					if (received[0] == 83)
					{
#elif defined(TBS_EDITION_IT)
					if (received[0] == 73)
					{
#elif !EDITION_INTERNATIONAL
					if (received[0] == 67)
					{
#else
					if (received[0] == 85)
					{
#endif
						if (received[1] == 86)
						{
							if (received[2] == 84)
							{
								if (received[3] == 83)
								{
									v7 = 1;
									goto link_waited;
								}
							}
						}
					}
				}
				Engine_TaskWait(1);
			}
			Engine_AudioPlayCue(113);
		link_waited:;
			UiWork_Finalize(v6, 2);
			if (v7 == 0)
			{
				goto link_select;
			}
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
			v6 = UiWindow_Create(4, 10, 22, 4, 2);
#else
			v6 = UiWindow_Create(5, 10, 20, 4, 2);
#endif

			UiText_DrawResource((s32)MsgClearSendingData, v6, 0, 4);
			Engine_TaskWait(10);
			SerialRuntime_BeginTransferA((s32)gSaveBuffer, 0x1004);
			Engine_TaskWait(10);
			v5 = 0;
			v1 = 3;
			v4 = 1;
			for (v7 = 0; v7 <= 0x927bf; v7++)
			{
				v5 = (v5 + 1);
				if ((v1 & gLinkStatus) == v1)
				{
					v5 = 0;
				}
				if (v5 == 10)
				{
					v4 = 0;
					break;
				}
				if (gSerialSendSource == 0)
				{
					break;
				}
				Engine_TaskWait(1);
			}
			if (v4 == 0)
			{
				RenderOutput_PrepareForRedraw(v6);
				UiText_DrawResource((s32)MsgClearCommunicationError, v6, 0, 4);
				v5 = 1;
				do
				{
					Engine_TaskWait(1);
				} while ((gKeyState & v5) == 0);
			}
			Engine_TaskWait(10);
			SerialRuntime_RemoveIrqHandlers();
			Engine_TaskWait(10);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			UiWork_Finalize(v6, 2);
			Engine_TaskWait(1);
#else
			RenderOutput_PrepareForRedraw(v6);
			UiWork_Finalize(v6, 2);
#endif
			goto menu_top;
		}
		if (v6 == 0)
		{
		page_select:;
			v6 = Menu_SelectResourceLayout(1);

			if (v6 == -1)
			{
				goto link_select;
			}
			{
				u8 *data = save_data;
				s16 sum;

				encoded_size = Clear_PackPasswordData(0, v6, data);
				sum = SceneData_GetBufferCrc16(encoded_size, data);
				save_data[encoded_size] = (u16)sum >> 8;
				save_data[encoded_size + 1] = sum;
				encoded_size += 2;
				encoded_size = Clear_EncodePassword(data, encoded_size, password);
			}
			v6 = 2;
			Ui_LoadWindowGraphics();
			page_window = UiWindow_Create(5, 4, 20, 12, v6);
			page = 0;
			page_count = ((encoded_size / 50) + 1);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			record = UiWindow_Create(8, 0, 13, 4, v6);
#else
			record = UiWindow_Create(10, 0, 10, 4, v6);
#endif
			page_message = (s32)MsgClearPasswordLabel;
			heading_window = record;
			UiText_DrawResource(page_message, heading_window, 6, 4);
			v7 = 1;
			if (page_count == 1)
			{
				record = UiWindow_Create(5, 16, 20, 3, v6);
				help_window = record;
#if EDITION_INTERNATIONAL
				UiText_DrawCharacterAtOffset(page_message - 2, help_window, 80, 0);
#else
				UiText_DrawCharacterAtOffset(page_message - 2, help_window, 0, 0);
#endif
			}
			else
			{
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
				record = UiWindow_Create(0, 16, 30, 3, v6);
#elif !EDITION_INTERNATIONAL
				record = UiWindow_Create(3, 16, 24, 3, v6);
#else
				record = UiWindow_Create(1, 16, 28, 3, v6);
#endif
				help_window = record;
				UiText_DrawCharacterAtOffset(page_message - 1, help_window, 0, 0);
			}
			Graphics_ExpandVramTilesByColorTable(0x6006000);
			RenderOutput_PrepareForRedraw(page_window);
			for (;;)
			{
				Link_DrawShiftedTilePair(0x6002500);
				if ((gKeyState & KEY_B) != 0)
				{
					Engine_AudioPlayCue(113);
					RenderOutput_PrepareForRedraw(page_window);
					UiWork_Finalize(page_window, 2);
					UiWork_Finalize(help_window, 2);
					UiWork_Finalize(heading_window, 2);
					goto page_select;
				}
				if ((gKeyState & KEY_A) != 0)
				{
					page = (page + 1);
					v7 = 1;
					if (page == page_count)
					{
						Engine_AudioPlayCue(112);
						RenderOutput_PrepareForRedraw(page_window);
						UiWork_Finalize(page_window, 2);
						UiWork_Finalize(help_window, 2);
						UiWork_Finalize(heading_window, 2);
						Engine_TaskWait(1);
						goto menu_top;
					}
					Engine_AudioPlayCue(111);
				}
				else
				{
					if ((gKeyState & KEY_LEFT) != 0)
					{
						if (page_count <= 1)
						{
							goto check_right;
						}
						Engine_AudioPlayCue(111);
						rec5 = page + page_count;
						rec5--;
					}
					else
					{
					check_right:;
						if ((gKeyState & KEY_RIGHT) == 0)
						{
							goto keys_done;
						}
						if (page_count <= 1)
						{
							goto keys_done;
						}
						Engine_AudioPlayCue(111);
						rec5 = page + page_count;
						rec5++;
					}
					rec5 = (rec5 % page_count);
					v7 = 1;
					page = rec5;
				}
			keys_done:;
				if (v7 != 1)
				{
					v5 = page_message;
				}
				else
				{
					RenderOutput_PrepareForRedraw(page_window);
					v7 = 0;
					v5 = 2;
					do
					{
						UiWindow_DrawDividerLine(page_window, 0, v5, 18, v5);
						v5 = (v5 + 2);
						v7 = (v7 + 1);
					} while (v7 != 4);
					if (page_count > 1)
					{
						v7 = 0;
						if (page_count != 0)
						{
							v6 = 0;
							do
							{
								value = v7 + 0xf301;
								if (v7 == page)
								{
									value = v7 + 0xf30b;
								}
								UiWindow_SetTilemapEntry(
									page_window, value, v7 + 18 - page_count, -1, v6);
								v7 = (v7 + 1);
							} while (v7 != page_count);
						}
						v5 = 0;
						UiWindow_SetTilemapEntry(
							page_window, 0xf128, (17 - (s32)page_count), -1, 0);
						UiWindow_SetTilemapEntry(page_window, 0xf129, 18, -1, 0);
						UiRender_MarkRow((struct UiRenderWork *)gWindowWork[0],
							((struct UiWindow *)page_window)->y);
					}
					v6 = page * 50 + 50;
					if (v6 > encoded_size)
					{
						v6 = encoded_size;
					}
					for (v7 = page * 50, n = 0; v7 != v6; v7++, n++)
					{
						/* FAKEMATCH: the loop note ends the CSE path, which otherwise carries the glyph address into both draw calls for gcse to hoist out of the loop; the native rebuilds sp + 24 at each call */
						do
						{
							Clear_NameEntryCharacter(password[v7] & 63, glyph);
						} while (0);
						record = (v7 % 10);
						if (record > 4)
						{
							s32 row;

							rec7 = (n % 10);
							row = (n / 10);
							UiText_DrawString(glyph, page_window, rec7 * 12 + 18, (row << 4) + 2);
						}
						else
						{
							s32 row;

							rec7 = (n % 10);
							/* FAKEMATCH: an explicit libgcc call; written with the operator, tbs-en differs */
							row = __divsi3(n, 10);
							UiText_DrawString(glyph, page_window, rec7 * 12 + 8, (row << 4) + 2);
						}
					}
					v7 = 0;
				}
				Engine_TaskWait(1);
			}
		}
		Engine_TaskWait(0x12c);
		goto menu_top;
	}
leave:;
	gEventWork->requested_exit = 0x3e7;
	Engine_EventWait(30);
	Engine_AudioPlayCue(17);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(60);
	return 0;
}

static void SaveMenu_FillTileGrid(struct RenderInput *window, s32 value)
{
	u16 *shadow;
	u16 *vram;
	s32	 saved;
	s32	 row;
	s32	 col;
	s32	 offset;

	shadow = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
	saved = (s32)Runtime_BumpAllocateAlternatePool(0x300);
	Resource_DecodeType01(value, saved);
	offset = (window->y * 32 + window->x) * 2;
	vram = (u16 *)(0x06002000 + offset);
	shadow = (u16 *)((u8 *)shadow + offset);
	for (row = 0; row <= 7; row++)
	{
		for (col = 0; col <= 15; col++)
		{
			s16 tile = (window->width * row + col) | 0xf000;

			*vram++ = tile;
			*shadow++ = tile;
		}
		vram += 16;
		shadow += 16;
	}
	Runtime_BumpFree((void *)saved);
}

static void SaveMenu_FillTileBlock(const struct RenderInput *window)
{
	s16 *mirror = ((struct UiRenderWork *)gWindowWork[0])->tilemap;
	s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
	s16 *vram;
	s32	 cell;
	s32	 row;
	s32	 base;

	cell = window->y * 32 + window->x;
	vram = (s16 *)0x06002000 + cell;
	mirror += cell;
	row = 0;
	base = 0;
	do
	{
		s32 tile = base + 32;
		s32 remaining = 15;

		do
		{
			s16 value = tile | -0x1000;

			remaining--;
			*vram = value;
			tile++;
			*mirror = value;
			vram++;
			mirror++;
		} while (remaining >= 0);
		row++;
		vram += 16;
		mirror += 16;
		base += 16;
	} while (row <= 7);
	Runtime_BumpFree(buffer);
}

struct PasswordStats
{
	s16 value_10;
	s16 value_12;
	u8	unknown_14[4];
	u16 value_18;
	u16 value_1a;
	u16 value_1c;
	u8	level_1e;
};

struct PasswordOwner
{
	u8					 unknown_00[0x0f];
	u8					 rank;
	struct PasswordStats stats;
	u8					 unknown_20[0xb8];
	u16					 items[15];
	u8					 unknown_f6[2];
	u32					 values[4];
};

static s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out)
{
	s32 length;
	u32 rank_bits;
	u32 value_bits;
	u8	item_bits;
	u8	flag_bits;
	u32 rows[8];
	s32 i;
	s32 p;
	s32 bit;
	s32 j;
	s32 k;
	s32 n;

	length = 11;
	switch (mode)
	{
	case 0:
		length = 173;
		break;
	case 1:
		length = 39;
		break;
	case 2:
		length = 9;
		break;
	}
	for (i = 0; i != length; i++)
	{
		out[i] = 0;
	}

	rank_bits = 0;
	value_bits = 0;
	item_bits = 0;
	flag_bits = 0;
	for (i = 0; i != 8; i++)
	{
		rows[i] = 0;
	}

	for (i = 0; i != 6; i++)
	{
		if (Engine_GameFlagIsSet(((const u16 *)Clear_PasswordFlags)[i]))
		{
			flag_bits |= 1 << i;
		}
	}

	for (i = 0; i != 4; i++)
	{
		struct PasswordOwner *owner =
			UnitGet(((const struct { s32 id[4]; } *)Clear_PasswordMembers)->id[i]);
		struct PasswordStats *stats = &owner->stats;

		if (stats->value_10 > 1999)
		{
			stats->value_10 = 1999;
		}
		if (stats->value_10 < 0)
		{
			stats->value_10 = 0;
		}
		if (stats->value_12 > 1999)
		{
			stats->value_12 = 1999;
		}
		if (stats->value_12 < 0)
		{
			stats->value_12 = 0;
		}
		if (stats->value_18 > 999)
		{
			stats->value_18 = 999;
		}
		if (stats->value_1a > 999)
		{
			stats->value_1a = 999;
		}
		if (stats->value_1c > 999)
		{
			stats->value_1c = 999;
		}
		if (stats->level_1e > 99)
		{
			stats->level_1e = 99;
		}
		rows[i * 2] = stats->value_10 << 21 | stats->value_12 << 10 | stats->value_18;
		rows[i * 2 + 1] = stats->value_1a << 22 | stats->value_1c << 12 | stats->level_1e << 4;
		if (owner->rank > 99)
		{
			owner->rank = 99;
		}
		if (owner->rank == 0)
		{
			owner->rank = 1;
		}
		rank_bits |= owner->rank << (i * 7);
		for (j = 0; j != 4; j++)
		{
			value_bits += owner->values[j] << (j * 7);
		}
		for (j = 0; j != 15; j++)
		{
			u16 item = owner->items[j] & 0x1ff;

			for (k = 0; k != 8; k++)
			{
				if (item == ((const u16 *)Clear_PasswordKeyItems)[k])
				{
					item_bits |= 1 << k;
				}
			}
		}
	}

	if (mode == 0)
	{
		p = 39;
		bit = 0;
		for (i = 0; i != 4; i++)
		{
			struct PasswordOwner *owner =
				UnitGet(((const struct { s32 id[4]; } *)Clear_PasswordMembers)->id[i]);

			for (n = 0; n != 15; n++)
			{
				u16 item;

				Item_Get(owner->items[n]);
				item = owner->items[n] & 0x1ff;
				out[p] += item >> (bit + 1);
				out[p + 1] += item << (7 - bit);
				bit++;
				p++;
				if (bit == 7)
				{
					bit = 0;
					p++;
				}
			}
		}

		p = 107;
		bit = -1;
		for (i = 0; i != 4; i++)
		{
			struct PasswordOwner *owner =
				UnitGet(((const struct { s32 id[4]; } *)Clear_PasswordMembers)->id[i]);
			s32 m;

			for (m = 0; m != 23; m++)
			{
				u16 quantity;

				j = 0;
				for (n = 0; n != 15; n++)
				{
					if ((owner->items[n] & 0x1ff) == ((const u16 *)Clear_PasswordStackItems)[m])
					{
						j = (owner->items[n] & 0xf800) >> 11;
					}
				}
				/* FAKEMATCH: the unused 16-bit copy leaves the shift hoisted above the sign test; without it 1095, used in both shifts 585. */
				quantity = j;
				if (bit < 0)
				{
					out[p] += (u16)j >> -bit;
					p++;
					bit += 8;
				}
				out[p] += (u16)j << bit;
				bit -= 5;
				if (bit == -5)
				{
					p++;
					bit = 3;
				}
			}
		}
		out[165] = (u32)gGameState.coins >> 16;
		out[166] = (u32)gGameState.coins >> 8;
		out[167] = gGameState.coins;
	}

	if (mode != 2)
	{
		s32 q = 8 + (mode != 0);

		for (i = 0; i != 2; i++)
		{
			out[q] = rows[i * 4] >> 24;
			out[q + 1] = rows[i * 4] >> 16;
			out[q + 2] = rows[i * 4] >> 8;
			out[q + 3] = rows[i * 4];
			out[q + 4] = rows[i * 4 + 1] >> 24;
			out[q + 5] = rows[i * 4 + 1] >> 16;
			out[q + 6] = rows[i * 4 + 1] >> 8;
			out[q + 7] = rows[i * 4 + 1];
			out[q + 7] |= rows[i * 4 + 2] >> 28;
			out[q + 8] = rows[i * 4 + 2] >> 20;
			out[q + 9] = rows[i * 4 + 2] >> 12;
			out[q + 10] = rows[i * 4 + 2] >> 4;
			out[q + 11] = rows[i * 4 + 2] << 4;
			out[q + 11] |= rows[i * 4 + 3] >> 28;
			out[q + 12] = rows[i * 4 + 3] >> 20;
			out[q + 13] = rows[i * 4 + 3] >> 12;
			out[q + 14] = rows[i * 4 + 3] >> 4;
			q += 15;
		}
	}

	out[0] = rank_bits;
	out[1] = rank_bits >> 8;
	out[2] = rank_bits >> 16;
	out[3] = (rank_bits >> 20 & 0xf0) | (value_bits & 0x0f);
	out[4] = value_bits >> 4;
	out[5] = value_bits >> 12;
	out[6] = value_bits >> 20;
	out[7] = flag_bits;
	if (mode != 0)
	{
		out[8] = item_bits;
	}
	return length;
}

static s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out)
{
	s32 key;
	s32 count;
	s32 i;
	s32 j;
	s32 bit;
	s32 index;
	s32 value;
	s32 sum;
	s32 group;
	s32 b;

	key = data[size - 1];
	count = 0;
	for (i = 0; i != size - 1; i++)
		data[i] ^= key;
	bit = 0;
	index = 0;
	sum = 0;
	group = 0;
	do
	{
		value = 0;
		for (j = 0; j != 6; j++)
		{
			b = (data[index] >> (7 - bit)) & 1;
			bit++;
			if (bit == 8)
			{
				bit = 0;
				index++;
			}
			value |= b << (5 - j);
			if (index == size)
				break;
		}
		group++;
		out[count++] = value;
		sum += value;
		if (group == 9)
		{
			out[count++] = sum & 63;
			sum = 0;
			group = 0;
		}
	} while (index != size);
	for (i = 0; i != count; i++)
		out[i] = (out[i] + i) & 63;
	return count;
}

static u16 SceneData_GetBufferCrc16(u32 count, u8 *p)
{
	u32 crc;
	u32 i;
	u32 j;

	i = 0;
	crc = 0xFFFF;
	if (count != 0)
	{
		do
		{
			crc ^= *p << 8;
			j = 0;
			do
			{
				if ((crc & 0x8000) != 0)
				{
					crc = (crc << 1) + 0xFFFFEFDF;
				}
				else
				{
					crc <<= 1;
				}
				j++;
			} while (j != 8);
			i++;
			p++;
		} while (i != count);
	}
	return ~crc;
}

struct SceneEntrance Clear_ScriptTable[] = {
	{ 0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Clear_MessageTable[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement Clear_ActorTable[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent Clear_EffectTable[] = {
	{ SCENE_EVENTS_END },
};

s16 Clear_BlendFrame = 0;

s16 Clear_CodeUnlocked = 0;

s16 Clear_ExtraCodeUnlocked = 0;

s16 Clear_CodeProgress = 0;

s16 Clear_ExtraCodeProgress = 0;

s16 Clear_CodeHeld = 0;

s32 Clear_ExtraCodeHeld = 0;

u32 Clear_PasswordMembers[] = {
	0, 1, 2, 3,
};

u32 Clear_PasswordFlags[] = {
	0x9510941, 0x8d108b3, 0x868081e,
};

u32 Clear_PasswordKeyItems[] = {
	0xc900c8, 0xcb00ca, 0xcd00cc, 0xcf00ce,
};

u16 Clear_PasswordStackItems[] = {
	180, 181, 182, 183, 186, 187, 188, 189,
	191, 192, 193, 194, 195, 196, 226, 227,
	228, 229, 236, 238, 239, 240, 241,
};
