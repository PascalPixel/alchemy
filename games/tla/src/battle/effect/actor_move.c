#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_unit.h"

void *UnitGet(s32 owner);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration, s32 speed_limit, s32 velocity_y, s32 percent)
{
	struct BattleObj		*slot;
	struct MotionObject *object;
	u32					 enemy;

	slot = GetBattleObjectSlotFar(id);
	object = slot->object;
	enemy = ((struct BattleUnit *)UnitGet(id))->enemy_id;
	if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376)
	{
		object->acceleration = acceleration;
		object->speed_limit = speed_limit;
		if (force != 0 || object->velocity_y != 0)
			object->velocity_y = velocity_y;
		Object_ResetMotion(object);
		Object_SetPosition(object, slot->anchor_x * percent / 100, 0, slot->anchor_z);
	}
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	Object_SetMode(object, 5);
}
