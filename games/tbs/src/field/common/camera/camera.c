#include "unit/field/common/camera/camera.h"
#include "unit/system/scheduler/callback.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "io_reg.h"
#include "fixed_point_position.h"
#include "btldisp.h"
#include "project.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "edition.h"
#include "callback_scheduler.h"
#include "call.h"

s32 Runtime_BlankDisplayAndRun(void)
{
	REG_DISPCNT = 0x40;
	AudioCommand_PlayFar(9);
	LuckyDice_Run();
	return 0;
}

void Camera_ResetSceneDefaults(void)
{
	struct BattleCamera		 *state = gCameraWork;
	struct FixedPointPosition local;
	u32						  result;
	u32						  param1;

	state->pitch = 152 << 8;
	state->distance = 255 << 17;
	state->pos[0] = 0;
	state->pos[1] = 0;
	state->pos[2] = 0;
	state->yaw = 0;
	state->follow_pos = 0;
	gProjection.center_x = 0;
	gProjection.center_y = 0;
	state->unknown_18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	local.x = 0;
	local.y = 0;
	local.z = state->distance;
	Iwram_TransformVector((s32 *)&local, (s32 *)state);

	param1 = 250;
	param1 <<= 16;
	result = Iwram_RatioMulQ14(param1, 192 << 8);

	param1 = 250;
	param1 <<= 16;
	Camera_StoreSceneParameters(param1, result, 0x7fff0000);
}

s32 FixedPoint_Multiply8_8(s32 arg0, s32 arg1)
{
	s16 left = arg0;
	s16 right = arg1;
	s32 product;
	s32 adjusted;
	s32 multiplier;

	multiplier = right;
	product = left;
	product *= multiplier;
	if (product >= 0)
	{
		adjusted = product;
	}
	else
	{
		adjusted = product + 255;
	}
	return (s16)(adjusted >> 8);
}

s16 Math_ScaleByRatio(s16 arg0, s16 arg1)
{
	return (arg0 << 8) / arg1;
}

s16 FixedPoint_Reciprocal(s16 value)
{
	return 0x10000 / value;
}

#if !GAME_TLA
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"
#include "fixed_math.h"
#include "ui.h"
#include "game_state.h"
#include "map_scroll.h"
#include "battle_effect_work.h"
#include "transform.h"
#include "resource_ids.h"
#include "text/msg_ids.h"

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#define DICE_COIN_X 17
#define DICE_COIN_W 13
#define DICE_WIN_W 13
#elif EDITION_INTERNATIONAL
#define DICE_COIN_X 18
#define DICE_COIN_W 12
#define DICE_WIN_W 12
#else
#define DICE_COIN_X 18
#define DICE_COIN_W 11
#define DICE_WIN_W 10
#endif

#if defined(TBS_EDITION_ES)
#define DICE_LABEL_X 56
#elif defined(TBS_EDITION_FR)
#define DICE_LABEL_X 53
#else
#define DICE_LABEL_X 48
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES)
#define DICE_HELP_X 20
#define DICE_HELP_W 10
#elif defined(TBS_EDITION_FR)
#define DICE_HELP_X 21
#define DICE_HELP_W 9
#elif EDITION_INTERNATIONAL
#define DICE_HELP_X 22
#define DICE_HELP_W 8
#else
#define DICE_HELP_X 23
#define DICE_HELP_W 7
#endif

#if EDITION_INTERNATIONAL
#define DICE_WIN_X 9
#define DICE_WIN_NUM_X 48
#define DICE_WIN_LABEL_X DICE_LABEL_X
#define DICE_KIND_X 0x278
#else
#define DICE_WIN_X 10
#define DICE_WIN_NUM_X 40
#define DICE_WIN_LABEL_X 40
#define DICE_KIND_X 0x280
#endif

struct LuckyDiceObj
{
	volatile u32 attr01;
	volatile u32 attr2;
};

struct LuckyDiceSprites
{
	u8					filler[200];
	struct LuckyDiceObj obj[128];
	s32					coins_window;
	s32					winnings_window;
	s32					help_window;
};

#include "keystate.h"
extern u8 gOamCopyEnabled;
// どこからも参照されない符号の表
const s32 LuckyDice_UnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

// 結果の文字のスプライト数（種類ごと）
const u8 LuckyDice_LabelCounts[] = {5, 4, 3, 3};

// 結果の文字の先頭タイル（種類ごと）
const u8 LuckyDice_LabelTiles[] = {
	28,
	48,
	64,
	76,
#if EDITION_INTERNATIONAL
	28,
	248,
	64,
	248, // 読まれない
#endif
};

// 回転の段階ごとのさいころの面のタイル
const u8 LuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10, 3, 9, 13, 4, 7, 6, 5, 17, 11};

// 面を決める高さの境目（z > 値 << 16）
const s16	LuckyDice_FaceHeights[] = {147, 80, 15, -42, -95, -32767};

#endif

s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count)
{
	s32 remaining;
	u32 mask_red;
	u32 mask_green;
	u32 mask_blue;
	u32 pixel;
	u32 red;
	u32 green;
	u32 blue;

	if (count > 0)
	{
		mask_red = 0x1f;
		mask_green = 0x3e0;
		mask_blue = 0x7c00;
		remaining = count;
		do
		{
			pixel = *source;
			red = pixel & mask_red;
			green = pixel & mask_green;
			blue = mask_blue & pixel;
			red *= scale;
			green *= scale;
			blue *= scale;
			pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
			pixel |= (blue >> 16) & mask_blue;
			*destination = pixel;
			source++;
			destination++;
			remaining--;
		} while (remaining != 0);
	}
	return 0;
}

#if !GAME_TLA
void LuckyDice_Run(void)
{
	u16 palette[256];
	s32 depth[4];
	s32 results[4];
	s32 point[3];
	s32 proj[3];
	/* FAKEMATCH: unused local; the native frame keeps 32 unused bytes at sp+168..200, between proj and the affine halfwords (without it 42) */
	s32 unused[8];
	/* FAKEMATCH: volatile restores the form the bytes prove, four halfwords stored to sp+166..160 and each reloaded with ldrh (without it 902) */
	volatile u16			 affine_a;
	volatile u16			 affine_b;
	volatile u16			 affine_c;
	volatile u16			 affine_d;
	s32						 spin[2];
	s32						 flash[2];
	s32						 faces[2];
	s32						 order[2];
	s32						 matches[2];
	u16						*obj_palette;
	struct BattleEffectWork *work;
	/* FAKEMATCH: the OAM shadow entries (struct LuckyDiceObj) are volatile, the form the bytes prove: loop.c forms no DEST_ADDR givs for volatile stores, so every obj loop keeps the count * 8 + 200 and + 204 offset givs (without it 1702) */
	struct LuckyDiceSprites *sprites;
	struct BattleCamera		*render;
	s32						 frame;
	s32						 state;
	s32						 bet;
	s32						 timer;
	s32						 kind;
	s32						 winnings;
	s32						 counter;
	s32						 digits;
	s32						 count;
	struct EffectStep		*die;
	struct EffectStep		*other;
	s32						 reset;
	s32						 window;
	s32						 i;
	s32						 j;
	s32						 n;
	s32						 x;
	s32						 y;
	s32						 d;
	s32						 e;
	s32						 key;
	s32						 tx;
	s32						 ty;
	s32						 base;
	s32						 dx;
	s32						 dy;
	s32						 dz;
	s32						 dist;
	s32						 speed;
	u32						 angle;
	s32						 scale;
	s32						 pairs;
	s16						 value;

	Runtime_AllocateHeapBlock(41, 0x60e);
	obj_palette = Runtime_AllocateHeapBlock(40, 0x200);
	work = Runtime_AllocateBlock(39, 0x782c);
	sprites = Runtime_AllocateBlock(45, 0x618);
	render = Runtime_AllocateBlock(12, 76);
	RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
	Camera_ResetSceneDefaults();
	Scheduler_ResetTaskTable();
	gOamCopyEnabled = 0;

	i = 0;
	for (ty = 0; ty != 20; ty++)
	{
		for (tx = 0; tx != 32; tx++)
		{
			*(volatile u16 *)(0x06003000 + i) = tx + ty * 30;
			i += 2;
		}
	}

	base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
	Dma_Set((void *)base, (void *)0x05000000, 0x84000070, REG_DMA3);
	Dma_Set((void *)0x05000000, palette, 0x84000080, REG_DMA3);
	base += 0x1c0;
	Value2(Resource_DecodeType01, base, (s32)Ram_MapCellBuffer);
	Dma_Set(Ram_MapCellBuffer, (void *)0x06004000, 0x84002580, REG_DMA3);
	base = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
	Dma_Set((void *)base, obj_palette, 0x84000080, REG_DMA3);
	Dma_Set((void *)base, (void *)0x05000200, 0x84000080, REG_DMA3);
	base += 0x200;
	Value2(Resource_DecodeType01, base, (s32)Ram_MapCellBuffer);
	Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x84001f00, REG_DMA3);
	Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, 0, 256);
	Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, 0, 256);

	REG_BG1CNT = 0x0686;
	REG_WININ = 0x3737;
	REG_WINOUT = 0x2723;
	REG_DISPCNT = 0x3340;
	REG_BLDCNT = 0x3f44;
	REG_BLDALPHA = 0x0810;
	gBgScroll[1].x = 0;
	gBgScroll[1].y = 0;
	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0xff60;
	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0xff60;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0xa0;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0xa0;

	state = 0;
	timer = 0;
	kind = -1;
	winnings = 0;
	counter = 0;
	for (i = 0; i != 2; i++)
	{
		work->particles[i].x = ((Random16() & 127) - 64) << 16;
		work->particles[i].y = (Random16() & 31) << 16;
		work->particles[i].z = ((Random16() & 127) - 64) << 16;
		work->particles[i].velocity_x = ((Random16() & 127) - 64) << 14;
		work->particles[i].velocity_y = ((Random16() & 31) + 32) << 12;
		work->particles[i].velocity_z = ((Random16() & 127) - 64) << 14;
		work->particles[i].variant = 0;
		spin[i] = 300;
	}
	results[0] = results[1] = results[2] = results[3] = 0;
	flash[0] = flash[1] = 0;
	faces[0] = Random16() % 6;
	do
	{
		faces[1] = Random16() % 6;
	} while (faces[0] == faces[1]);
	work->particles[0].x = 0x280000;
	work->particles[0].y = 0x600000;
	work->particles[0].z = -0xa00000;
	work->particles[1].x = -0x280000;
	work->particles[1].y = 0x600000;
	work->particles[1].z = -0xa00000;

	UiWork_InitializeWithResourceCountersFar();
	window = UiWindow_CreateFar(DICE_COIN_X, 0, DICE_COIN_W, 3, 6);
	sprites->coins_window = window;
	UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins, window, DICE_LABEL_X, 0);
	UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
	window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
	sprites->help_window = window;
	UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 10, window, 0, 0);
	UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceCoins + 11, sprites->help_window, 0, 8);

	frame = 0;
	for (;;)
	{
		reset = 0;
		if (frame <= 16)
		{
			Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, frame << 12, 256);
			Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, frame << 12, 256);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(render->eye, render->pos);

		count = 0;
		sprites->obj[count].attr01 = 0xc0006000;
		sprites->obj[count].attr2 = 0x358;
		count++;
		sprites->obj[count].attr01 = 0x40102020;
		sprites->obj[count].attr2 = 0x398;
		count++;
		sprites->obj[count].attr01 = 0x40102030;
		sprites->obj[count].attr2 = 0x3a0;
		count++;
		sprites->obj[count].attr01 = 0x40042020;
		sprites->obj[count].attr2 = 0x3a8;
		count++;
		sprites->obj[count].attr01 = 0x40042030;
		sprites->obj[count].attr2 = 0x3a8;
		count++;
		for (i = 0; i != 2; i++)
		{
			sprites->obj[count].attr01 = (32 + i * 16) | 0x40202000;
			sprites->obj[count].attr2 = faces[i] * 8 + 0x3b0;
			count++;
		}

		if (state == 2 || state == 3)
		{
			for (i = 0; i != 4; i++)
			{
				if (timer == i * 5)
				{
					AudioCommand_PlayFar(0x12f);
				}
				if (timer >= i * 5)
				{
					sprites->obj[count].attr01 = 0x80002040 | ((56 + i * 32) << 16);
					sprites->obj[count].attr2 = results[i] * 32 + 152;
					count++;
				}
			}
			if (timer > 19 && kind >= 0)
			{
				for (i = 0; i != LuckyDice_LabelCounts[kind]; i++)
				{
					s32 kx;
					s32 ky;

					kx = DICE_KIND_X - LuckyDice_LabelCounts[kind] * 16 + i * 32;
					ky = 96;
					sprites->obj[count].attr01 = (((kx & 0x1ff) << 16) | ky) | 0x80002000;
					sprites->obj[count].attr2 = LuckyDice_LabelTiles[kind] * 8 + 152 + i * 32;
					count++;
				}
			}
		}

		if (state == 2)
		{
			if (timer == 20 && winnings > 0)
			{
				Party_AdjustCoinsFar(winnings);
				if (winnings <= 9)
				{
					digits = 1;
				}
				else if (winnings <= 99)
				{
					digits = 2;
				}
				else if (winnings <= 999)
				{
					digits = 3;
				}
				else if (winnings <= 9999)
				{
					digits = 4;
				}
				else
				{
					digits = 5;
				}
				window = UiWindow_CreateFar(DICE_WIN_X - digits / 2, 16, digits + DICE_WIN_W, 3, 6);
				sprites->winnings_window = window;
				UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceWinnings, window, 0, 0);
				UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceWinnings - 1,
					sprites->winnings_window, digits * 8 + DICE_WIN_LABEL_X, 0);
				UiText_DrawNumberInWindowFar(
					winnings, digits, sprites->winnings_window, DICE_WIN_NUM_X, 0);
			}
			if (counter > 0)
			{
				counter--;
			}
			if (timer > 19 && (gKeysRepeat & KEY_A))
			{
				AudioCommand_PlayFar(112);
				if (winnings > 0)
				{
					state = 3;
					counter = 0;
				}
				else
				{
					reset = 1;
					state = 0;
#if EDITION_INTERNATIONAL
					window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
					sprites->help_window = window;
					UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceThrow, window, 0, 0);
					UiText_DrawCharacterAtOffsetFar(
						(s32)MsgLuckyDiceThrow + 1, sprites->help_window, 0, 8);
#endif
				}
			}
			timer++;
		}
		else if (state == 3)
		{
			if (counter == 0)
			{
				winnings = 0;
				UiText_DrawNumberInWindowFar(0, digits, sprites->winnings_window, 40, 0);
				UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
			}
			if (counter == 15)
			{
				state = 0;
				UiWork_FinalizeFar(sprites->winnings_window, 1);
				window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
				sprites->help_window = window;
				UiText_DrawCharacterAtOffsetFar((s32)MsgLuckyDiceThrow, window, 0, 0);
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgLuckyDiceThrow + 1, sprites->help_window, 0, 8);
				reset = 1;
			}
			counter++;
		}
		else if (state == 0 && frame > 17)
		{
			/* FAKEMATCH: two discarded reads of gKeysRepeat, the form the bytes prove (native loads it twice before the KEY_UP test; without them 189) */
			(void)gKeysRepeat;
			(void)gKeysRepeat;
			if (gKeysHeld & KEY_UP)
			{
				work->particles[0].y += 0x40000;
				work->particles[1].y += 0x40000;
			}
			if (gKeysHeld & KEY_DOWN)
			{
				work->particles[0].y += -0x40000;
				work->particles[1].y += -0x40000;
			}
			if (work->particles[0].y < 0x200000)
			{
				work->particles[0].y = 0x200000;
			}
			if (work->particles[0].y > 0xa00000)
			{
				work->particles[0].y = 0xa00000;
			}
			if (work->particles[1].y < 0x200000)
			{
				work->particles[1].y = 0x200000;
			}
			if (work->particles[1].y > 0xa00000)
			{
				work->particles[1].y = 0xa00000;
			}
			if (gGameState.coins < (u32)(Party_GetAverageLevelFar() * 10))
			{
				UiWork_FinalizeFar(sprites->help_window, 1);
				break;
			}
			if (gKeysRepeat & KEY_B)
			{
				AudioCommand_PlayFar(113);
				UiWork_FinalizeFar(sprites->help_window, 1);
				break;
			}
			if (gKeysRepeat & KEY_A)
			{
				AudioCommand_PlayFar(0x12e);
				UiWork_FinalizeFar(sprites->help_window, 1);
				state = 1;
				bet = Party_GetAverageLevelFar() * 10;
				if ((u32)bet > gGameState.coins)
				{
					bet = gGameState.coins;
				}
				Party_AdjustCoinsFar(-bet);
				UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
				for (i = 0; i != 2; i++)
				{
					die = &work->particles[i];
					if (i > 1)
					{
						die->z = -0xb40000;
						die->y = 0x600000;
					}
					die->velocity_x = ((Random16() & 127) - 64) << 12;
					die->velocity_y = ((Random16() & 15) + 48) << 13;
					die->velocity_z = (((Random16() & 63) + 140) << 12) + die->y / 6;
					die->variant = Random16() % 0x4800;
					spin[i] = 300;
				}
			}
		}

		if (reset == 1)
		{
			work->particles[0].x = 0x280000;
			work->particles[0].y = 0x600000;
			work->particles[0].z = -0xa00000;
			work->particles[1].x = -0x280000;
			work->particles[1].y = 0x600000;
			work->particles[1].z = -0xa00000;
			faces[0] = Random16() % 6;
			do
			{
				faces[1] = Random16() % 6;
			} while (faces[0] == faces[1]);
		}

		for (i = 0; i != 2; i++)
		{
			order[i] = i;
		}
		for (i = 1; i < 2; i++)
		{
			key = order[i];
			for (j = i - 1; j >= 0 && work->particles[order[j]].z > work->particles[key].z; j--)
			{
				order[j + 1] = order[j];
			}
			order[j + 1] = key;
		}

		for (i = 0; i != 2; i++)
		{
			s32 px;
			s32 py;

			die = &work->particles[order[i]];
			Render_ProjectPoint(die, proj);
			if (proj[2] < 0x15e)
			{
				proj[2] = 0x15e;
			}
			if (proj[2] > 0x28a)
			{
				proj[2] = 0x28a;
			}
			depth[order[i]] = proj[2] - 0x15e;
			d = 0x800 / (depth[order[i]] / 2 + 128);
			px = proj[0] - d;
			py = proj[1] - d;
			if (py + 96 > -16)
			{
				sprites->obj[count].attr01 =
					((order[i] << 25) | (((px + 0x278) & 0x1ff) << 16) | ((py + 0x160) & 255))
					| 0x40002300;
				sprites->obj[count].attr2 = (LuckyDice_FaceTiles[die->variant / 1024] << 3) | 0x400;
				count++;
			}
		}

		if (state == 0)
		{
			for (i = 0; i != 2; i++)
			{
				Render_ProjectPoint(&work->particles[i], proj);
				x = proj[0] - 0x800 / (depth[order[i]] / 2 + 128);
				y = proj[1] + 0x300 / (depth[order[i]] / 2 + 128);
				sprites->obj[count].attr01 =
					((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
				if (i == 1)
				{
					sprites->obj[count].attr01 |= 0x10000000;
				}
				sprites->obj[count].attr2 = 0x958;
				count++;
			}
		}

		for (i = 0; i != 2; i++)
		{
			s32 sx;
			s32 sy;

			point[0] = work->particles[i].x;
			point[1] = 0;
			point[2] = work->particles[i].z;
			Render_ProjectPoint(point, proj);
			if (proj[2] < 0x15e)
			{
				proj[2] = 0x15e;
			}
			if (proj[2] > 0x28a)
			{
				proj[2] = 0x28a;
			}
			depth[2 + i] = proj[2] - 0x15e;
			depth[2 + i] += work->particles[i].y >> 16;
			sx = proj[0];
			sy = proj[1];
			if (sy + 88 > -16)
			{
				sprites->obj[count].attr01 = (((i + 2) << 25) | (((sx + 0x270) & 0x1ff) << 16))
											 | ((sy + 0x158) & 255) | 0x40002300;
				sprites->obj[count].attr2 = 0x890;
				count++;
			}
		}

		if (state == 1)
		{
			s32 stopped;

			stopped = 0;
			for (i = 0; i != 2; i++)
			{
				die = &work->particles[i];
				die->x += die->velocity_x;
				die->y += die->velocity_y;
				die->z += die->velocity_z;
				for (j = 0; j != 2; j++)
				{
					other = &work->particles[j];
					if (i != j)
					{
						dx = (die->x - other->x) >> 16;
						dy = (die->y - other->y) >> 16;
						dz = (die->z - other->z) >> 16;
						if (dx * dx + dy * dy + dz * dz <= 0x9c3)
						{
							dist = Iwram_Sqrt(dx * dx + dy * dy + dz * dz);
							die->velocity_x += (dx << 15) / dist;
							die->velocity_y += (dy << 15) / dist;
							die->velocity_z += (dz << 15) / dist;
						}
					}
				}
				if (die->x < -0xc80000)
				{
					die->x = -0xc80000;
					die->velocity_x = -die->velocity_x * 48 / 64;
				}
				if (die->x > 0xc80000)
				{
					die->x = 0xc80000;
					die->velocity_x = -die->velocity_x * 48 / 64;
				}
				if (die->z < -0x960000)
				{
					die->z = -0x960000;
					die->velocity_z = -die->velocity_z * 48 / 64;
				}
				if (die->z > die->y + 0xb40000)
				{
					die->z = die->y + 0xb40000;
					if (die->velocity_z > 0)
					{
						die->velocity_y += die->velocity_z / 2;
						die->velocity_z = -die->velocity_z * 48 / 64;
					}
				}
				die->velocity_y += -0x8000;
				if (die->y <= 0)
				{
					die->y = 0;
					die->velocity_y = -die->velocity_y * 48 / 64;
					if ((Random16() & 3) == 0)
					{
						s32 vx;
						s32 vz;

						vx = die->velocity_x >> 8;
						vz = die->velocity_z >> 8;
						speed = Iwram_Sqrt(vx * vx + vz * vz) * 56 / 64;
						angle = Random16() & 0xffff;
						die->velocity_x = Trig_Sin(angle) * speed >> 8;
						die->velocity_z = Trig_Cos(angle) * speed >> 8;
					}
					else
					{
						die->velocity_x = die->velocity_x * 56 / 64;
						die->velocity_z = die->velocity_z * 56 / 64;
					}
					if (spin[i] > 19)
					{
						spin[i] -= 20;
					}
					if (die->velocity_y < 0x30000)
					{
						die->velocity_x = 0;
						die->velocity_y = 0;
						die->velocity_z = 0;
						spin[i] = 0;
						n = die->variant;
						e = n / 1024 % 3;
						if (e == 1)
						{
							die->variant = n + 0x800;
						}
						else if (e == 2)
						{
							die->variant = n + 0x400;
						}
					}
				}
				die->velocity_x = die->velocity_x * 63 / 64;
				die->velocity_y = die->velocity_y * 63 / 64;
				die->velocity_z = die->velocity_z * 63 / 64;
				die->variant += spin[i];
				if (die->variant > 0x47ff)
				{
					die->variant -= 0x4800;
				}
				if (spin[i] > 0)
				{
					spin[i]--;
				}
				if (die->velocity_x == 0 && die->velocity_y == 0 && die->velocity_z == 0
					&& die->y == 0)
				{
					results[i] = die->variant / 0xc00;
					for (j = 0; j != 6; j++)
					{
						if (die->z > LuckyDice_FaceHeights[j] << 16)
						{
							results[i + 2] = j;
							break;
						}
					}
					stopped++;
				}
			}

			if (stopped == 2)
			{
				state = 2;
				timer = 0;
				kind = -1;
				winnings = 0;
				counter = 60;
				if (results[0] == results[1] && results[0] == results[2]
					&& results[0] == results[3])
				{
					flash[0] = flash[1] = 120;
					kind = 0;
					if (results[0] == faces[0])
					{
						winnings = bet * 15;
					}
					else if (results[0] == faces[1])
					{
						winnings = bet * 25;
					}
					else
					{
						winnings = bet * 5;
					}
					UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coins_window, 0, 0);
					AudioCommand_PlayFar(93);
				}
				else
				{
					pairs = 0;
					for (i = 0; i != 3; i++)
					{
						for (j = i + 1; j != 4; j++)
						{
							if (results[i] == results[j])
							{
								matches[pairs] = results[i];
								pairs++;
							}
						}
					}
					if (pairs == 1)
					{
						winnings = bet;
						UiText_DrawNumberInWindowFar(
							gGameState.coins, 6, sprites->coins_window, 0, 0);
						kind = 3;
					}
					if (pairs == 2)
					{
						winnings = bet * 2;
						UiText_DrawNumberInWindowFar(
							gGameState.coins, 6, sprites->coins_window, 0, 0);
						flash[Random16() & 1] = 60;
						kind = 2;
						AudioCommand_PlayFar(91);
					}
					if (pairs == 3)
					{
						winnings = bet * 3;
						UiText_DrawNumberInWindowFar(
							gGameState.coins, 6, sprites->coins_window, 0, 0);
						flash[Random16() & 1] = 60;
						kind = 1;
						AudioCommand_PlayFar(92);
					}
				}
			}
		}

		for (; count != 128; count++)
		{
			sprites->obj[count].attr01 = 0x40f02000;
			sprites->obj[count].attr2 = 0;
		}

		for (i = 0; i != 4; i++)
		{
			value = depth[i] / 2 + 128;
			affine_a = value;
			affine_b = 0;
			affine_c = 0;
			affine_d = value;
			sprites->obj[i * 4 + 0].attr2 |= affine_a << 16;
			sprites->obj[i * 4 + 1].attr2 |= affine_b << 16;
			sprites->obj[i * 4 + 2].attr2 |= affine_c << 16;
			sprites->obj[i * 4 + 3].attr2 |= affine_d << 16;
		}

		Dma_Set(sprites->obj, (void *)0x07000000, 0x84000100, REG_DMA3);
		WaitFrames(1);
		frame++;
	}

	for (i = 0; i != 17; i++)
	{
		scale = 0x10000 - (i << 12);
		Graphics_ScaleRgb555Buffer(obj_palette, (u16 *)0x05000200, scale, 256);
		Graphics_ScaleRgb555Buffer(palette, (u16 *)0x05000000, scale, 256);
		WaitFrames(1);
	}
	Runtime_ReleaseHeapBlock(12);
	Runtime_ReleaseHeapBlock(45);
	Runtime_ReleaseHeapBlock(40);
	Runtime_ReleaseHeapBlock(39);
	Runtime_ReleaseHeapBlock(41);
}
#endif
