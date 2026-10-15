//--------------------------------------------------------------------------
// facing.c
// オブジェクト：相手のほうへ向きを変える
//--------------------------------------------------------------------------

#include "types.h"
#include "facing_object.h"

s32 ArcTan2(s32, s32);

//--------------------------------------------------------------------------
// 向く相手がいれば、その方向へ 1 回に 0x1000（22.5 度）まで向きを変える
// obj : 向きを変えるオブジェクト
//--------------------------------------------------------------------------
s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 turn;
	u16					 old;
	s32					 dir;
	struct FacingObject *target;

	target = obj->facingTarget;
	if (target != NULL)
	{
		obj->facingFlags = 0xfe & obj->facingFlags;
		dir = (u16)ArcTan2(target->positionZ - obj->positionZ, target->positionX - obj->positionX);
		old = obj->facing;
		turn = (s16)(dir - old);
		if (turn != 0)
		{
			if (turn > 0x1000)
				turn = 0x1000;
			if (turn < -0x1000)
				turn = -0x1000;
			obj->facing = old + turn;
		}
	}
	return 1;
}
