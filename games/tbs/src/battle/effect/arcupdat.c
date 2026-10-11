//--------------------------------------------------------------------------
// arcupdat.c
// エナジー演出：演出用の物体、ほかの物体の一括設定、ふくらむ弧の動き
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
#include "animspr.h"
#include "fieldobj.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "global_cells.h"
#include "types.h"
#include "fx_scene.h"
#include "object_efx.h"
#include "fixed_math.h"
#include "field_event.h"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct ObjectRuntime *, s32))ObjectDispatch_ApplyValueToChildrenFar)

#define OBJ_LAST 66 // 調べる物体の番号の上限

extern struct BattleFxScene *gEffectWork;
struct ObjectRuntime		*Object_CreateFar(s32, s32, s32, s32);
void						 ObjectDispatch_ReleaseFar(struct ObjectRuntime *);
void						 ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *, s32);

//--------------------------------------------------------------------------
// 演出用の物体を出す
// アニメのない物体は消して NULL。高さは使う者に合わせ、優先度は 0
//--------------------------------------------------------------------------
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
	struct BattleFxScene *work = gEffectWork;
	struct ObjectRuntime *obj;

	obj = Object_CreateFar(kind, x, y, z);
	if (obj != NULL)
	{
		if (obj->animation_kind == 0)
		{
			ObjectDispatch_ReleaseFar(obj);
			return NULL;
		}
		obj->terrain_height = ((struct ObjectRuntime *)work->main_object)->terrain_height;
		obj->flags = 4;
		obj->unknown_23 = 4;
		((struct AnimationObject *)obj->animation)->part[0].priority = 0;
		ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	}
	return obj;
}

//--------------------------------------------------------------------------
// 主人公と except 以外のすべての物体に、動きの状態と値を設定する
//--------------------------------------------------------------------------
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state, s32 value)
{
	s16					 *player;
	struct ObjectRuntime *obj;
	s32					  id;

	id = 0;
	player = (s16 *)gPlayerObjectId;
	do
	{
		obj = ObjectTable_Get(id);
		if (id != *player && obj != NULL && obj != except)
		{
			obj->movement_state = state;
			ObjectDispatch_ApplyValueToChildrenFar(obj, value);
		}
		id++;
	} while (id <= OBJ_LAST);
}

//--------------------------------------------------------------------------
// 弧 A：32 フレームでふくらんでしぼみ、毎フレーム 1 ドット上がる
// z は物体の 9 ドット先から、小さいほど 5 ドットまで離れる
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = (s16)++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

//--------------------------------------------------------------------------
// 弧 B：A の上下を反転したもの。z は物体の 16 ドット先から、小さいほど 5 ドットまで寄る
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = (s16)++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = -v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}
