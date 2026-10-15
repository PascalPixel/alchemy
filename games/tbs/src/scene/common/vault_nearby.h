#include "types.h"
#include "field_obj.h"

static s32 SceneActor_UpdateProximityToLeader(struct FieldActor *self)
{
	u8				 **globals = gWindowWork;
	u8				  *scene = globals[0];
	u8				  *workspace = globals[12];
	s16				  *flags = &self->timer;
	s32				   force = 0;
	s32				   range = 18;
	struct FieldActor *partner;
	struct FieldActor *player;

	if ((*flags & 1) != 0)
	{
		partner = Object_GetById(15);
	}
	else
	{
		partner = Object_GetById(14);
	}
	if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0)
	{
		return 0;
	}

	player = Object_GetById(ACTOR_PARTY_LEADER);

	if (*(s16 *)(workspace + 376) != 0 || scene[RENDER_MODE_OFS] != 0)
	{
		range = 26;
		if ((*flags & 2) != 0)
		{
			force = 1;
		}
	}

	SceneActor_CheckFacingAndRange(self, player, range, force);
	return 0;
}
