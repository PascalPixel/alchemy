//--------------------------------------------------------------------------
// fragment.c
// 演出：アイテムがこわれる演出の本体の動き
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "objdisp.h"
#include "fixed_math.h"
#include "object_effect_scripts.h"
#include "system.h"
#include "field_event.h"

struct ItemBreakFragmentPosition
{
	s32 x;
	s32 y;
	s32 z;
};

struct ItemBreakFragmentSource
{
	u8								reserved_00[6];
	u16								angle; // 向き（0x10000 で 1 周）
	s32								x;
	s32								y;
	s32								z;
	u8								reserved_14[70];
	u8								flag_5a;
	u8								reserved_5b[13];
	struct ItemBreakFragmentSource *target; // 向く相手
};

struct ItemBreakFragmentObject
{
	u8	reserved_00[72];
	s32 field_48; // 重力（gravity と同じ位置）
	u8	reserved_4c[9];
	u8	mode_55; // motion_flags と同じ位置
	u8	reserved_56[8];
	u16 field_5e;
};

extern s32 ArcTan2(s32, s32);

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);

//--------------------------------------------------------------------------
// こわれたアイテムの本体の update
// 向く相手がいれば、そちらへ 1 回に 0x1000（22.5 度）まで曲がる
// そして、まわりにかけらの粒をひとつ出す
// src : 本体のオブジェクト
//--------------------------------------------------------------------------
void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src)
{
	struct ItemBreakFragmentSource	*target;
	struct ItemBreakFragmentPosition pos;
	struct ItemBreakFragmentObject	*obj;
	s32								 turn;
	s32								 dist;

	target = src->target;
	if (target != 0)
	{
		s32 dx = target->x - src->x;
		s32 dz = target->z - src->z;

		if (dx != 0 || dz != 0)
		{
#if GAME_TLA
			s32 dir = ArcTan2(dz, dx);
			s32 cur = src->angle;
			s32 lim = 128;

			turn = (s16)(dir - cur);
			lim <<= 5;
			if (turn > lim)
				turn = lim;
#endif
			if (turn < -0x1000)
				turn = -0x1000;
			src->angle += turn;
		}
		src->flag_5a = 0;
	}

	// 粒は本体から y を 8〜24 ずらし、ランダムな向きに半径 0〜48 散らした位置に出す
	pos.x = src->x;
	pos.y = src->y - (Random16() << 4) - 0x80000;
	pos.z = src->z;
	dist = Random16() * 3;
	dist <<= 4;
	Vector_AddPolarOffset(dist, Random16(), &pos);

#if GAME_TLA
	obj = Object_Spawn(0x2a1, pos.x, pos.y, pos.z);
#endif
	if (obj != 0)
	{
		obj->mode_55 = 2;
		obj->field_48 = 0x1999;
#if GAME_TLA
		Object_SetMode(obj, 0);
#endif
		obj->field_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}
