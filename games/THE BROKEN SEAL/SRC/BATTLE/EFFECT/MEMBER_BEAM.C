//--------------------------------------------------------------------------
// MEMBER_BEAM.C
// 戦闘エフェクト：ジンが現れ、光の粒が降り、空から光の柱が対象に落ちる
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
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLDISP.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "MENU_LIST.H"
#include "TRANSFORM.H"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer) // 対象ひとりに 64 個

// work->sheet の中の絵
#define PIC_MOTE 0x2710	 // 粒 (CrescentMoonSheet、24×24)
#define PIC_DJINN 0x65c0 // ジン (40×40)

typedef s32 (*WordCopy)(void *, const void *, s32);

void BattleFx_PrepareCanvasEffect(
	struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);

static __inline__ void PalCopy(WordCopy copy, void *dst, const void *src, s32 size)
{
	/* FAKEMATCH: the palette copies share the routine's address but build the palette address again at each call, which only an inlined constant argument compiles to; a direct call keeps it in a register. */
	copy(dst, src, size);
}

//--------------------------------------------------------------------------
// mode 0 : ヴィーナスのジン
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBeam(effect, 0);
}

//--------------------------------------------------------------------------
// mode 1 : マーキュリーのジン
//--------------------------------------------------------------------------
void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunMemberBeam(effect, 1);
}

//--------------------------------------------------------------------------
// prelude が 1 ならジンを出して揺らしながら上へ動かし、対象ごとに
// 16 フレームずつずらして粒を 8 つ降らせ、72 フレーム目から光の柱を落とす
// mode : 0 はヴィーナス、1 はマーキュリーのジンの絵
//--------------------------------------------------------------------------
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 one;
	struct EffectPosition	 pos;
	s32						 ground[3];
	s32						 point[3];
	s32						 djinn_x;
	s32						 djinn_y;
	DrawRectangle			 draw[2];
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 ofs_x;
	u8						*palette;
	s32						 i;
	s32						 k;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	if (work->effect->prelude == 1)
		BattleFx_PrepareCanvasEffect(
			effect, mode, work->effect->side ^ work->effect->prelude, 0, &djinn_x, &djinn_y);
	palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
	PalCopy(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_CrescentMoonSheet);
	PalCopy(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + PIC_MOTE);
	if (mode == 0)
		palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
	else
		palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
	PalCopy(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + PIC_DJINN);

	// 粒 : 80 ドットの高さから、揺れの位相はでたらめ
	for (i = 0; i != 512; i++)
	{
		gMotes[i].x = 0;
		gMotes[i].y = 80 << 16;
		gMotes[i].z = (Random16() | -32) << 14;
		gMotes[i].variant = Random16() & 255;
	}

	// 対象がひとりなら、その画面位置が x = 64 に来るよう BG2 をずらす
	REG_BG2PA = 0x100;
	if (work->effect->count == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &one);
		ofs_x = 64 - one.x;
	}
	else
	{
		if (work->effect->side == 1)
			ofs_x = -112;
		else
			ofs_x = 0;
	}
	REG_BG2X = ofs_x << 8;
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 16 + 116; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		if (frame == 64)
			AudioCommand_PlayFar(212);
		if (frame == 80)
			BattleEventRuntime_BeginPhaseFar(0);

		// ジン : 左右に 20 ドット揺れ、32 フレームのちから 2 ドットずつ上がる
		k = work->effect->prelude;
		if (k == 1)
		{
			s32 angle = frame << 11;
			s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn_x + ofs_x - 20;
			s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn_y - 24;

			BattleFx_FetchRectangleBlitters(work->effect->side ^ k, draw);
			if (frame > 32)
				y = y - frame * 2 + 64;
			draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
			if (frame <= 3)
				draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		}
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
		draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
		draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

		// 粒 : 8 フレームおきに出て、左右に 16 ドット揺れながら
		// 1 ドットずつ降り、高さ 8 ドットで消える
		for (k = 0; k != work->effect->count; k++)
		{
			struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
			s32					 start = k * 16;

			Render_ResetTransformState();
			Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
			point[0] = target->x;
			point[1] = 0;
			point[2] = target->z;
			SceneTransform_ApplyPosition(point);
			if (frame > start)
			{
				for (i = 0; i != 8; i++)
				{
					struct EffectStep *mote = &gMotes[k * 64 + i];

					if (frame > i * 8 + start && mote->y > 8 << 16)
					{
						s32 sway;

						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &pos);
						pos.x += ofs_x;
						sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
						if (i & 1)
							pos.x -= sway;
						else
							pos.x += sway;
						draw[i & 1](canvas,
							work->sheet + MemberBeam_MotePictures[mote->variant / 8 & 7] * 576
								+ PIC_MOTE,
							pos.x - 12, pos.y - 12, 24, 24);
						mote->y -= 1 << 16;
						mote->variant++;
					}
				}
			}
		}

		// 光の柱 : 画面の上から対象の足もとまで、左右対称に描く
		// 幅は 6 ドット、88 フレーム目から 3 フレームに 1 ドットずつ細る
		for (i = 0; i != work->effect->count; i++)
		{
			s32 start = i * 16;

			if (frame >= start + 72)
			{
				struct MotionObject *target =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				if (frame == start + 72)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 1, -1, -1, 0);
				if (frame == start + 88)
					ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
				ground[0] = target->x;
				ground[1] = 0;
				ground[2] = target->z;
				EffectPosition_ApplyBaseAndYOffset(ground, &pos);
				pos.x += ofs_x;
				if (frame < start + 104)
				{
					s32 pic = frame / 4;
					s32 width = 6;

					if (frame > start + 88)
						width = 6 - (frame - start - 88) / 3;
					if (pic > 2)
						pic = (pic & 1) + 1;
					// 足もとの光
					if (frame < start + 100)
					{
						draw[0](canvas, work->sheet + MemberBeam_HeadOffsets[pic],
							pos.x - MemberBeam_HeadWidths[pic],
							pos.y - MemberBeam_HeadHeights[pic] + 8, MemberBeam_HeadWidths[pic],
							MemberBeam_HeadHeights[pic]);
						draw[1](canvas, work->sheet + MemberBeam_HeadOffsets[pic], pos.x,
							pos.y - MemberBeam_HeadHeights[pic] + 8, MemberBeam_HeadWidths[pic],
							MemberBeam_HeadHeights[pic]);
					}
					// 柱 (1 行ずつ)
					for (k = 0; k != pos.y; k++)
					{
						draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
						draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
					}
				}
			}
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}
