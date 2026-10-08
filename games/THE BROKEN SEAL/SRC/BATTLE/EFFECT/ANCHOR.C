//--------------------------------------------------------------------------
// ANCHOR.C
// エナジー演出 7：左右の二つの物体と、飛び散る破片
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
#include "MOTOBJ.H"
#include "FX_SCENE.H"
#include "OBJDISP.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "SOUND_IDS.H"
#include "FIXED_MATH.H"
#include "FIELD_SPRITE.H"

#define Motion_SetTargetPositionFromMagnitudeAngle \
	((void (*)(void *, s32, s32))Motion_SetTargetPositionFromMagnitudeAngle)

extern struct BattleFxScene *gEffectWork;
void						 Script_WaitForEventTimeoutFar(void *object);
void						 AudioCommand_PlayFar(s32 sound);
struct MotionObject			*Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
extern const s32			 BattleFx_FragmentScript[];
void						 ObjectDispatch_ReleaseFar(void *object);

void ObjectDispatch_ApplyArgumentToChildrenFar(void *, s32);

#define HAHEN_MAX 24 // 破片の数

//--------------------------------------------------------------------------
// 対象のない演出 7
// 演出の位置の左右 32 ドットに物体を出して動かし、破片を 24 個散らす
//--------------------------------------------------------------------------
void RunBattleEffect07(void)
{
	struct BattleFxScene  *work;
	struct MotionObject	  *obj;
	struct MotionObject	  *pair[2];
	struct ScatterPosition pos;
	struct MotionObject	 **walk;
	s32					   n;
	s32					   dist;

	work = gEffectWork;
	BattleEffect_InitializeSharedScene();
	pos.x = work->x;
	pos.y = work->y + 0x100000; // 16 ドット上
	pos.z = work->z;
	pair[0] = BattleFx_SpawnItemBreakMode3(pos.x + 0x200000, pos.y, pos.z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos.x - 0x200000, pos.y, pos.z, 0);

	// それぞれの向きへ 24 ドット動かす
	WaitFrames(15);
	walk = pair;
	for (n = 0; n < 2; n++)
	{
		obj = *walk++;
		if (obj != 0)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle);
	}

	// 破片：8-32 ドット先へ散る
	Script_WaitForEventTimeoutFar(pair[0]);
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	for (n = 0; n < HAHEN_MAX; n++)
	{
		pos.x = work->x;
		pos.y = work->y + 0x100000;
		pos.z = work->z;
		obj = Object_Spawn(285, pos.x, pos.y, pos.z);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
			obj->speed_limit = Random16() + 0x20000;
			obj->acceleration = 0x20000;
			obj->motion_flags = 0;
			dist = Random16() * 24 + 0x80000;
			Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
		}
	}

	ObjectDispatch_ReleaseFar(pair[0]);
	ObjectDispatch_ReleaseFar(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_x = obj->scale_y = 0x4000;
		obj->acceleration = obj->speed_limit = 0x30000; // 3 ドット/フレーム
		((struct FieldSprite *)obj->records)->priority = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000; // 1/8 回転
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}
