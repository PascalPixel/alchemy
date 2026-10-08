//--------------------------------------------------------------------------
// COLUMNS.C
// バトル演出：地面から次々に伸びるつる・柱・とげと土けむり
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
#include "SCENE.H"
#include "BATTLE_EFX.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

extern u8 gMapCellBuffer[];

// 伸びる 1 本（struct EffectStep に重ねて使う）
struct DualColumn
{
	s32 x; // 根もと（キャンバスのドット）
	s32 y;
	u8	unknown_08[8];
	s32 height; // 伸びきったときの長さ
	u8	unknown_14[4];
	s32 start; // 伸び始めるフレーム
};

// 土けむり 1 つ（gMapCellBuffer に並べる）
struct DualPuff
{
	s32 x;
	s32 y;
	u8	unknown_08[16];
	s32 age; // 経過フレーム（-1 は空き）
};

void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);
void AudioCommand_PlayFar(s32);

#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer) // とげの土けむり

void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 0);
}

void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunDualTable(effect, 1);
}

//--------------------------------------------------------------------------
// つる（mode 0）か緑の柱（mode 1）が、左右交互に 4 フレームおきに伸びる
// 本数は DualTable_ColumnCounts[variant]、根もとから土けむりが上がる
//--------------------------------------------------------------------------
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*rectangle[2];
	void					*second_slot;

	s32	   frame;
	s32	   duration;
	void **draw;
	s32	   cue_frame;
	s32	   fade_start;
	s32	   fade_end;
	s32	   start;

	u8 *age;

	struct DualColumn *seed;
	s32				   x;
	s32				   height;
	s32				   limit;

	s32				   puff_cells;
	struct DualColumn *column;
	s32				   length;
	s32				   reach;
	s8				   pillar_height;

	s32 n;
	s32 react_frame;

	s32				 i;
	struct DualPuff *puff;
	s32				 puff_age;
	s32				 image;

	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	if (mode == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
	}
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	rectangle[0] = gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	second_slot = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	/* FAKEMATCH: needed to match: the columns draw through a second pointer to the pair, whose address the frame keeps in a stack slot */
	draw = rectangle;
	draw[1] = second_slot;

	duration = DualTable_ColumnCounts[work->effect->variant] * 4 + 56;

	// 土けむりをすべて空きにする
	age = gMapCellBuffer + 0x18;
	i = 0;
	do
	{
		i++;
		*(s32 *)age = -1;
		age += sizeof(struct DualPuff);
	} while (i != 1024);

	// 16 本の根もとと長さ
	i = 0;
	seed = (struct DualColumn *)work->particles;
	do
	{
		x = (DualTable_ColumnX[i] + (7 & Random16())) - 4;
		seed->y = i / 2 + 108;
		seed->x = x;
		height = (63 & Random16()) + 55;
		seed->height = height;
		limit = DualTable_HeightLimits[i % 3];
		if (limit < height)
		{
			seed->height = limit;
		}
		limit = i * 4 + 8;
		seed->start = limit;
		i++;
		seed++;
	} while (i != 16);

	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	frame = 0;
	if (duration != 0)
	{
		cue_frame = duration - 64;
		fade_start = duration - 20;
		fade_end = duration - 4;
		do
		{
			if (frame == cue_frame)
			{
				BattleEventRuntime_BeginPhaseFar(132);
			}
			if (frame >= fade_start && frame < fade_end)
			{
				// 終わりの 16 フレームで薄くする
				REG_BLDCNT = 0x3f44;
				REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
			}
			if (frame < fade_end)
			{
				i = 0;
				if (DualTable_ColumnCounts[work->effect->variant] != 0)
				{
					do
					{
						column = (struct DualColumn *)((u8 *)work->particles
													   + i * sizeof(struct DualColumn));
						start = i * 4 + 8;
						if (frame == i * 4 + 9)
						{
							work->shake_frames = 2;
						}

						// 1 フレームに 8 ドットずつ伸ばす（奇数番は左右反転）
						if (frame > start)
						{
							u32 shape = i % 3;

							length = (frame - start) * 8;
							reach = column->height;
							if (length > reach)
							{
								length = reach;
							}
							if (mode == 0)
							{
								{
									s32 side = 1 & i;
									s32 cell = DualTable_VineCells[shape];
									shape = DualTable_VineWidths[shape];
									((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
							else
							{
								pillar_height = DualTable_PillarHeights[7 & i];
								if (length > pillar_height)
								{
									length = pillar_height;
								}
								{
									s32 side = 1 & i;
									s32 cell = DualTable_PillarCells[shape];
									shape = DualTable_PillarWidths[shape];
									((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
										column->x - (shape >> 1), column->y - length, shape,
										length);
								}
							}
						}

						// 伸び始めて 4 フレーム目に対象が反応する
						n = 0;
						if (work->effect->count != 0)
						{
							react_frame = start + 4;
							do
							{
								if (frame == react_frame)
								{
									if (!(i & 1))
									{
										AudioCommand_PlayFar(133);
									}
									ObjectGroup_UpdateMembers(work->effect->actors[n], 7, 5, n, 3);
								}
								n++;
							} while (n != work->effect->count);
						}
						else
						{
							react_frame = start + 4;
						}

						// 4・8 フレーム目に、空きの土けむりをひとつ根もとに出す
						if ((frame == react_frame) || (frame == (start + 8)))
						{
							/* FAKEMATCH: needed to match: the scan reuses length as its puff pointer */
							length = (u32)gMapCellBuffer;
							n = 0;
							while (n != 512)
							{
								if (((struct DualPuff *)length)->age == -1)
								{
									((struct DualPuff *)length)->x =
										((Random16() & 0xf) + column->x) - 8;
									((struct DualPuff *)length)->y = (Random16() & 0xf) + 80;
									((struct DualPuff *)length)->age = 0;
									break;
								}
								length += sizeof(struct DualPuff);
								n++;
							}
						}
						i++;
					} while (i != DualTable_ColumnCounts[work->effect->variant]);
				}
			}

			// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
			i = 0;
			puff = (struct DualPuff *)gMapCellBuffer;
			do
			{
				puff_age = puff->age;
				if (puff_age >= 0)
				{
					image = puff_age / 2;
					puff_cells = 0x1e59;
					if (mode != 0)
					{
						puff_cells = 0xaff;
					}
					((DrawRectangleFn)rectangle[0])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells),
						puff->x - DualTable_PuffWidths[image],
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					((DrawRectangleFn)rectangle[1])(canvas,
						work->sheet + (DualTable_PuffCells[image] + puff_cells), puff->x,
						puff->y - (s8)DualTable_PuffHeights[image] / 2, DualTable_PuffWidths[image],
						(s8)DualTable_PuffHeights[image]);
					puff->age++;
					if (puff->age == 14)
					{
						puff->age = -1;
					}
				}
				puff++;
				i++;
			} while (i != 512);

			Camera_ApplyShake(4, 4);
			ObjectGroup_TickMemberTimers();
			work->transfer_pending = 1;
			WaitFrames(1);
			frame++;
		} while (frame != duration);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 対象の列のまん中から、緑のつるが 16 本、2 フレームおきに伸びて縮む
//--------------------------------------------------------------------------
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect)
{
	u32						*cache, *entry;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	s32						 origin_x, origin_y;
	struct EffectPosition	 first, last;
	s32						 i, frame, cell, height, ofs, middle;
	struct EffectStep		*column;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	entry = cache;
	work = (struct BattleEffectWork *)*entry++;
	canvas = (void *)*entry;
	work->effect = effect;
	BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1, 1);

	// 最初と最後の対象のまん中を、キャンバスの x = 64 に合わせる
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
	middle = first.x;
	middle += (last.x - middle) / 2;
	first.x = middle;
	REG_BG2X = (64 - first.x) << 8;
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	column = work->particles;
	i = 0;
	do
	{
		column[i].x = RisingColumns_ColumnOffsets[i] + 64;
		i++;
	} while (i != 16);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	frame = 0;
	do
	{
		if (frame == 32)
		{
			AudioCommand_PlayFar(143);
			for (i = 0; i != work->effect->count; i++)
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 16);
		}
		i = 0;
		column = work->particles;
		do
		{
			if (frame == i * 4 + 5)
				work->shake_frames = 2;

			// 1 フレームに 4 ドット伸びて 32 で止まり、i * 2 + 32 フレーム目から縮む
			ofs = i * 2;
			if (frame > ofs + 4)
			{
				cell = (frame / 4 + i) % 5; // 32x32 を 5 コマ
				if (frame < ofs + 32)
				{
					height = (frame - ofs) * 4 - 16;
					if (height > 32)
						height = 32;
				}
				else
				{
					height = 160 - (frame - ofs) * 4;
				}
				if (height > 0)
					draw[i & 1](canvas, work->sheet + (cell << 10), column->x - 16,
						(i & 7) - height + 104, 32, height);
			}
			i++;
			column++;
		} while (i != 16);
		Camera_ApplyShake(4, 4);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
		frame++;
	} while (frame != 70);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// とげが 8 フレームおきに地面から突き出し、根もとから土けむりが上がる
// 本数は BattleFxPillar_Counts[variant]、向きと大きさは BattleFxPillar_Kinds
//--------------------------------------------------------------------------
void BattleFx_RaisePillars(struct BattleEffectArgument *effect)
{
	void					*canvas;
	s32						 frame;
	DrawRectangle			 draw_flip;
	DrawRectangle			 draw;
	s32						 total;
	u32						*cache;
	u32						*cursor;
	struct BattleEffectWork *work;
	s32						 i;
	s32						 j;
	s32						 age;
	s32						 start;

	cache = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	cursor = cache;
	work = (struct BattleEffectWork *)*cursor++;
	canvas = (void *)*cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 横も等倍
	REG_BLDCNT = 0;
	REG_BLDALPHA = 0x1010;
	Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000; // 112 ドット右へ
	}
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	draw_flip = ((DrawRectangle *)cache)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

	for (i = 0; i != 1024; i++)
	{
		PUFFS[i].variant = -1; // 空き
	}

	for (frame = 0; frame != total; frame++)
	{
		if (frame == total - 64)
		{
			BattleEventRuntime_BeginPhaseFar(132);
		}
		if (frame >= total - 16)
		{
			// 終わりの 16 フレームで薄くする
			REG_BLDCNT = 0x3f44;
			REG_BLDALPHA = (total - frame - 1) | 0x1000;
		}

		for (i = 0; i != BattleFxPillar_Counts[work->effect->variant]; i++)
		{
			start = i * 8 + 8;
			age = frame - (i * 8 + 8);
			if (frame > start)
			{
				s32 h;
				s32 w;

				// 種類 0・1 は大きいとげ（48 幅）、2・3 は小さいとげ（32 幅）
				// 奇数は左右反転で、伸びるにつれて横へずれる
				if (BattleFxPillar_Kinds[i] < 2)
				{
					h = age * 16;
					w = age * 6;
					if (h > 80)
					{
						h = 80;
					}
					if (w > 30)
					{
						w = 30;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(canvas, work->sheet, BattleFxPillar_X[i] - w, 108 - h, 48, h);
					}
					else
					{
						draw(canvas, work->sheet, BattleFxPillar_X[i] + w, 108 - h, 48, h);
					}
				}
				else
				{
					h = age * 8;
					w = age;
					if (h > 64)
					{
						h = 64;
					}
					if (age > 8)
					{
						w = 8;
					}
					if (BattleFxPillar_Kinds[i] & 1)
					{
						draw_flip(
							canvas, work->sheet + 0xf00, BattleFxPillar_X[i] - w, 108 - h, 32, h);
					}
					else
					{
						draw(canvas, work->sheet + 0xf00, BattleFxPillar_X[i] + w, 108 - h, 32, h);
					}
				}
				if (frame == start + 1)
				{
					work->shake_frames = 3;
				}

				// 突き出して 2 フレームの間、毎フレーム土けむりをひとつ出す
				if (frame < start + 3)
				{
					s32				   y = (Random16() & 31) + 72;
					struct EffectStep *p;
					for (j = 0; j != 64; j++)
					{
						p = &PUFFS[j];
						if (p->variant == -1)
						{
							p->x = BattleFxPillar_X[i] + (Random16() & 31) + 32;
							if (p->x > 96)
							{
								p->x = 96;
							}
							p->y = y;
							p->variant = 0;
							break;
						}
					}
				}
			}
			for (j = 0; j != work->effect->count; j++)
			{
				if (frame == start + 4)
				{
					AudioCommand_PlayFar(132);
					ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 3);
				}
			}
		}

		// 土けむり：7 コマを 2 フレームずつ、左右対称に描く
		for (i = 0; i != 64; i++)
		{
			if (PUFFS[i].variant >= 0)
			{
				s32 n = PUFFS[i].variant / 2;
				draw(canvas, work->sheet + BattleFxPillar_PuffCells[n],
					PUFFS[i].x - BattleFxPillar_PuffWidths[n],
					PUFFS[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				draw_flip(canvas, work->sheet + BattleFxPillar_PuffCells[n], PUFFS[i].x,
					PUFFS[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2,
					BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
				if (++PUFFS[i].variant == 14)
				{
					PUFFS[i].variant = -1;
				}
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
