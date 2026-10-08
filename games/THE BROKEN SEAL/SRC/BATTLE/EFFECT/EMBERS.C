//--------------------------------------------------------------------------
// EMBERS.C
// バトル演出：粒を下から昇らせる演出と、粒を噴き上げて 3 本の柱を流す演出
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
#include "IO_REG.H"
#include "RESOURCE.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "MOTOBJ.H"
#include "BTLDISP.H"
#include "RAM_BUFFER.H"
#include "TRANSFORM.H"

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)
#define Object_InitializeMode ((void (*)(void *, s32))Object_InitializeMode)

struct Point2D
{
	s32 x;
	s32 y;
};

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];

struct BattleEffectTargetArgument;
void AudioCommand_PlayFar(s32 value);

#define HI(v) (((s16 *)&(v))[1])						// 16.16 固定小数の整数部
#define GRAINS ((struct EffectStep *)Ram_MapCellBuffer) // 昇る粒 (最大 512)

typedef s32 (*WordCopy)(void *, const void *, s32);

static __inline__ void CopyPalette(WordCopy copy, void *dst, const void *src, s32 size)
{
	/* FAKEMATCH: the palette copies share the routine's address but build the palette address again at each call, which only an inlined constant argument compiles to; a direct call keeps it in a register. */
	copy(dst, src, size);
}

struct BlitterPair
{
	DrawRectangle hikaku; // 比較合成
	DrawRectangle kasan;  // 加算合成
};

//--------------------------------------------------------------------------
// 208 フレームの演出。49〜159 フレームに A か B を押すと 160 フレームへ飛ばす
// mode : 0 なら 8 体の隊列を出して動かし、最後にその位置で ImpactBurst を起こす
//        1 なら effect->side で決まる位置から火花と閃光を出し、
//        LightningBolt のパレットで描く
// 粒は 4 種類 (キャンバス座標、y は下向きが正)：
//   GRAINS[0..511]    昇る粒：y = 112 から上へ加速して昇る。33〜103 フレームは
//                     1 フレームに 1 つ、104〜175 フレームは 8 つずつ出す
//   particles[40..63] 火花：41〜127 の奇数フレームに 1 つずつ、斜め上へ飛ぶ
//   particles[0..23]  閃光：129〜175 フレームに 1 つずつ、横へ飛ぶ
//   particles[24..39] 筋：上から斜めに落ち、y が 55 を越えたら絵を繰り返す
//--------------------------------------------------------------------------
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode)
{
	struct EffectPosition	 origin;
	void				   **cursor;
	void					*canvas;
	struct BattleCamera		*camera;
	struct BattleEffectWork *work;
	struct BlitterPair		 draw;
	s32						 frame;
	u8						*sheet;
	struct Point2D			 focus;
	s32						 dir;
	u8						*palette;
	s32						 i;

	// cursor は gWorkSlot[40] (キャンバス) を指す
	cursor = (void **)(gBattleFxWork + 4);
	canvas = cursor[0];
	camera = cursor[-28]; // [12] カメラ
	work = cursor[-1];	  // [39] 作業領域
	sheet = cursor[1];	  // [41] 粒の絵
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	REG_BG2CNT = 0x784;
	BattleEffect_LoadWork(46, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw.hikaku = cursor[6];
	BattleEffect_LoadWork(47, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_KASAN);
	draw.kasan = cursor[7];

	// 閃光は FlashBurst、筋は EmberStreak (sheet + 0x3000) の絵
	palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
	CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x3000);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
	if (mode == 1)
		CopyPalette(Iwram_CopyWords, (void *)0x05000000,
			Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// mode 1 で side が 1 なら左右を逆にする
	if (mode == 0)
		dir = 1;
	else if (work->effect->side == 1)
		dir = -1;
	else
		dir = 1;
	if (mode == 1)
	{
		EffectPosition_ApplyStepAndYOffset(work->effect->actor, &origin);
		origin.x /= 2;
		origin.y = 66;
		if (work->effect->side == 1)
			origin.x = 76;
		else
			origin.x = 44;
	}

	// 隊列の中心は (212, -60) から
	focus.x = 212 << 16;
	focus.y = -(60 << 16);
	for (i = 0; i != 64; i++)
		work->particles[i].variant = -1;
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *streak = &work->particles[24 + i];

		if (dir == 1)
			streak->x = (Random16() & 127) + 128;
		else
			streak->x = (Random16() & 127) - 128;
		streak->y = (Random16() & 7) - 72;
		streak->variant = -(Random16() & 31); // 落ち始めるまでの待ち
	}
	for (i = 0; i != 512; i++)
		GRAINS[i].variant = -1;
	if (mode == 0)
	{
		BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);
		WaitFrames(1);
		BattleFx_SpawnObjects(8, 377, 2);
	}

	for (frame = 0; frame != 208; frame++)
	{
		// A か B で 160 フレームへ
		if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159)
		{
			if (mode == 0)
			{
				Object_InitializeMode(work->objects[0], 8);
				Object_InitializeMode(work->objects[1], 9);
				Object_InitializeMode(work->objects[3], 10);
				Object_InitializeMode(work->objects[4], 11);
			}
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
			}
			frame = 160;
		}
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 178)
			BattleEventRuntime_BeginPhaseFar(134);

		// 128 から残像を半分に、160〜175 はキャンバスを塗りつぶしてパレットを明るくし、
		// 176 からは転送のたびに画素値を 2 ずつ下げて消す
		if (frame == 128)
			work->transfer_value = 50;
		if (frame == 176)
		{
			work->transfer_mode = 3;
			work->transfer_value = 0x02020202;
			palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
			Iwram_CopyWords((void *)0x05000000, palette, 128);
		}
		else if (frame >= 160 && frame <= 175)
		{
			work->transfer_mode = 1;
			work->transfer_value = 0x10101010;
			if (frame > 173)
				work->transfer_value = 0x3f3f3f3f;
			Palette_BrightenBgEntries(2, 2, 2);
		}

		//------------------------------------------------------------------
		// 新しい粒を出す
		//------------------------------------------------------------------
		// 昇る粒：y = 112 から上向きに出す
		if (frame >= 33 && frame <= 175)
		{
			s32 spawned = 0;
			s32 limit = 1;

			if (frame > 103)
				limit = 8;
			for (i = 0; i != 512; i++)
			{
				struct EffectStep *grain = &GRAINS[i];

				if (grain->variant == -1)
				{
					grain->x = ((Random16() & 255) - 32) << 16;
					grain->y = 112 << 16;
					grain->velocity_x = ((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3])
										<< 9;
					grain->velocity_y = -((Random16() & 127) + PaletteParticles_DropSpeeds[i & 3])
										<< 11;
					grain->variant = 0;
					spawned++;
					if (spawned == limit)
						break;
				}
			}
		}
		// 火花：奇数フレームに 1 つ、斜め上へ出す
		if (frame >= 41 && frame <= 127)
		{
			s32 spawned = 0;

			if (frame & 1)
			{
				for (i = 0; i != 24; i++)
				{
					struct EffectStep *spark = &work->particles[40 + i];

					if (spark->variant == -1)
					{
						s32 speed = (Random16() & 255) + 128;
						s32 angle = (Random16() & 0x1fff) + 20000;

						if (mode == 0)
						{
							spark->x = ((Random16() & 7) + 78) << 16;
							spark->y = 70 << 16;
						}
						else
						{
							spark->x = ((Random16() & 7) + origin.x - 8) << 16;
							spark->y = origin.y << 16;
						}
						spark->velocity_x = speed * Trig_Sin(angle) >> 9;
						spark->velocity_y = speed * Trig_Cos(angle) >> 9;
						spawned++;
						spark->variant = 0;
						if (spawned == 1)
							break;
					}
				}
			}
		}
		if (frame == 48)
			AudioCommand_PlayFar(141);
		if (frame == 128)
			AudioCommand_PlayFar(145);
		// 閃光：1 フレームに 1 つ、横へ出す
		if (frame >= 129 && frame <= 175)
		{
			s32 spawned = 0;

			for (i = 0; i != 24; i++)
			{
				struct EffectStep *flash = &work->particles[i];

				if (flash->variant == -1)
				{
					s32 speed = (Random16() & 255) + 128;
					s32 angle = (Random16() & 0x1fff) - 20000;

					if (mode == 0)
					{
						flash->x = 68 << 16;
						flash->y = 64 << 16;
					}
					else
					{
						flash->x = origin.x << 16;
						flash->y = origin.y << 16;
					}
					flash->velocity_x = speed * Trig_Sin(angle) >> 6;
					flash->velocity_y = speed * Trig_Cos(angle) >> 6;
					flash->variant = 0;
					spawned++;
					if (spawned == 1)
						break;
				}
			}
		}

		//------------------------------------------------------------------
		// 粒を描いて動かす
		//------------------------------------------------------------------
		// 閃光：4 フレームごとに絵を進め、24 フレームで消える
		if (frame <= 175)
		{
			for (i = 0; i != 24; i++)
			{
				struct EffectStep *flash = &work->particles[i];

				if (flash->variant >= 0)
				{
					draw.hikaku(canvas, work->sheet + ((flash->variant >> 2) << 11),
						HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
					flash->x += flash->velocity_x * dir;
					flash->y += flash->velocity_y;
					flash->variant++;
					if (flash->variant == 24)
						flash->variant = -1;
				}
			}
		}

		// 火花：1×2 の点、上へ加速して 48 フレームで消える
		for (i = 0; i != 24; i++)
		{
			struct EffectStep *spark = &work->particles[40 + i];
			s32				   size = 1;

			if (spark->variant >= 0)
			{
				draw.kasan(canvas, PaletteParticles_SparkPixels, HI(spark->x), HI(spark->y) - size,
					size, size * 2);
				spark->x += spark->velocity_x * dir;
				spark->y += spark->velocity_y;
				spark->velocity_y += -0x400;
				spark->variant++;
				if (spark->variant == 48)
					spark->variant = -1;
			}
		}

		// 昇る粒：128 フレームまでは上へ加速し、その後は上昇の速さを保ったまま
		// x の速度を毎フレーム減らして左へ曲げる (dir = -1 なら右、奇数番が強い)。
		// 256 フレームで消える
		if (frame <= 175)
		{
			for (i = 0; i != 512; i++)
			{
				struct EffectStep *grain = &GRAINS[i];

				if (grain->variant >= 0)
				{
					u8	size = PaletteParticles_DropSizes[i & 3];
					s32 height = size * 2;

					draw.hikaku(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
					grain->x += grain->velocity_x * dir;
					grain->y += grain->velocity_y;
					if (frame > 128)
					{
						if (i & 1)
							grain->velocity_x += -0x8000;
						else
							grain->velocity_x += -0x2000;
					}
					else
					{
						grain->velocity_y += -0x400;
					}
					grain->variant++;
					if (grain->variant == 256)
						grain->variant = -1;
				}
			}
		}

		//------------------------------------------------------------------
		// 揺れと隊列
		//------------------------------------------------------------------
		if (frame == 128)
			work->shake_frames = 48;
		if (mode == 0 && frame == 48)
			work->shake_frames = 8;

		// 40〜47 フレームに隊列を 1 フレームあたり左へ 8、下へ 16 ドット動かす
		if (frame >= 40 && frame <= 47)
		{
			focus.x += -(8 << 16);
			focus.y += 16 << 16;
		}
		if (mode == 0)
		{
			if (frame == 128)
			{
				Object_InitializeMode(work->objects[0], 8);
				Object_InitializeMode(work->objects[1], 9);
				Object_InitializeMode(work->objects[3], 10);
				Object_InitializeMode(work->objects[4], 11);
			}
			if (frame == 176)
			{
				Object_InitializeMode(work->objects[0], 0);
				Object_InitializeMode(work->objects[1], 1);
				Object_InitializeMode(work->objects[3], 3);
				Object_InitializeMode(work->objects[4], 4);
			}
			BattleFx_PlaceFormationObjects(3, focus.x, focus.y);
		}
		if (frame == 138)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, -1, 0);
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
			}
		}

		// 筋：待ちの後、1 フレームに 6 ドットずつ斜めに落ち、y = 55 を越えたら
		// 2 フレームごとに 6 枚の絵を繰り返す
		if (frame <= 175)
		{
			for (i = 0; i != 16; i++)
			{
				struct EffectStep *streak = &work->particles[24 + i];

				if (streak->y > 55)
				{
					if (streak->variant >= 0 && streak->variant <= 11)
					{
						s32 cel = streak->variant / 2;

						draw.hikaku(canvas,
							work->sheet + PaletteParticles_StreakOffsets[cel] + 0x3000,
							streak->x - PaletteParticles_StreakWidths[cel] / 2,
							streak->y + PaletteParticles_StreakDrops[cel],
							PaletteParticles_StreakWidths[cel],
							PaletteParticles_StreakHeights[cel]);
					}
					streak->variant++;
					if (streak->variant == 12)
						streak->variant = 0;
				}
				else if (streak->variant == 0)
				{
					s32 size = 10;

					streak->x -= dir * 6;
					streak->y += 6;
					if (frame <= 47 && streak->y > 55)
						AudioCommand_PlayFar(136);
					draw.hikaku(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
						streak->x - size / 2, streak->y + 30, size, size * 2);
				}
				else
				{
					streak->variant++;
				}
			}
		}
		Camera_ApplyShake(8, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	if (mode == 0)
	{
		BattleEffect_RunImpactBurst(3, focus.x, focus.y);
		for (i = 0; i != 8; i++)
			ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	}
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 124 フレームの演出。33〜97 フレームに A か B を押すと 98 フレームへ飛ばす
// 12 体の隊列を右から入れ、28 フレームと 32〜79 フレームに粒を噴き上げる
// 粒は重力で落ち、80 フレームを過ぎると上へ加速して昇る
// 32〜79 フレームには 3 本の柱 (FirePillarA の絵) を上へ流す
//--------------------------------------------------------------------------
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect)
{
	DrawRectangle			 draw[2];
	void				   **cursor;
	void					*canvas;
	struct BattleEffectWork *work;
	u8						*graphics;
	u8						*palette;

	// cursor は gWorkSlot[40] (キャンバス) を指す
	cursor = (void **)(gWorkSlot + 40 * 4);
	canvas = cursor[0];
	work = cursor[-1];	  // [39] 作業領域
	graphics = cursor[1]; // [41] 粒の絵
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDALPHA = 0x1010;
	BattleFx_FetchRectangleBlitters(0, draw);

	// 下の帯は WaterSpray、柱は FirePillarA (sheet + 0x6e4) の絵
	palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
	Iwram_CopyWords((void *)0x05000000, palette, 128);
	palette += 128;
	Resource_DecodeType01(palette, work);
	palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
	palette += 128;
	Resource_DecodeType01(palette, work->sheet + 0x6e4);
	Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
	work->transfer_mode = 2;
	work->transfer_value = 75; // 転送のたびに 3/4 へ減らして残像にする
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	{
		struct Point2D	   focus;
		struct Point2D	   velocity;
		s32				   rise;
		s32				   frame;
		s32				   k;
		struct EffectStep *point;
		struct EffectStep *spark;
		struct EffectStep *sparks;
		s32				  *life;

		// 隊列は (256, 88) から左へ
		rise = 0;
		focus.x = 256 << 16;
		focus.y = 88 << 16;
		velocity.x = -(16 << 16);
		velocity.y = -(4 << 16);
		for (k = 0; k != 64; k++)
			work->particles[k].variant = -1;
		for (k = 0; k != 16; k++)
		{
			point = &work->particles[24 + k];
			point->x = Random16() & 127;
			point->y = (Random16() & 7) + 56;
			point->variant = -(Random16() & 15);
		}
		life = &((struct EffectStep *)gMapCellBuffer)->variant;
		for (k = 0; k != 1024; k++)
		{
			*life = -1;
			life += 7; // sizeof (struct EffectStep) / 4
		}
		sparks = (struct EffectStep *)gMapCellBuffer;
		BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);
		WaitFrames(1);
		BattleFx_SpawnObjects(12, 380, 2);

		for (frame = 0; frame != 124; frame++)
		{
			if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 32 && frame <= 97)
				frame = 98;
			if (frame == 120)
				BattleEventRuntime_BeginPhaseFar(134);
			if (frame <= 15)
				rise += 2;

			// 隊列：速度を毎フレーム x 58/64、y 56/64 に減らし、x = 120 より左では右へ押し戻す
			if (frame <= 99)
			{
				focus.x += velocity.x;
				focus.y += velocity.y;
				velocity.x = velocity.x * 58 / 64;
				velocity.y = velocity.y * 56 / 64;
				if (focus.x < 120 << 16)
					velocity.x += 0x8000;
			}
			BattleFx_PlaceFormationObjects(1, focus.x, focus.y);

			// 粒：(32, 96) のまわりから上向きに、28 フレームに 256 個、
			// 32〜79 フレームには 1 フレームに 16 個ずつ噴き上げる
			if (frame == 28)
			{
				for (k = 0; k != 256; k++)
				{
					spark = &sparks[k];
					if (spark->variant == -1)
					{
						s32 radius = Random16() & 63;
						s32 angle = Random16() & 0xffff;

						spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
						spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
						spark->velocity_x = ((Random16() & 63) - 32) << 14;
						spark->velocity_y = (-(Random16() & 63) - 8) << 13;
						spark->variant = 0;
					}
				}
			}
			if (frame >= 32 && frame < 80)
			{
				s32 spawned = 0;

				for (k = 0; k != 1024; k++)
				{
					spark = &sparks[k];
					if (spark->variant == -1)
					{
						s32 radius = Random16() & 63;
						s32 angle = Random16() & 0xffff;

						spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
						spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
						spark->velocity_x = ((Random16() & 63) - 32) << 14;
						spark->velocity_y = (-(Random16() & 63) - 8) << 13;
						spark->variant = 0;
						spawned++;
						if (spawned == 16)
							break;
					}
				}
			}
			if (frame == 0)
				AudioCommand_PlayFar(164);
			if (frame == 32)
				AudioCommand_PlayFar(145);
			if (frame == 80)
				AudioCommand_PlayFar(144);

			// 柱：104 ドットの絵を 1 フレームに 16 ドットずつ上へ流す
			if (frame >= 32 && frame < 80)
			{
				for (k = 0; k != 3; k++)
				{
					s32 span = (frame * 16 - 256 + k * 25) % 104;

					draw[0](canvas, work->sheet + 0x6e4, EmberColumns_Columns[k * 2] - 17,
						EmberColumns_Columns[k * 2 + 1] - span - 104, 34, 104);
					draw[0](canvas, work->sheet + 0x6e4, EmberColumns_Columns[k * 2] - 17,
						EmberColumns_Columns[k * 2 + 1] - span, 34, span);
				}
			}

			// 下の帯：32×32 の絵を 5 枚、16 フレームで 32 ドットせり上がる
			if (frame <= 95)
			{
				for (k = 0; k != 5; k++)
					draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise, 32, 32);
			}

			// 粒：68 フレームを過ぎると大きくし、落ちているものは加算合成で描く。
			// 80 フレームまでは EmberColumns_Gravity で下へ、その後は上へ加速し、
			// 速度は毎フレーム 62/64 に減らす。落ちながら y が 104 を越えたら消す
			for (k = 0; k != 1024; k++)
			{
				spark = &sparks[k];
				if (spark->variant >= 0)
				{
					s32 size = k % 3 + 2;

					if (spark->velocity_y > 0)
						size += 2;
					if (frame > 68 && size <= 5)
						size = 6;
					if (frame > 70 && size <= 6)
						size = 7;
					if (frame > 72 && size <= 7)
						size = 8;
					if (frame > 74 && size <= 8)
						size = 9;
					if (frame > 76)
						size = 10;
					draw[spark->velocity_y > 0](canvas,
						graphics + ParticleStreams_CellOffsets[size - 1], HI(spark->x) - size / 2,
						HI(spark->y) - size, size, size * 2);
					spark->x += spark->velocity_x;
					spark->y += spark->velocity_y;
					if (frame > 80)
						spark->velocity_y += -0x8000;
					else
						spark->velocity_y += EmberColumns_Gravity[k & 3];
					spark->velocity_x = spark->velocity_x * 62 / 64;
					spark->velocity_y = spark->velocity_y * 62 / 64;
					spark->variant++;
					if (spark->velocity_y > 0 && HI(spark->y) > 104)
						spark->variant = -1;
				}
			}

			// 対象：36・48・60・72 フレームにパレット 7 で跳ねさせ、6 フレーム後にパレットを戻す
			if (frame <= 79)
			{
				for (k = 0; k != work->effect->count; k++)
				{
					if (frame > 29)
					{
						s32 phase = frame % 12;

						if (phase == 0)
						{
							struct MotionObject *obj =
								GetBattleObjectSlotFar(work->effect->actors[k])->object;

							ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, -1, 0);
							obj->velocity_y = 0x48000;
							obj->vertical_motion_strength = 0xab85;
						}
						if (phase == 6)
							ObjectGroup_UpdateMembers(work->effect->actors[k], 0, 5, -1, 0);
					}
				}
			}
			work->transfer_pending = 1;
			WaitFrames(1);
		}

		Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
		Runtime_ReleaseHeapBlock(47);
		Runtime_ReleaseHeapBlock(46);
		BattleEffect_RunImpactBurst(1, focus.x, focus.y);
		for (k = 0; k != 12; k++)
			ResourceObject_ReleaseFar(
				(struct ResourceObjectWork *)((void **)(work->sheet + 0x77d8))[k]);
	}
	BattleFx_EndCanvasLayer();
}
