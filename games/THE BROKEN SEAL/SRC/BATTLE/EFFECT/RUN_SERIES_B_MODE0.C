//--------------------------------------------------------------------------
// RUN_SERIES_B_MODE0.C
// バトル演出：対象ごとに光を集め、はじけたあとに絵を出す (六種)
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
#include "RESOURCE.H"
#include "BTLDISP.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTOBJ.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "HEAP_STATE.H"
#include "TRANSFORM.H"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

#define WORK_SLOT(kind) (*(void **)((u8 *)gWorkSlot + (kind) * 4))

#define FLARES ((struct EffectStep *)Ram_MapCellBuffer) // 集まる光 (対象ごとに 64)

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 narrow, s32 *x, s32 *y);
void Audio_PlayCue(s32 cue);

//--------------------------------------------------------------------------
// mode ごとの入口
//--------------------------------------------------------------------------
void BattleFx_RunSeriesBMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 0);
}

void BattleFx_RunSeriesBMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 1);
}

void BattleFx_RunSeriesBMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 2);
}

void BattleFx_RunSeriesBMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 3);
}

void BattleFx_RunSeriesBMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 5);
}

void BattleFx_RunSeriesBMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RunSeriesB(effect, 4);
}

// sheet の中の置き場所
enum SeriesBSheet
{
	SERIES_B_FLARES = 0x0000,  // 光の粒 (ParticleSpritesD)
	SERIES_B_BURST = 0x060e,   // はじける光 40x40 が 6 枚 (GlowOrb)
	SERIES_B_PICTURE = 0x2b8e, // mode で選ぶ絵
	SERIES_B_DJINNI = 0x65c0   // ジンの絵 40x40
};

//--------------------------------------------------------------------------
// 対象ごとに 8 フレームずつずらして、まわりの光の粒を回しながら
// 中心へ集め、はじけさせてから mode の絵を描く
// キャンバスは横 1.25 倍で見せる (画面の x は 4/5 してキャンバスの x)
// effect->prelude が 1 なら、先にジンを出してその場で揺らす
// mode : 絵は 0 = 短剣 (Dagger)、1・2 = 盾 (Shield)、3・5 = 筋 (VioletStreak)、4 = Wisp
//        色は 3 が Venus、2 が Jupiter、0・1・5 が Mars の絵から (4 は変えない)
//        ジンは 3 が Venus、2・4 が Jupiter、ほかは Mars
//--------------------------------------------------------------------------
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw[2];
	s32						 member;
	s32						 shift;
	s32						 flags;
	s32						 frame;
	s32						 i;
	struct EffectPosition	 base;
	s32						 center[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	s32						 origin_x;
	s32						 origin_y;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);

	// ジンを出す (Venus は 0、Jupiter は 3、Mars は 2)。出た位置を origin に受け取る
	if (work->effect->prelude == 1)
	{
		if (mode == 3)
			BattleFx_PrepareCanvasEffect(
				effect, 0, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		else if (mode == 2 || mode == 4)
			BattleFx_PrepareCanvasEffect(
				effect, 3, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		else
			BattleFx_PrepareCanvasEffect(
				effect, 2, work->effect->side ^ 1, 1, &origin_x, &origin_y);
		origin_x = origin_x * 4 / 5;
	}
	REG_BG2PA = 0xcc; // 横 1.25 倍

	// 絵と色
	Resource_LoadAndDecompress(
		(s32)&ResourceId_ParticleSpritesD, work->sheet + SERIES_B_FLARES, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + SERIES_B_BURST, 1, 1);
	/* FAKEMATCH: forwarded through Call3, each palette copy loads its routine before it shifts the destination, as the reference does. */
	if (mode == 3 || mode == 5)
	{
		Resource_LoadAndDecompress(
			(s32)&ResourceId_VioletStreakSheet, work->sheet + SERIES_B_PICTURE, 1, 1);
		if (mode == 3)
			Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
				(s32)Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
		else
			Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
				(s32)Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
	}
	else if (mode == 4)
	{
		Resource_LoadAndDecompress(
			(s32)&ResourceId_WispSheet, work->sheet + SERIES_B_PICTURE, 1, 1);
	}
	else
	{
		void *palette;

		if (mode == 0)
			Resource_LoadAndDecompress(
				(s32)&ResourceId_DaggerSheet, work->sheet + SERIES_B_PICTURE, 1, 0);
		else
			Resource_LoadAndDecompress(
				(s32)&ResourceId_ShieldSheet, work->sheet + SERIES_B_PICTURE, 1, 0);
		if (mode == 0)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
		else if (mode == 2 || mode == 4)
			palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
		else if (mode == 1)
			palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
		else
			palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
		Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);
	}
	if (mode == 3)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_VenusDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);
	else if (mode == 2 || mode == 4)
		Resource_LoadAndDecompress(
			(s32)&ResourceId_JupiterDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);
	else
		Resource_LoadAndDecompress(
			(s32)&ResourceId_MarsDjinnSheet, work->sheet + SERIES_B_DJINNI, 1, 0);

	// 光の粒：中心から x と z は ±25 ドット、y は ±50 ドットに散らす
	for (i = 0; i != 512; i++)
	{
		struct EffectStep *flare = &FLARES[i];

		flare->x = (Random16() % 200 - 100) << 14;
		flare->y = (Random16() % 200 - 100) << 15;
		flare->z = (Random16() % 200 - 100) << 14;
		flare->variant = 0;
	}

	// 対象がひとりなら、その対象がキャンバスの中央 (64) に来るようずらす
	if (work->effect->count == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
		shift = -base.x * 4 / 5 + 64;
	}
	else
	{
		if (work->effect->side == 1)
			shift = -64;
		else
			shift = 0;
	}
	REG_BG2X = shift << 8;
	work->transfer_mode = 2;   // 転送のたびに半分へ減らして残像にする
	work->transfer_value = 50; // 1/2
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	if (work->effect->side == 1)
		flags = GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X;
	else
		flags = GOUSEI_CLIP_X | GOUSEI_CLIP_Y;
	Audio_PlayCue(142);

	for (frame = 0; frame != work->effect->count * 8 + 108; frame++)
	{
		/* FAKEMATCH: WORK_SLOT keeps the slot offset in the literal as native does; indexing gWorkSlot loads the base and adds it. */
		struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);

		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);

		// ジン：左右に揺れ、32 フレームを過ぎたら 2 ドットずつ昇る
		// はじめの 4 フレームは加算でも重ねる
		if (work->effect->prelude == 1)
		{
			s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
			s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags ^ GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags ^ GOUSEI_FLIP_X, GOUSEI_KASAN);
			draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			if (frame > 32)
				y = y - frame * 2 + 64;
			draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
			if (frame <= 3)
				draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}

		for (member = 0; member != work->effect->count; member++)
		{
			struct MotionObject *object =
				GetBattleObjectSlotFar(work->effect->actors[member])->object;
			s32 height = Battle_GetObjectTableValueFar(work->effect->actors[member]) * 2 / 3;

			if (frame == member * 8 + 80)
				Audio_PlayCue(212);
			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			// 対象の 2/3 の高さを中心にする
			point[0] = object->x;
			point[1] = height;
			point[2] = object->z;
			SceneTransform_ApplyPosition(point);
			if (frame == member * 8 + 48)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 16);

			// 光の粒を回しながら、1 フレームに 2 ドットずつ中心へ寄せる
			if (frame > member * 8)
			{
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
				draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
				BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags, GOUSEI_KASAN);
				draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
				if (mode == 0)
				{
					SceneTransform_ApplyPitch(-frame << 10);
				}
				else if (mode == 1)
				{
				}
				else if (mode == 2)
				{
					SceneTransform_ApplyYaw(frame << 10);
				}
				else
				{
					SceneTransform_ApplyYaw(frame << 10);
					SceneTransform_ApplyRoll(frame << 10);
				}
				for (i = 0; i != 32; i++)
				{
					struct EffectStep *flare = &FLARES[member * 64 + i];

					if (frame > member * 8 + i)
					{
						s32 xx = (flare->x >> 8) * (flare->x >> 8);
						s32 yy = (flare->y >> 8) * (flare->y >> 8);
						s32 zz = (flare->z >> 8) * (flare->z >> 8);
						s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

						if (distance != 0)
						{
							s32 size;

							// 大きさ (幅の半分) は奥行き 314〜634 で 6〜1
							EffectPosition_ApplyBaseAndYOffset((s32 *)flare, &screen);
							screen.x = screen.x * 4 / 5 + shift;
							if (screen.depth < 314)
								screen.depth = 314;
							if (screen.depth > 634)
								screen.depth = 634;
							size = 6 - (screen.depth - 314) / 64;
							draw[1](canvas, work->sheet + BattleFx6_FlareCells[size - 1],
								screen.x - size, screen.y - size, size * 2, size * 2);
							flare->x -= flare->x / distance;
							flare->y -= flare->y / distance;
							flare->z -= flare->z / distance;
						}
					}
				}
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			}

			// 中心の画面の位置
			center[0] = 0;
			center[1] = 0;
			center[2] = 0;
			EffectPosition_ApplyBaseAndYOffset(center, &screen);
			screen.x = screen.x * 4 / 5 + shift;

			// はじける光：52 フレーム目から 4 フレームずつ 6 枚
			if (frame >= member * 8 + 52 && frame < member * 8 + 76)
			{
				s32 cel = (frame - member * 8 - 52) / 4 % 6;

				BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
				draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
				draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST, screen.x - 20,
					screen.y - 20, 40, 40);
				Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			}

			// 80 フレーム目から mode の絵
			if (mode == 0)
			{
				// 24x40 を 4 フレームずつ 7 枚
				if (frame >= member * 8 + 80 && frame < member * 8 + 108)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 7;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE, screen.x - 12,
						screen.y - 20, 24, 40);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else if (mode == 3 || mode == 5)
			{
				// 32x64 を 4 フレームずつ 6 枚
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 16,
						screen.y - 32, 32, 64);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else if (mode == 4)
			{
				// 64x32 を 2 フレームずつ、上下を反転した下半分と合わせて 64x64
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 2 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_HIKAKU);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					BattleEffect_LoadWork(
						HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, flags | GOUSEI_FLIP_Y, GOUSEI_HIKAKU);
					draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
					draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 32,
						screen.y - 24, 64, 32);
					draw[1](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE, screen.x - 32,
						screen.y + 8, 64, 32);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
			else
			{
				// 40x40 を 4 フレームずつ 6 枚、加算で
				if (frame >= member * 8 + 80 && frame < member * 8 + 104)
				{
					s32 cel = (frame - member * 8 - 80) / 4 % 6;

					BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, flags, GOUSEI_KASAN);
					draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
					draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE, screen.x - 20,
						screen.y - 20, 40, 40);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}
