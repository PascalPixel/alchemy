#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static void Mercury_ArriveWithSparks(void)
{
	struct FieldActor *actor;
	s32				   flag;
	s32				   record;

	actor = Object_GetById(ACTOR_PARTY_LEADER);
	flag = GameFlag_IsSet(0x109);
	if (flag == 0)
	{
		Engine_EventBegin();
		Camera_MoveTo(-1, -1, -1, 0);
		actor->motion_flags = 0;
		Engine_ActorSetPosition(
			0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
		record = Actor_Get(ACTOR_PARTY_LEADER);
		Engine_ActorSetSpriteFlags(record, 0);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		Engine_AudioPlayCue(228);
		actor->update = (void (*)(union FieldObject *))Mercury_TrailSparks;
		Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
		Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
		record = Actor_Get(ACTOR_PARTY_LEADER);
		Engine_ActorSetSpriteFlags(record, 1);
		actor->sprite->priority = 1;
		Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
		actor->motion_flags = 3;
		actor->update = NULL;
		BattleFx_PlayQueuedSound();
		Engine_EventEnd();
	}
}
