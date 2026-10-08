//--------------------------------------------------------------------------
// PUFFARC.C
// バトル効果：弧にならんで順に立ちのぼる炎
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MEM.H"
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"

void Audio_PlayCue(s32);

typedef struct Puff // work->particles と同じ置き場
{
	s32 x; // キャンバスのドット
	s32 y;
	s32 unk08;
	s32 unk0C;
	s32 unk10;
	s32 unk14;
	s32 tick; // 立ちのぼってからのフレーム（負なら待ち）
} Puff;

typedef struct
{
	s32 reserved_00[3];
	s32 current_value;
	s32 index_radius;
	s32 entry_limit;
	s32 reserved_18[3];
	s16 values[1];
} BattleEffectIndexWindow;

#define HONOO_KAZU 9 // 炎の数

//--------------------------------------------------------------------------
// 9 つの炎が半円の弧の上に 4 フレームおきに立ちのぼる（80 フレーム）
// variant 1 なら 16 ドット上にもうひとつ、variant 2 なら 32 ドット上にも
// 対象は 16 フレーム目から 8 フレームおきに当たる
//--------------------------------------------------------------------------
void BattleFx_RunPuffArc(struct BattleEffectArgument *efx)
{
	u32						*heap;
	u32						*cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw;
	s32						 i;
	s32						 cell;
	u32						 w;
	s32						 sign;
	s32						 org;
	s32						 angle;
	s32						 tick;
	Puff					*puff;
	Puff					*p;

	// ワーク、キャンバス、BG1・BG2 を半透明に（14:16）
	heap = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = efx;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDCNT = 0x3f46;
	REG_BLDALPHA = 0x100e;
	Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];

	// 弧：-90 度から 22.5 度ずつ、横の半径 32、縦の半径 16
	// 対象が 128 番以上なら左から、ほかは右から並べる
	if (work->effect->actors[0] > 127)
	{
		org = 0;
		sign = 1;
	}
	else
	{
		org = 64;
		sign = -1;
	}
	angle = -0x4000;
	i = 0;
	tick = 0;
	puff = (Puff *)work->particles;
	do
	{
		puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
		puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
		puff->tick = tick;
		angle += 0x1000;
		tick -= 4;
		i += 1;
		puff++;
	} while (i != HONOO_KAZU);

	work->transfer_mode = 2;
	if (work->effect->variant == 2)
	{
		work->transfer_value = 75;
	}
	else
	{
		work->transfer_value = 50;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	Audio_PlayCue(136);
	frame = 0;
	do
	{
		if (frame == 24)
		{
			BattleEventRuntime_BeginPhaseFar(133);
		}

		// 炎の絵は 4 フレームごとに次へ、6 枚で消える
		i = 0;
		p = (Puff *)work->particles;
		do
		{
			if ((p->tick >= 0) && (p->tick <= 23))
			{
				cell = p->tick / 4;
				draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
					p->x - ((w = PuffArc_CellWidths[cell]) >> 1), p->y + PuffArc_CellBiasY[cell], w,
					PuffArc_CellHeights[cell]);
				if (work->effect->variant != 0)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 16, w, PuffArc_CellHeights[cell]);
				}
				if (work->effect->variant == 2)
				{
					draw(canvas, work->sheet + PuffArc_CellSourceOffsets[cell],
						p->x - ((w = PuffArc_CellWidths[cell]) >> 1),
						(p->y + PuffArc_CellBiasY[cell]) - 32, w, PuffArc_CellHeights[cell]);
				}
			}
			i += 1;
			p->tick += 1;
			p++;
		} while (i != HONOO_KAZU);

		i = 0;
		while (i != work->effect->count)
		{
			if (frame == (i * 8) + 16)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, i, 12);
			}
			i += 1;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame += 1;
	} while (frame != 80);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// values のうち current_value のある位置を中心に、前後 index_radius の
// 番号を out に並べ、その数を返す。0〜entry_limit - 1 の番号だけを入れる
//--------------------------------------------------------------------------
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *win, s16 *out)
{
	s32 len = win->index_radius * 2 + 1;
	s32 n = 0;
	s32 center = 0;
	s32 i;

	for (i = 0; i != len; i++)
	{
		if (win->current_value == win->values[i])
		{
			center = i;
			break;
		}
	}
	for (i = 0; i != len; i++)
	{
		s32 no = center + i - win->index_radius;

		if (no >= 0 && no < win->entry_limit)
		{
			out[n++] = no;
		}
	}
	return n;
}
