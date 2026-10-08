//--------------------------------------------------------------------------
// MOTION_MOVE_HALFWAY_TOWARD_TARGET.C
// オブジェクト：つながった相手への追従
//--------------------------------------------------------------------------

#include "OBJECT_RUNTIME.H"

//--------------------------------------------------------------------------
// つながったオブジェクトとの距離を、x・y・z とも半分つめる
// obj : 動かすオブジェクト
//--------------------------------------------------------------------------
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj)
{
	struct ObjectRuntime *tgt = obj->linked_object;

	obj->x += (tgt->x - obj->x) / 2;
	/* FAKEMATCH: ⚓️'s ARM9 scheduling would load the target's y before the object's own. */
	__asm__("");
	obj->y += (tgt->y - obj->y) / 2;
	/* FAKEMATCH: and the target's z before the object's own. */
	__asm__("");
	obj->z += (tgt->z - obj->z) / 2;
}
