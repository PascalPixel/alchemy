//--------------------------------------------------------------------------
// FLIPPING_BURST.C
// バトル演出：逆さに跳ぶふたりと閃光、剣の雨、柱とらせんの粒、割れて飛ぶ絵
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
#include "BTLDISP.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "TRANSFORM.H"
#include "BTLWORK.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

//--------------------------------------------------------------------------
// 使い手を最初の対象との間の 9/10 の位置に置き、ふたりで跳んで逆さになる
// 54 フレーム目に対象の色を変えてふたりを跳ね返らせ、閃光を輪に広げながら昇らせる
//--------------------------------------------------------------------------
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	/* FAKEMATCH: needed to match: an unused second blitter word fills the frame's 8-byte draw slot */
	DrawRectangleFn		  draw[2];
	struct BtlObj		 *first;
	struct BtlObj		 *second;
	struct MotionObject	 *user;
	struct MotionObject	 *target;
	s32					  x;
	s32					  z;
	s32					  user_height;
	s32					  target_height;
	s32					  point[3];
	struct EffectPosition screen;
	s32					  frame;
	s32					  percent;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	draw[0] = heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	first = GetBattleObjectSlotFar(work->effect->actor);
	second = GetBattleObjectSlotFar(work->effect->actors[0]);
	user = first->object;
	target = second->object;
	percent = 90;
	x = user->x + percent * (target->x - user->x) / 100;
	z = user->z + percent * (target->z - user->z) / 100;
	user_height = Battle_GetObjectTableValueFar(work->effect->actor);
	target_height = Battle_GetObjectTableValueFar(work->effect->actors[0]);
	Object_ResetMotion(user);
	Object_SetPosition(user, x, 0, z);
	Object_SetMode(user, 2);
	user->snap_to_target = 1;
	user->auto_face_motion = 1;
	user->acceleration = 2 << 16;
	user->speed_limit = 8 << 16;
	WaitFrames(20);
	for (frame = 0; frame != 96; frame++)
	{
		struct BattleCamera *camera;

		camera = gCameraWork;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 0：ふたりで跳ぶ、11：逆さにして体の高さだけ持ち上げる
		if (frame == 0)
		{
			target->velocity_y = 15 << 16;
			target->vertical_motion_strength = 0x91eb;
			user->velocity_y = 15 << 16;
			user->vertical_motion_strength = 0x91eb;
		}
		if (frame == 11)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y += user_height;
			target->y += target_height;
		}

		// 54：対象の色を変え、ふたりを跳ね返らせる
		if (frame == 54)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 10);
			target->velocity_y = 8 << 16;
			target->vertical_motion_strength = 0xab85;
			user->velocity_y = 5 << 16;
			user->vertical_motion_strength = 0x7851;
			user->acceleration = 1 << 16;
			user->speed_limit = 2 << 16;
			user->auto_face_motion = 0;
			Object_ResetMotion(user);
			Object_SetPosition(user, 0, 0, user->z);
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		point[0] = user->x;
		point[1] = user->y;
		point[2] = user->z;
		EffectPosition_ApplyBaseAndYOffset(point, &screen);
		screen.x >>= 1;

		// 54〜55：大きな閃光、56〜67：16 個の閃光が半径を広げながら昇る
		if (frame == 54 || frame == 55)
			draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
		if (frame >= 56 && frame <= 67)
		{
			s32 i;

			for (i = 0; i != 16; i++)
			{
				s32 angle;
				s32 rx;
				s32 ry;

				angle = i << 12; // 22.5° ずつ
				rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
				ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
				draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
			}
		}

		// 64：向きと高さを元に戻す
		if (frame == 64)
		{
			target->scale_y = -target->scale_y;
			user->scale_y = -user->scale_y;
			user->y -= user_height;
			target->y -= target_height;
			Object_SetMode(user, 0);
		}
		if (frame == 54)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 0)
		{
			Audio_PlayCue(136);
			work->shake_frames = 6;
		}
		if (frame == 53)
			work->shake_frames = 6;
		Camera_ApplyShake(16, 16);
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 剣の雨：16 本の剣が 4 フレームおきに降り、中央 (x = 64) へ寄りながら落ちて刺さる
// 刺さった剣は下 4 ドットを描かない。画面は 16 フレームで現れ、104 から消える
//--------------------------------------------------------------------------
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 pos;
	DrawRectangle			 draw[2];
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			*blit;
	struct EffectStep		*blade;
	s32						 frame;
	s32						 i;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	REG_BG2PA = 0x100; // 等倍
	REG_BLDALPHA = 0x1000;

	// [0] そのまま、[1] 左右反転 (どちらも 0 以外の画素だけ描く)
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	draw[0] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
		GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_NURI);
	draw[1] = (DrawRectangle)heap_cache[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
	blit = draw;
	Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 剣：x 16〜111、中央から遠いほど横の速さ (2 ドット単位) を大きくする
	for (i = 0; i != 64; i++)
	{
		s32 column;

		blade = &work->particles[i];
		column = Random16() % 96 + 16;
		blade->x = column;
		blade->y = (24 - i / 4) << 16;
		if (column <= 43)
			blade->velocity_x = 3;
		else if (column <= 51)
			blade->velocity_x = 2;
		else if (column <= 59)
			blade->velocity_x = 1;
		else if (column <= 67)
			blade->velocity_x = 0;
		else if (column <= 75)
			blade->velocity_x = -1;
		else if (column <= 83)
			blade->velocity_x = -2;
		else
			blade->velocity_x = -3;
		blade->velocity_x <<= 17;
		blade->velocity_y = 8 << 16;
		blade->x <<= 16;
	}
	Audio_PlayCue(212);
	for (frame = 0; frame != 120; frame++)
	{
		if (frame <= 16)
		{
			REG_BLDALPHA = frame | 0x1000;
			if (frame == 16)
				REG_BLDCNT = 0;
		}
		if (frame > 103)
		{
			REG_BLDALPHA = (120 - frame) | 0x1000;
			if (frame == 104)
				REG_BLDCNT = 0x3f44;
		}

		// 絵は横の速さで選び、左へ進む剣は左右反転して描く
		for (i = 15; i != -1; i--)
		{
			/* FAKEMATCH: the drift is pinned to r6, as the reference keeps it. */
			register s32 drift __asm__("r6");
			s32			 speed;
			s32			 cell;
			u32			 wide;
			u32			 high;

			blade = &work->particles[i];
			drift = blade->velocity_x;
			speed = drift;
			if (drift < 0)
				speed = -drift;
			cell = speed >> 17;
			if (frame < i * 4 + 25)
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide, high);
				if (frame >= i * 4 + 16)
				{
					blade->x += blade->velocity_x;
					blade->y += blade->velocity_y;
				}
			}
			else
			{
				blit[(u32)drift >> 31](canvas, work->sheet + BladeRain_CellSourceOffsets[cell],
					(blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
					(blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1), wide,
					high -= 4);
			}
		}

		// 24〜84 フレームの 4 フレームごとに対象のパレットを 2 フレーム 7 番に替えて
		// アニメ 5 にし、画面を揺らし、8 フレームごとに音を鳴らす
		if (frame >= 23 && frame <= 87 && (frame & 3) == 0)
		{
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
			work->shake_frames = 1;
			if ((frame & 7) == 0)
				Audio_PlayCue(133);
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

#define FLECKS ((struct EffectStep *)Ram_MapCellBuffer) // らせんの粒 (128 個)

void AudioCommand_PlayFar(s32 value);

struct BlitterPair
{
	DrawRectangle kasan;  // 加算合成
	DrawRectangle hikaku; // 比較合成
};

//--------------------------------------------------------------------------
// 使い手の足元に左右対称の柱 (IceTile の絵) を立て、周りにらせんを描いて昇る粒を出す
// effect->kind	   : パレット (0 YellowPaletteA、1 IceTile、2 RedPaletteA、他 VioletPaletteA)
// effect->variant : 1 足して 1〜4 に収め、粒の大きさにする
//--------------------------------------------------------------------------
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect)
{
	struct EffectPosition	 ground;
	struct EffectPosition	 pos;
	s32						 point[3];
	struct BattleEffectWork *work;
	void					*canvas;
	struct BlitterPair		 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 frame;
	void					*palette;
	s32						 i;

	work =
		(struct BattleEffectWork *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
	camera = gCameraWork;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	work->fade_frames = 24;
	work->fade_step = 0;
	REG_BLDALPHA = 0x100c;
	REG_BG2PA = 0x100; // 等倍
	Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
	switch (work->effect->kind)
	{
	case 0:
		palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
		break;
	case 1:
		palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
		break;
	case 2:
		palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
		break;
	default:
		palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
		break;
	}
	{
		/* FAKEMATCH: needed to match: called through a pointer, the routine's address loads before the palette's */
		s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

		copy((void *)BG_PLTT, palette, 128);
	}

	// 粒：x は角度、z は半径 (1/16 ドット単位)、i フレーム待ってから昇る
	for (i = 0; i != 128; i++)
	{
		FLECKS[i].y = 0;
		FLECKS[i].x = Random16() & 0xffff;
		FLECKS[i].z = (Random16() & 0x1ff) + i * 2;
		FLECKS[i].variant = -i;
	}
	/* FAKEMATCH: read-only work use exchanges r11/r9, fixing 16 register differences. */
	__asm__("" : : "r"(work));
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw.kasan = gWorkSlot[HEAP_SLOT_BLITTER];
	work->effect->variant++;
	if (work->effect->variant <= 0)
		work->effect->variant = 1;
	if (work->effect->variant > 4)
		work->effect->variant = 4;
	AudioCommand_PlayFar(212);

	for (frame = 0; frame != 56; frame++)
	{
		EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &ground);
		REG_BG2X = (64 - ground.x) << 8;
		if (frame > 49)
			REG_BLDALPHA = (112 - frame * 2) | 0x1000;
		if (frame == 16)
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);

		// 柱：左半分 (x = 47) と左右反転した右半分 (x = 64) を比較合成で描く
		// 柱の絵 (17×64) は 2 フレームごとに 4 枚、光 (24×43) は 4 フレームごとに 3 枚
		if (frame <= 55)
		{
			s32 n;
			u8 *glow;

			i = frame / 2;
			n = i % 4;
			BattleEffect_LoadWork(
				HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
			draw.hikaku = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			draw.hikaku(canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
			n = frame / 4 % 3;
			glow = work->sheet + n * 1032 + 0x1100;
			draw.hikaku(canvas, glow, 40, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7,
				GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_HIKAKU);
			draw.hikaku = ((DrawRectangle *)Ram_WorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
			n = i % 4;
			draw.hikaku(canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
			draw.hikaku(canvas, glow, 64, ground.y - 36, 24, 43);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		}
		GetBattleObjectSlotFar(work->effect->actor);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

		// 粒 (最初の 32 個)：1 フレームに 0x400 回り、5 ドット昇り、半径を 4 ドット広げる
		for (i = 0; i != 32; i++)
		{
			struct EffectStep *fleck = &FLECKS[i];

			if (fleck->variant >= 0)
			{
				s32 size;

				point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
				point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
				point[1] = fleck->y;
				fleck->x += 0x400;
				fleck->y += 5 << 16;
				fleck->z += 64;
				EffectPosition_ApplyBaseAndYOffset(point, &pos);
				pos.x /= 2;
				size = (i & 1) + work->effect->variant;
				draw.kasan(canvas, sheet + BattleFx6_FlareCells[size - 1], pos.x - size,
					pos.y - size, size * 2, size * 2);
			}
			fleck->variant++;
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
	/* FAKEMATCH: a scheduling barrier the native order needs. */
	__asm__("");
}

// 割れて飛ぶ 33 片：始まりの位置、大きさ、絵の位置
extern u8  BattlePieceStartX[];
extern u8  BattlePieceStartY[];
extern u8  BattlePieceWidth[];
extern u8  BattlePieceHeight[];
extern u16 BattlePieceOffset[];

// 256×120 の 8bpp タイル並び (横 32 タイル) の点 (x, y) に色 2 を置く
#define PLOT(x, y) \
	(*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + (y) / 8 * 2048) = 2)

//--------------------------------------------------------------------------
// 中心 (96, 60) から灰色の円が 1 フレームに半径 32 ずつ広がり、
// TileAnimationMask の絵を 33 片に割って飛ばしながら、パレット 6〜13 を
// 白から戦闘背景の色へ戻す。51 フレームからは閃光 32 個が昇る
//--------------------------------------------------------------------------
void BattleEffect_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect)
{
	/* FAKEMATCH: needed to match: never read: the native frame keeps 12 unused bytes at sp+44..55, exactly this struct */
	struct EffectPosition		 position;
	struct BattleEffectWork		*work;
	void						*canvas;
	s32							 frame;
	DrawRectangle				 draw1;
	DrawRectangle				 draw0;
	struct BtlWork				*battle;
	struct BattleBackgroundView *view;
	void					   **slots;
	void					   **cache;
	s32							 rmax;
	struct EffectStep			*piece;
	s32							 r;
	s32							 i;
	s32							 px;
	s32							 j;
	s32							 k;
	s32							 d;
	s32							 py;
	s32							 x1;
	s32							 size;
	s32							 left;
	s32							 left2;
	s32							 y1;
	s32							 x2;
	s32							 y2;
	u8							*start_y;
	u8							*start_x;
	s32							 cx;
	s32							 cy;
	s32							 cell_size;
	u8							*cells;
	u32							 cell_fill;

	work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
	size = 0x4000;
	canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, size);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	slots = gWorkSlot + HEAP_SLOT_BATTLE;
	battle = slots[0];
	view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
	work->effect = effect;

	// BG2 を 128×128 のキャンバスにする (BattleFx_BeginCanvasLayer と同じ設定)
	BattleFx_ClearActorTimers();
	view->second_mode = 1;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
	REG_BG2CNT = 0x784;
	BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
	view->second_mode = 0;
	REG_BG2X = 0;
	REG_BG2Y = 0xfffff000;
	REG_BG2PA = 0x80;
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = 0x100;
	REG_WIN0H = 0xf0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0xf0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;

	i = 0;
	for (k = 0; k != 16; k++)
	{
		for (j = 0; j != 8; j++, i += 2)
		{
			s16 entry;

			entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
			*(volatile s16 *)(0x06003800 + i) = entry;
		}
	}

	Iwram_ClearWords(canvas, size);
	Iwram_ClearWords((void *)0x06004000, size);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work, 1, 1);
	work->transfer_mode = 1; // 転送のたびにキャンバスを 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_NURI);
	cache = gWorkSlot;
	draw0 = cache[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(
		HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw1 = cache[HEAP_SLOT_BLITTER_ALTERNATE];

	// 片：中心 (32, 60) から外へ、距離の 1/4 の速さで飛ぶ
	/* FAKEMATCH: needed to match: the copy and fill operands set here, before the piece loop, so CSE does not share them and the pair reloads both as the native does */
	cell_size = 0x7800;
	cells = Ram_MapCellBuffer;
	cell_fill = 0x01010101;
	start_y = BattlePieceStartY;
	start_x = BattlePieceStartX;
	r = 0;
	piece = work->particles;
	for (i = 0; i != 33; i++)
	{
		s32 x, y, vx, vy;

		x = *start_x++ << 16;
		y = *start_y++ << 16;
		piece->x = x;
		piece->y = y;
		vx = (x - 0x200000) >> 2;
		vy = (y - 0x3c0000) >> 2;
		piece->velocity_x = vx;
		piece->velocity_y = vy;
		piece++;
	}

	// VRAM 0x06008000 の絵を写してから色 1 で埋め、パレット 6〜13 を白にする
	Iwram_CopyWords(cells, (void *)0x06008000, cell_size);
	Iwram_FillWords(cells, cell_size, cell_fill);
	view->busy = 1;
	work->saved_bg1_x = gBgScroll[1].x;
	work->saved_bg1_y = gBgScroll[1].y;
	gBgScroll[1].x = 0;
	IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
	Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
	/* FAKEMATCH: r5 clobber prunes draw1's r5 preference (its load address is in r5); without it the piece constants swap r5/r7 and the fill routine takes r6, 173/1052. */
	asm volatile("" ::: "r5");
	Audio_PlayCue(212);
	ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 3, 0, 30);

	for (frame = 0; frame != 128; frame++)
	{
		if (frame == 2)
			Audio_PlayCue(212);
		if (frame == 3)
			Audio_PlayCue(212);
		if (frame == 28)
			ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 3, -1, 0);
		if (frame == 32)
			Audio_PlayCue(149);
		if (frame == 5)
		{
			Audio_PlayCue(145);
			/* FAKEMATCH: barrier; sched2 otherwise loads 0x77a0 before reloading work, 2/1051. */
			asm("");
			gBgScroll[1].x = work->saved_bg1_x;
			BattleBackground_LoadFar(1, battle->background, -1);
		}

		// 8 フレームから、パレット 6〜13 の各色を 1 段ずつ戦闘背景の色へ近づける
		if (frame > 7)
		{
			u16 *pal;

			pal = (u16 *)0x050000c0;
			for (i = 0; i != 128; i++)
			{
				s32 red = *pal & 0x1f;
				s32 green = (*pal >> 5) & 0x1f;
				s32 blue = (*pal >> 10) & 0x1f;
				s32 goal_red = battle->palette[i] & 0x1f;
				s32 goal_green = (battle->palette[i] >> 5) & 0x1f;
				s32 goal_blue = (battle->palette[i] >> 10) & 0x1f;

				if (red < goal_red)
					red++;
				else if (red > goal_red)
					red--;
				if (green < goal_green)
					green++;
				else if (green > goal_green)
					green--;
				if (blue < goal_blue)
					blue++;
				else if (blue > goal_blue)
					blue--;
				*pal++ = (blue << 10) | (green << 5) | red;
			}
		}

		if (frame == 4)
			Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

		// 0〜3 フレーム：円を半径 32 ずつ広げ、色 2 の灰色を明るくしていく
		if (frame <= 3)
		{
			s32 v;

			v = frame * 4 + 8;
			rmax = frame * 32;
			BG_PLTT[2] = (v << 10) | (v << 5) | v;
			cx = 96;
			cy = 60;
			for (; r != rmax; r++)
			{
				// 中点円の描き方で、8 分の 1 ずつ対称に点を置く
				px = r;
				py = 0;
				d = r;
				while (px >= py)
				{
					x2 = px + cx;
					x1 = cx - px;
					left = x1;
					y1 = cy - py;
					y2 = py + cy;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left + 1;
					x2 = px + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					x2 = py + cx;
					x1 = cx - py;
					left2 = x1;
					y2 = px + cy;
					y1 = cy - px;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					if (y1 < 0)
						y1 = 0;
					if (y2 > 119)
						y2 = 119;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = left2 + 1;
					x2 = py + 97;
					if (x1 < 0)
						x1 = 0;
					if (x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					d -= py * 2 + 1;
					if (d < 0)
					{
						d += px * 2 - 2;
						px--;
					}
					py++;
				}
			}
			Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
		}

		// 50 フレームまで片を描き、4 フレームから落とす
		if (frame <= 50)
		{
			for (i = 0; i != 33; i++)
			{
				piece = &work->particles[i];
				draw0(canvas, (u8 *)work + BattlePieceOffset[i], HI(piece->x), HI(piece->y),
					BattlePieceWidth[i], BattlePieceHeight[i]);
				if (frame > 3)
					EffectStep_AdvanceWithGravity2D(piece, 64, 0x4000);
			}
		}

		// 8〜50 フレーム：色 1 を黒から白へ
		if (frame >= 8 && frame <= 50)
		{
			s32 v;

			v = frame - 8;
			if (v > 31)
				v = 31;
			BG_PLTT[1] = (v << 10) | (v << 5) | v;
		}

		// 51 フレーム：閃光の絵と色を用意し、x 32〜63、y 80〜111 に 32 個を置く
		if (frame == 51)
		{
			Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 0);
			for (i = 1; i != 64; i++)
			{
				s32 v;

				v = i / 2;
				if (v < 0)
					v = 0;
				BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
			}
			REG_BLDCNT = 0x3f44;
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				piece->x = ((Random16() & 31) + 32) << 16;
				piece->y = ((Random16() & 31) + 80) << 16;
				piece->velocity_x = ((Random16() & 0x1ff) - 0x100) << 12;
				piece->velocity_y = 0;
				piece->variant = 0;
			}
			work->transfer_mode = 2;
			work->transfer_value = 50; // 転送のたびに半分へ減らして残像にする
		}

		// 閃光：i/4 フレームずつ遅れて出て、4 フレームごとに絵を進めながら昇る
		if (frame > 52)
		{
			for (i = 0; i != 32; i++)
			{
				piece = &work->particles[i];
				if (frame >= i / 4 + 52 && piece->variant <= 39)
				{
					s32 cell;

					cell = piece->variant / 4;
					if (cell > 5)
						cell = 5;
					draw1(canvas, (u8 *)work + (cell << 11), HI(piece->x) - 16, HI(piece->y) - 32,
						32, 64);
					EffectStep_AdvanceWithGravity2D(piece, 60, -0x1000);
					piece->variant++;
				}
			}
		}

		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	// 背景と表示を元に戻す
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 1, -1, 0);
	gBgScroll[1].x = work->saved_bg1_x;
	gBgScroll[1].y = 32;
	BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
	WaitFrames(1);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}
