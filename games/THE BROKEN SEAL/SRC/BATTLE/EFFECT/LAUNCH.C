//--------------------------------------------------------------------------
// LAUNCH.C
// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒
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
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "EFFECT_SLOT.H"
#include "FX_SCENE.H"
#include "FIELDOBJ.H"
#include "OBJECT_LOOKUP.H"

#define Camera_WorldToScreen ((void (*)(struct Output *))Camera_WorldToScreen)
#define EffectSlot_Initialize ((void (*)(void *, s32, s32, s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar \
	((void (*)(void *, s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj)
{
	u32 step;

	step = (s16)obj->timer * 0x50;
	obj->facing = obj->facing + step + 0x1000;
	if (step < 0x1000)
		obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj)
{
	u32 scale;

	obj->scale_y -= 0x280;
	scale = obj->scale_x - 0x280;
	obj->facing += 0x2000;
	obj->y.fixed += 1 << 16;
	obj->scale_x = scale;
	if ((s32)scale < 0x3000)
		obj->active = 0;
}

struct Output
{
	s32 x; // 16.16 固定小数
	s32 y;
	s32 z;
};

extern struct BattleFxScene *gEffectWork;
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
extern u32					 gFrameCount;
#endif

void Vector_AddPolarOffset(s32, s32, struct Output *);
void AudioCommand_PlayFar(s32);

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45 度へ
// 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot)
{
	struct Output	   pos;
	struct FieldActor *src;
	s32				   state;
	u32				   r;

	src = gEffectWork->main_object;
	state = slot->state;

	if (state == 0)
	{
		pos.x = src->x.fixed;
		pos.y = src->y.fixed;
		pos.z = src->z.fixed;
		Camera_WorldToScreen(&pos);

		slot->x = pos.x;
		slot->z = pos.z + (8 << 16);
		slot->origin_z = slot->z;
		slot->origin_x = slot->x;
		pos.z = slot->origin_z;
		pos.x = slot->origin_x;

		r = Random16();
		Vector_AddPolarOffset(
			120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000, &pos);

		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 5 << 16;
		slot->max_speed = 5 << 16;
		slot->flag42 = state;
		slot->state++;

		if ((gFrameCount & 2) != 0)
			AudioCommand_PlayFar(246);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			slot->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			BattleFx_ClearOwnedSlot(slot);
	}
}

void Battle_Reset(void);
void ObjectMotion_ArmCallback(s32, s32, s32);
void AudioCommand_PlayFar(s32);
void ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *obj = scene->main_object;
	struct Output		  pos;
	struct EffectSlot	 *slot;
	u8					  last;
	u32					  i;

	Battle_Reset();
	Motion_CamBounds(-1, -1, -1, 0);
	BattleEffect_InitializeSharedScene();
	WaitFrames(10);
	ObjectMotion_ArmCallback(scene->first_object_id, 0x4000, 0);
	WaitFrames(30);
	obj->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(131);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
	WaitFrames(40);

	// 回りはじめる
	AudioCommand_PlayFar(220);
	Animation_ApplyChildValuesFar(obj, 0);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
	obj->timer = 0;
	WaitFrames(70);

	// 縮みはじめ、その画面位置から粒を飛ばす
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	obj->motion_flags = 0;
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
	obj->target_x = ACTOR_NO_TARGET;
	pos.x = obj->x.fixed;
	pos.y = obj->y.fixed;
	pos.z = obj->z.fixed;
	Camera_WorldToScreen(&pos);
	i = 0;
	slot = scene->slots;
	do
	{
		s32 scale;
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
		scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
		slot->scale_y = scale;
		slot->scale_x = scale;
		i++;
		WaitFrames(1);
		slot++;
	} while (i < SLOT_MAX);
	WaitFrames(70);

	// 飛んでいる粒を、着いたら消える状態にする
	i = 0;
	last = 2;
	do
	{
		if (scene->slots[i].active != 0)
			scene->slots[i].state = last;
		i++;
	} while (i < SLOT_MAX);
	WaitFrames(40);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(10);
}
