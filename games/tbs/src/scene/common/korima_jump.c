#include "types.h"
#include "engcall.h"
#include "fieldobj.h"
#include "staged_actor_effect.h"
#include "game_state.h"

static s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
	struct StagedActorEffect *actor = (void *)Object_GetById(0);
	u8						 *flags = &actor->motion_flags;
	u8						  saved = *flags;
	s32						  ret = FieldObject_CheckMovementCollision(actor, request);

	if (ret == 0)
	{
		Engine_EventBegin();
		Object_SetMode((struct FieldActor *)actor, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode((struct FieldActor *)actor, 7);
		actor->move_rate_x = 0x30000;
		actor->move_rate_z = 0x20000;
		actor->elevation_rate = 0x40000;
		*flags &= 0x7e;
		Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
		Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, request->cell_x, request->cell_z);
		Object_SetMode((struct FieldActor *)actor, 6);
		Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 1);
		*flags = ret;
		Engine_ActorSetAnimation(10, 7);
		actor->position_x += 0xffff0000;
		actor->position_z += 0xffff0000;
		Engine_TaskWait(2);
		actor->position_x += 0xffff0000;
		actor->position_z += 0xffff0000;
		Engine_TaskWait(10);
		actor->position_x += 0x10000;
		actor->position_z += 0x10000;
		Engine_TaskWait(4);
		actor->position_x += 0x10000;
		actor->position_z += 0x10000;
		*flags = saved;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}

static void EV11_TOUCH(void)
{
	u32 buf[3];
#if !EDITION_INTERNATIONAL
	buf[0] = Object_GetById(0)->x.fixed + 0x1e0000;
	buf[1] = Object_GetById(0)->y.fixed;
	buf[2] = Object_GetById(0)->z.fixed;
#else
	s32				   off = 500;
	struct FieldActor *p = Object_GetById(*(s32 *)((u8 *)&gGameState + off));
	u32				   base = p->x.fixed & 0xfff00000;

	buf[0] = base + 0x80000;
	buf[1] = p->y.fixed;
	buf[2] = (p->z.fixed & 0xfff00000) + 0x80000;
	buf[0] = base + 0x280000;
#endif
	StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}
