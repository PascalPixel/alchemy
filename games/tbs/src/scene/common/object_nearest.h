#include "types.h"
#include "field_obj.h"
#include "field_scene.h"

struct SpriteNumber
{
	u8		   unk00[0x28];
	const s16 *number;
};

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
	struct EventWork   *work = gEventWork;
	struct FieldActor **slot;
	struct FieldActor  *origin;
	s32					nearest_actor = 0;
	s32					min_dist;
	u32					actorId;

	min_dist = 640;
	origin = Object_GetById(ACTOR_PARTY_LEADER);
	actorId = ACTOR_FIRST_PLACED;
	slot = work->placedActors;
	do
	{
		struct FieldActor *actor = *slot++;
		if (actor != 0)
		{
			if (*((struct SpriteNumber *)actor->sprite)->number == 0xf2)
			{
				s32 dist = FixedPoint_Distance(&origin->x.fixed, &actor->x.fixed);
				if (dist < min_dist)
				{
					min_dist = dist;
					nearest_actor = actorId;
				}
			}
		}
		actorId++;
	} while (actorId <= 65);
	return nearest_actor;
}
