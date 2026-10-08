//--------------------------------------------------------------------------
// FINISH_SCENE_AND_RELEASE_HEAP_BLOCK.C
// フィールド効果：ラインごとの波と色の変化、対象から使い手へ流れる粒
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
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIXED_MATH.H"
#include "MENU_LIST.H"

#define Animation_ApplyChildPalette ((void (*)(struct FxObject *, s32))Animation_ApplyChildPalette)
#define Object_SetMoveTargetFar \
	((void (*)(struct FxObject *, s32, s32, s32))Object_SetMoveTargetFar)
#define BattleFx_SetCallbackWhenTargetUnset ((void (*)(void))BattleFx_SetCallbackWhenTargetUnset)

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct FxVector
{
	s32 x;
	s32 y;
	s32 z;
};

struct FxSprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
	u8	unknown_0a[0x1c];
	u8	palette;
};

struct FxObject
{
	u16				 unknown_00[3];
	u16				 angle;
	s32				 x;
	s32				 y;
	s32				 z;
	u32				 unknown_14[7];
	s32				 scale_x; // 0x10000 で等倍
	s32				 scale_y;
	u32				 unknown_38[6];
	struct FxSprite *sprite;
	u32				 unknown_54 : 8;
	u32				 visible : 8;
	u32				 unknown_56 : 16;
	u32				 unknown_58[5];
	void			*callback;
};

// RunSceneTransitionEffect が 0x298 バイト確保して初期化する
struct WaveFxWork
{
	u16 lines[2][162]; // ラインごとのずれ (2 面を交互に書く)
	u16 phase;		   // 波の位相
	u8	page;		   // 書き込む面
	s8	blue;		   // 色相から求めた色
	s8	green;
	s8	red;
	u16 hue;	   // 色相 (0〜359 度)
	u16 source_id; // 使い手
	u16 target_id; // 対象
	u8	delay;	   // 開始までの待ち
	u8	counter;   // 粒を出す周期 (0〜60)
};

void			 Ui_SetBank15PaletteAndClearRenderMode(void);
s16				*BattleAction_FindDescriptor(s32 id);
struct FxObject *Object_CreateFar(s32 sprite, s32 x, s32 y, s32 z);
s32				 ArcTan2(s32 y, s32 x);

//--------------------------------------------------------------------------
// 効果を止めて使い手の色を戻し、作業領域を返す
//--------------------------------------------------------------------------
void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
	void *work;

	work = *(void **)((u32)&gBattleBgFxWork);
	Ui_SetBank15PaletteAndClearRenderMode();
	Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
	Animation_ApplyChildPalette(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1); // source_id
	BattleFx_PrepareBufferInterpolation();
	Runtime_ReleaseHeapBlock(22);
}

//--------------------------------------------------------------------------
// 毎フレームの処理 (スケジューラから呼ばれる)
//--------------------------------------------------------------------------
void BattleFx_UpdateWaveScene(void)
{
	struct WaveFxWork		 *work = gBattleBgFxWork;
	u16						 *line;
	u32						  i;
	struct FxObject			 *src;
	struct FxObject			 *dst;
	struct FxObject			 *obj;
	struct FxSprite			 *sprite;
	struct FxVector			  from;
	struct FxVector			  to;
	struct AnimationMetadata *meta;

	if (work->delay != 0)
	{
		work->delay--;
		return;
	}

	// 160 ライン分のずれを ±4 ドットの正弦波で作り、面を切り替える
	line = work->lines[work->page ^ 1];
	for (i = 0; i < 160; i++)
		*line++ = Trig_Sin(__udivsi3((work->phase + i * 8) << 16, 160)) >> 14;
	work->phase += 4;
	work->page ^= 1;
	if (work->page != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(
			(work->red << 10) | (work->green << 5) | work->blue | 0x200000, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_AdvanceHueCycle();
	}
	Animation_ApplyChildPalette(Object_GetById(work->source_id), 0);

	// 周期の 0, 8, 16 で、対象から使い手へ向かう粒を出す
	if (work->counter == 0 || work->counter == 8 || work->counter == 16)
	{
		src = Object_GetById(work->source_id);
		dst = Object_GetById(work->target_id);
		if (src != 0 && dst != 0)
		{
			// メタデータの box_x を高さに使い、そこから 2 ドット下
			from.x = src->x;
			meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->source_id));
			from.y = src->y + ((s8)meta->box_x << 16) - (2 << 16);
			from.z = src->z;
			to.x = dst->x;
			meta = Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(work->target_id));
			to.y = dst->y + ((s8)meta->box_x << 16) - (2 << 16);
			to.z = dst->z;
			obj = Object_CreateFar(281, to.x, to.y, to.z);
			if (obj != 0)
			{
				sprite = obj->sprite;
				obj->visible = 0;
				obj->scale_x = 0xa3d7; // 0.64 倍
				obj->scale_y = 0xa3d7;
				obj->angle = ArcTan2(from.z - to.z, from.x - to.x);
				obj->callback = BattleFx_SetCallbackWhenTargetUnset;
				sprite->palette = 0;
				sprite->mode = 1;
				Object_SetMoveTargetFar(obj, from.x, from.y, from.z);
			}
		}
	}

	if (work->counter == 0)
		AudioCommand_PlayFar(130);
	if (++work->counter > 60)
		work->counter = 0;
}
