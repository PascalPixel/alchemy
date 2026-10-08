//--------------------------------------------------------------------------
// colburst.c
// バトル演出：4 コマの一枚絵と、立ちのぼる柱と火花
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "heap_state.h"
#include "canvas.h"
#include "types.h"
#include "iwram_call.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "callback_scheduler.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "effect_step.h"
#include "b5_context.h"
#include "io_reg.h"
#include "dma.h"

#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

extern u8 gMapCellBuffer[];
void	  AudioCommand_PlayFar(s32 cue);

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

void BattleFx_RunRevealColumnMode1(s32 arg)
{
	BattleFx_RunRevealColumn(arg, 1);
}

void BattleFx_RunRevealColumnMode2(s32 arg)
{
	BattleFx_RunRevealColumn(arg, 2);
}

//--------------------------------------------------------------------------
// 120x120 の絵を 4 コマ、4 フレームずつキャンバスに出し、
// キャンバスを塗りつぶしてから対象に当てる
// variant : 0〜2 で絵が変わる
//--------------------------------------------------------------------------
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant)
{
	void					**heap;
	struct BattleEffectWork	 *work;
	void					 *canvas;
	BattleEffectDrawRectangle draw[2];
	s32						  frame;

	heap = (void **)gBattleFxWork;
	work = *heap++;
	canvas = *heap;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BLDCNT = 0;

	// 1・2 コマ目は work->sheet、3・4 コマ目は gMapCellBuffer（各 120x120）
	if (variant == 0)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, gMapCellBuffer, 1, 1);
	}
	else if (variant == 1)
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, gMapCellBuffer, 1, 1);
	}
	else
	{
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
		Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, gMapCellBuffer, 1, 1);
	}
	work->transfer_mode = 1; // 毎フレーム 0 で消す
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// 行動側を対象へ 16 フレームで寄せる
	if (variant == 1)
		BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x80000);
	else
		BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
	WaitFrames(16);

	if (work->effect->side == 1)
		BattleEffect_LoadWork(
			HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, GOUSEI_YOMI);
	else
		BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_YOMI);
	draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	AudioCommand_PlayFar(212);
	for (frame = 0; frame != 21; frame++)
	{
		if (frame <= 3)
			draw[0](canvas, work, 0, 0, 120, 120);
		else if (frame <= 7)
			draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
		else if (frame <= 11)
			draw[0](canvas, gMapCellBuffer, 0, 0, 120, 120);
		else if (frame <= 15)
			draw[0](canvas, gMapCellBuffer + 0x3840, 0, 0, 120, 120);
		if (frame >= 16 && frame <= 19)
			Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f); // キャンバス全体を 0x3f 番で塗る
		if (frame == 18)
			BattleEventRuntime_BeginPhaseFar(134);
		if (frame == 20)
		{
			work->shake_frames = 8;
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
		}
		Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 対象の足元から柱が 2 本（左右対称）立ちのぼり、火花が 16 個散る
//--------------------------------------------------------------------------
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	void					*sprites;
	DrawRectangleFn			 draw[2];
	struct EffectPosition	 pos;
	struct EffectStep		*spark;
	struct EffectStep		*p;
	s32						 frame;
	s32						 i;
	s32						 height;
	s32						 mode;
	s32						 x;
	u8						*src;
	s32						 speed;
	s32						 angle;
	s32						 cell;
	s32						 size;

	heap = (void **)gBattleFxWork;
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	sprites = heap[2];
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100; // 横も等倍
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
	Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x460, 1, 0);
	BattleFx_FetchRectangleBlitters(work->effect->side, draw);
	work->transfer_mode = 2; // 残像 3/4
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	// キャンバスの x = 64 が対象に来るようにずらす
	EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
	REG_BG2X = (64 - pos.x) << 8;

	// 火花：(64, 112) から四方へ散らし、上へ流す
	for (i = 0, spark = work->particles; i != 16; i++)
	{
		speed = (Random16() & 0x1ff) + 128;
		angle = Random16() & 0xffff;
		spark->x = 0x400000;
		spark->y = 0x700000;
		spark->velocity_x = (Trig_Sin(angle) * speed) >> 8;
		spark->velocity_y = (Trig_Cos(angle) * speed) >> 9;
		spark->variant = Random16() & 7;
		spark++;
	}
	work->shake_frames = 8;
	for (frame = 0; frame != 54; frame++)
	{
		if (frame == 10)
		{
			work->shake_frames = 8;
			BattleEventRuntime_BeginPhaseFar(212);
			BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
			ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
		}

		// 柱：8〜31 フレームで 12 ドットずつ伸び、そのあと 6 ドットずつ縮む
		// 80 ドットで止まっている間は比較合成、それ以外は加算合成
		if (frame > 7)
		{
			if (frame <= 31)
				height = frame * 12 - 96;
			else
				height = 272 - frame * 6;
			if (height > 0)
			{
				if (height > 80)
				{
					height = 80;
					mode = GOUSEI_HIKAKU;
				}
				else
				{
					mode = GOUSEI_KASAN;
				}
				for (i = 0, x = 50; i != 2; i++, x += 14)
				{
					if (i == 0)
						BattleEffect_LoadWork(
							HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, mode);
					else
						BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7,
							GOUSEI_CLIP_X | GOUSEI_CLIP_Y | GOUSEI_FLIP_X, mode);
					draw[0] = ((DrawRectangleFn)gWorkSlot[HEAP_SLOT_BLITTER]);
					((DrawRectangleFn)gWorkSlot[HEAP_SLOT_BLITTER])(
						canvas, work, x, 112 - height, 14, height);
					Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
				}
			}
		}

		// 火花：3 フレームごとに次の絵へ（7 段）
		BattleFx_FetchRectangleBlitters(work->effect->side, draw);
		for (i = 0, p = work->particles; i != 16; i++)
		{
			if (frame >= i / 2 + 8 && p->variant <= 28)
			{
				s32 px;
				s32 py;

				cell = p->variant / 3;
				px = HI(p->x);
				py = HI(p->y);
				if (cell > 6)
					cell = 6;
				src = work->sheet + RisingBurst_SparkCells[cell] + 0x460;
				size = RisingBurst_SparkSizes[cell];
				draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
				p->variant++;
				EffectStep_AdvanceWithGravity2D(p, 62, -0x2000);
			}
			p++;
		}
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
		Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
		if (frame <= 7)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(16, 16);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

#define DMA3_REGISTERS (REG_DMA3)
#define DMA_COPY_WORDS(n) (0x84000000 | (n))

#define BRANCH_FIELD(site, target) \
    ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode)
{
    const u32 *src;
    u32 *dst;
    u32 *no_rows;
    u32 *no_columns;
    u32 *row;
    u32 *skip;
    u32 *run;
    u32 *cell;
    const u32 *end;
    s32 words;

#define PUT(word) words++
#define COPY(n) words += (n); src += (n)
#define COPY_FROM(from, n) words += (n)
#define COPY_TWICE(from, n) words += (n); words += (n)
#define PUT_BRANCH(target) words++
#define MARK(site)
#define PATCH(site)

    words = 0;
    src = SentouKouka_Gousei;

    COPY(3);
    if (mode == GOUSEI_KASAN) {
        COPY_FROM(SentouKouka_Mask, 3);
    }

    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_X) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_Y) {
        COPY_FROM(src, 4);
    }
    src += 4;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == 0) {
        PUT(src[0]);
    }
    src += 1;

    if ((flags & GOUSEI_CLIP_Y) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_Y) != 0) {
            PUT(src[2]);
        } else {
            PUT(src[3]);
        }
        PUT(src[4]);
        PUT(src[5]);
        PUT(src[6] + (1 << height_shift));
        PUT(src[7] + (1 << height_shift));
    }
    src += 8;

    PUT(*src++);
    MARK(no_rows);
    PUT(*src++);

    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0] + (1 << width_shift) - 1);
    }
    src += 1;
    if ((flags & GOUSEI_CLIP_X) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[2]);
            PUT(src[3]);
        } else {
            PUT(src[4]);
            PUT(src[5]);
        }
        PUT(src[6]);
        PUT(src[7]);
        PUT(src[8] + SentouKouka_BitOperands[width_shift]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[9]);
        } else {
            PUT(src[10]);
        }
        PUT(src[11] + SentouKouka_BitOperands[width_shift]);
    }
    src += 12;

    PUT(*src++);
    MARK(no_columns);
    PUT(*src++);

    COPY(6);

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    COPY(5);

    MARK(row);
    PUT(src[0]);
    PUT(src[1] + (1 << (height_shift - 3)) - 1);
    PUT(src[2] + ((width_shift - 3) << 7));
    PUT(src[3]);
    src += 4;

    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;
    PATCH(skip);

    PUT(*src++);
    MARK(skip);
    PUT(*src++);

    MARK(cell);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        {
            s32 i;

            for (i = 0; i < 8; i++) {
                if ((flags & GOUSEI_FLIP_X) != 0) {
                    COPY_FROM(SentouKouka_YomiGyaku, 2);
                } else {
                    COPY_FROM(SentouKouka_YomiJun, 2);
                }
            }
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
        } else {
            COPY_FROM(SentouKouka_Nuri8Jun, 25);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_Hikaku4Gyaku, 16);
        } else {
            COPY_TWICE(SentouKouka_Hikaku4Jun, 16);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_IroGyaku, 14);
        } else {
            COPY_TWICE(SentouKouka_IroJun, 14);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;

    PUT(*src++);
    PUT_BRANCH(cell);
    PATCH(skip);

    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PATCH(skip);

    COPY(3);

    PUT_BRANCH(row);
    PATCH(no_rows);
    PATCH(no_columns);

    PUT(*src++);
    PUT(*src++);

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

    /* FAKEMATCH: needed to match: one-pass blocks keep each writing step together and in list order while allowing scheduling within each step */
#define PUT(word) do { *dst++ = (word); } while (0)
#define COPY(n) do { \
        Dma_Set(src, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
        src += (n); \
    } while (0)
#define COPY_FROM(from, n) do { \
        Dma_Set(from, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
    } while (0)
#define COPY_TWICE(from, n) do { \
        const u32 *part = (from); \
        Dma_Set(part, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
        Dma_Set(part, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
    } while (0)
#define PUT_BRANCH(target) do { \
        *dst = *src++ + BRANCH_FIELD(dst, target); \
        dst++; \
    } while (0)
#define MARK(site) site = dst
#define PATCH(site) do { *(site) |= BRANCH_FIELD(site, dst); } while (0)

    dst = (u32 *)Runtime_AllocateHeapBlock(slot, words << 2);
    src = SentouKouka_Gousei;

    COPY(3);
    if (mode == GOUSEI_KASAN) {
        COPY_FROM(SentouKouka_Mask, 3);
    }

    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_X) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP_Y) {
        COPY_FROM(src, 4);
    }
    src += 4;
    if ((flags & GOUSEI_FLIP) == GOUSEI_FLIP) {
        COPY_FROM(src, 3);
    }
    src += 3;
    if ((flags & GOUSEI_FLIP) == 0) {
        PUT(src[0]);
    }
    src += 1;

    if ((flags & GOUSEI_CLIP_Y) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_Y) != 0) {
            PUT(src[2]);
        } else {
            PUT(src[3]);
        }
        PUT(src[4]);
        PUT(src[5]);
        PUT(src[6] + (1 << height_shift));
        PUT(src[7] + (1 << height_shift));
    }
    src += 8;

    PUT(*src++);
    MARK(no_rows);
    PUT(*src++);

    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0] + (1 << width_shift) - 1);
    }
    src += 1;
    if ((flags & GOUSEI_CLIP_X) != 0) {
        PUT(src[0]);
        PUT(src[1]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[2]);
            PUT(src[3]);
        } else {
            PUT(src[4]);
            PUT(src[5]);
        }
        PUT(src[6]);
        PUT(src[7]);
        PUT(src[8] + SentouKouka_BitOperands[width_shift]);
        if ((flags & GOUSEI_FLIP_X) != 0) {
            PUT(src[9]);
        } else {
            PUT(src[10]);
        }
        PUT(src[11] + SentouKouka_BitOperands[width_shift]);
    }
    src += 12;

    PUT(*src++);
    MARK(no_columns);
    PUT(*src++);

    COPY(6);

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    COPY(5);

    MARK(row);
    PUT(src[0]);
    PUT(src[1] + (1 << (height_shift - 3)) - 1);
    PUT(src[2] + ((width_shift - 3) << 7));
    PUT(src[3]);
    src += 4;

    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
    }
    src += 1;

    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;
    PATCH(skip);

    PUT(*src++);
    MARK(skip);
    PUT(*src++);

    MARK(cell);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        {
            s32 i;

            for (i = 0; i < 8; i++) {
                if ((flags & GOUSEI_FLIP_X) != 0) {
                    COPY_FROM(SentouKouka_YomiGyaku, 2);
                } else {
                    COPY_FROM(SentouKouka_YomiJun, 2);
                }
            }
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_Nuri8Gyaku, 25);
        } else {
            COPY_FROM(SentouKouka_Nuri8Jun, 25);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_Hikaku4Gyaku, 16);
        } else {
            COPY_TWICE(SentouKouka_Hikaku4Jun, 16);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_TWICE(SentouKouka_IroGyaku, 14);
        } else {
            COPY_TWICE(SentouKouka_IroJun, 14);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    if ((flags & GOUSEI_CLIP_X) == 0) {
        PUT(src[0]);
        PUT(src[1]);
    }
    src += 2;

    PUT(*src++);
    PUT_BRANCH(cell);
    PATCH(skip);

    PUT(*src++);
    MARK(skip);
    PUT(*src++);
    if ((flags & GOUSEI_FLIP_X) != 0) {
        PUT(src[0]);
    } else {
        PUT(src[1]);
    }
    src += 2;

    MARK(run);
    switch (mode) {
    default:
    case GOUSEI_YOMI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_YomiGyaku, 2);
        } else {
            COPY_FROM(SentouKouka_YomiJun, 2);
        }
        break;
    case GOUSEI_NURI:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_NuriGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_NuriJun, 4);
        }
        break;
    case GOUSEI_HIKAKU:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_HikakuGyaku, 4);
        } else {
            COPY_FROM(SentouKouka_HikakuJun, 4);
        }
        break;
    case GOUSEI_KASAN:
        if ((flags & GOUSEI_FLIP_X) != 0) {
            COPY_FROM(SentouKouka_KasanGyaku, 6);
        } else {
            COPY_FROM(SentouKouka_KasanJun, 6);
        }
        break;
    }
    src += 4;

    PUT(*src++);
    PUT_BRANCH(run);
    PATCH(skip);

    COPY(3);

    PUT_BRANCH(row);
    PATCH(no_rows);
    PATCH(no_columns);

    PUT(*src++);
    PUT(*src++);

#define ParticleStreams_CellOffsets ((const u32 (*))&ParticleStreams_CellOffsets)

    end = ParticleStreams_CellOffsets;
    return src == end;
}
