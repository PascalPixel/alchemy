#include "types.h"
#include "fieldobj.h"

static s32 SceneActor_SetFlagBitByRelativeDepth(struct FieldActor *actor)
{
	struct FieldActor *ref;
	u8				  *fp;
	u8				   flag;
	ref = Object_GetById(ACTOR_PARTY_LEADER);
	fp = &actor->priority_flags;
	flag = *fp | 2;
	*fp = flag;
	if (ref->z.fixed < actor->z.fixed)
	{
		s32 lim = actor->z.fixed - ref->z.fixed;
		s32 ay;
		lim += 0x00040000;
		ay = actor->y.fixed;
		ay += lim;
		if (ref->y.fixed <= ay)
		{
			flag &= 0xfd;
			*fp = flag;
		}
	}
	return 0;
}

static void SceneActor_SwapPositionsByDepth(s32 first, s32 second)
{
	struct FieldActor *a = Object_GetById(first);
	struct FieldActor *b = Object_GetById(second);

	if (a->z.fixed <= b->z.fixed)
	{
		s32 t;

		t = a->x.fixed;
		a->x.fixed = b->x.fixed;
		b->x.fixed = t;
		t = a->y.fixed;
		a->y.fixed = b->y.fixed;
		b->y.fixed = t;
		t = a->z.fixed;
		a->z.fixed = b->z.fixed;
		b->z.fixed = t;
		Engine_TaskWait(1);
	}
}
