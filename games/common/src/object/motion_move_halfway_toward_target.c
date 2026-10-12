//--------------------------------------------------------------------------
// motion_move_halfway_toward_target.c
// オブジェクト：つながった相手への追従
//--------------------------------------------------------------------------

#include "object_runtime.h"

//--------------------------------------------------------------------------
// つながったオブジェクトとの距離を、x・y・z とも半分つめる
// obj : 動かすオブジェクト
//--------------------------------------------------------------------------
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj)
{
	struct ObjectRuntime *tgt = obj->linked_object;

	obj->x += (tgt->x - obj->x) / 2;
	/* FAKEMATCH: separate CSE blocks retain each axis's own-position load and
	   destructive add. Direct updates reverse the y/z loads under TLA's flags;
	   unscoped temporaries change four add/store registers in TBS. */
	do {
		s32 position = obj->y;

		position += (tgt->y - position) / 2;
		obj->y = position;
	} while (0);
	do {
		s32 position = obj->z;

		position += (tgt->z - position) / 2;
		obj->z = position;
	} while (0);
}
