//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
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
#include "types.h"
#include "motobj.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "sound_ids.h"

#define Camera_WorldToScreen ((void (*)(s32 *))Camera_WorldToScreen)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(struct MotionObject *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)

struct FallbackPosition
{
	s32 x;
	s32 y;
	s32 z;
};

extern struct BattleFxScene *gEffectWork;

struct MotionObject *Object_Spawn(s32, s32, s32, s32);
void				 ObjectDispatch_ApplyArgumentToChildrenFar(struct MotionObject *, s32);
void				 WaitFrames(s32);
void				 AudioCommand_PlayFar(s32);
u32					 Random16(void);
void				 Vector_AddPolarOffset(s32, u32, s32 *);
void				 ObjectDispatch_ReleaseFar(struct MotionObject *);

//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void)
{
	struct BattleFxScene   *scene = gEffectWork;
	struct MotionObject	   *user = scene->main_object;
	struct MotionObject	   *obj;
	struct EffectSlot	   *slot;
	struct FallbackPosition pos;
	struct FallbackPosition from;
	struct FallbackPosition to;
	s32						step;
	s32						i;

	scene->y = user->y;
	obj = Object_Spawn(250, 0, 0, 0);
	step = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	if (obj == 0)
		return;

	// 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
	BattleEffect_InitializeSharedScene();
	from.x = user->x;
	from.y = user->y + (16 << 16);
	from.z = user->z;
	to.x = scene->x;
	to.y = scene->y + (8 << 16);
	to.z = scene->z;
	for (;;)
	{
		s32 steps = 11;
		s32 scale;

		obj->x = from.x + (to.x - from.x) * step / 10;
		obj->y = from.y + (to.y - from.y) * step / 10;
		obj->z = from.z + (to.z - from.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
		obj->scale_x = scale;
		obj->scale_y = scale;
		WaitFrames(1);
		step++;
		if (step >= steps)
			break;
	}

	WaitFrames(5);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

	// その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
	slot = &scene->slots[0];
	for (i = 0; i < 16; i++)
	{
		pos.x = obj->x;
		pos.y = obj->y + (8 << 16);
		pos.z = obj->z;
		Camera_WorldToScreen(&pos.x);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
		EffectSlot_Initialize(slot, 285, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
		slot++;
	}

	pos.x = obj->x;
	pos.y = obj->y + (8 << 16);
	pos.z = obj->z;
	WaitFrames(8);
	ObjectDispatch_ReleaseFar(obj);
	WaitFrames(4);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}
