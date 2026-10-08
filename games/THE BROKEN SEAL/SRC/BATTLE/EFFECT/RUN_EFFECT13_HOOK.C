//--------------------------------------------------------------------------
// RUN_EFFECT13_HOOK.C
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
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
#include "FX_SCENE.H"
#include "MOTOBJ.H"
#include "FIXED_POINT_POSITION.H"
#include "SYSTEM.H"
#include "FIELD_EVENT.H"
#include "MENU_LIST.H"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define Object_SetPositionAndResetMotionFar \
	((void (*)(struct MotionObject *, s32, s32, s32))Object_SetPositionAndResetMotionFar)

extern struct BattleFxScene *gEffectWork;

void				 Vector_AddPolarOffset(s32, s32, s32 *);
struct MotionObject *Object_Spawn(s32, s32, s32, s32);
void				 Animation_ApplyChildValuesFar(struct MotionObject *, u32);
void				 ObjectDispatch_ReleaseFar(struct MotionObject *);

//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(void)
{
	RunBattleEffect13();
}

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void)
{
	struct BattleFxScene	 *scene = gEffectWork;
	struct MotionObject		 *actor = scene->main_object;
	struct MotionObject		 *child = scene->child;
	struct MotionObject		 *object;
	struct FixedPointPosition spawn;
	struct FixedPointPosition origin;
	struct FixedPointPosition target;
	s32						  step;
	s32						  i;
	s32						  steps = 11;

	origin.x = actor->x;
	origin.y = actor->y + (16 << 16);
	origin.z = actor->z;
	if (scene->enabled)
	{
		target.x = actor->x;
		target.y = actor->y + (32 << 16);
		target.z = actor->z;
		Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
	}
	else
	{
		target.x = scene->x;
		target.y = scene->y + (32 << 16);
		target.z = scene->z;
	}
	spawn.x = scene->x;
	spawn.y = scene->y + (32 << 16);
	spawn.z = scene->z;
	object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
	if (object == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	object->angle = actor->angle;
	object->speed_limit = 0x14ccc;
	object->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	Animation_ApplyChildValuesFar(object, 1);

	// 目標へ伸ばす
	step = 0;
	do
	{
		s32 scale;

		object->x = origin.x + (target.x - origin.x) * step / 10;
		object->y = origin.y + (target.y - origin.y) * step / 10;
		object->z = origin.z + (target.z - origin.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	WaitFrames(10);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
	WaitFrames(15);

	// 2 ドットずつ 10 フレーム押し下げる
	for (i = 0; i < 10; i++)
	{
		object->y -= 2 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	AudioCommand_PlayFar(132);
	if (child != 0)
		Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y, -(9 << 16));
	WaitFrames(20);

	// 1.5 ドットずつ 13 フレーム上げる
	for (i = 0; i < 13; i++)
	{
		object->y += 0x18000;
		WaitFrames(1);
	}
	WaitFrames(10);
	AudioCommand_PlayFar(114);

	// 使う者のところへ縮めて戻す
	step = 0;
	do
	{
		s32 scale;

		object->x = target.x + (origin.x - target.x) * step / 10;
		object->y = target.y + (origin.y - target.y) * step / 10;
		object->z = target.z + (origin.z - target.z) * step / 10;
		scale = BattleFx_Interpolate(0x4000, 0x10000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	ObjectDispatch_ReleaseFar(object);
	BattleFx_PrepareBufferInterpolation();
}
