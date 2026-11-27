// Finds the pushable thing in front of the leader among the placed actors: returns it, with its
// slot, its kind (its row of PushKinds) and the leader's facing in sixteenths of a turn. A log,
// pipe or fallen column is pushed only from its side.
ACTOR *scPushTargetFindAhead(Sint32 *direction, Sint32 *slot, Sint32 *kind)
{
	FIELD_STATE *state  = RamHeapSlots->fieldState;
	ACTOR       *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR       *actor;
	Uint32       i, k;
	Sint32       x, z, left, top, right, bottom;

	*direction = FACING_INDEX(leader->facing);
	for (i = ACTOR_FIRST_PLACED; i <= FIELD_TREASURE_ACTOR - 1; i++)
	{
		actor = state->actors[i];
		for (k = 0; k <= PUSH_KINDS - 1; k++)
		{
			if (((SPR *)actor->sprite)->entries[0]->animationId == PushKinds[k])
			{
				x = (PIXEL_PART(leader->x.fixed) + PUSH_STEP_X(PushLeaderSteps[*direction])) >> 4;
				z = (PIXEL_PART(leader->z.fixed) + PUSH_STEP_Z(PushLeaderSteps[*direction])) >> 4;
				left   = (PIXEL_PART(actor->x.fixed) + PushRectangles[k][0]) >> 4;
				top    = (PIXEL_PART(actor->z.fixed) + PushRectangles[k][1]) >> 4;
				right  = (PIXEL_PART(actor->x.fixed) + PushRectangles[k][2]) >> 4;
				bottom = (PIXEL_PART(actor->z.fixed) + PushRectangles[k][3]) >> 4;
				if (left <= x && x < right && top <= z && z < bottom)
				{
					*kind = k;
					// An odd kind lies north to south.
					if (k & 1)
					{
						if (left != FIELD_METATILE(leader->x.fixed))
						{
							*slot = i;
							return actor;
						}
					}
					else
					{
						if (top != FIELD_METATILE(leader->z.fixed))
						{
							*slot = i;
							return actor;
						}
					}
				}
			}
		}
	}
	return NULL;
}
