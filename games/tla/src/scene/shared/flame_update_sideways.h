/* scFlameUpdateForward for a flame flying east or west: it goes along x, and a
   leader it reaches is knocked back along x. */
void scFlameUpdateSideways(ACTOR *actor)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	ACTOR			*other = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE		*event = RamHeapSlots->fieldState;
	FIXED			 dx = actor->bounce;
	FIXED			 dz;
	Sint32			 i;
	KNOCK_BACK_DIRECTION *direction;

	actor->z.fixed += dx;
	actor->y.fixed += actor->gravity;
	dz = actor->sceneWord;
	actor->x.fixed += dz;
	actor->bounce = dx - dx / 18;
	actor->sceneWord = dz - dz / 16;
	actor->scaleX += actor->speedLimit;
	actor->scaleY += actor->acceleration;
	((FIELD_SPR *)actor->sprite)->rotation += actor->timer;
	if(other->y.fixed == other->groundHeight &&
		fdActorIsTouching(actor, other) &&
		MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		if(actor->sceneWord != 0) {
			state->direction.z = 0;
			direction = &state->direction;
			if(actor->sceneWord > 0)
				direction->x = 1;
			else
				direction->x = -1;
		}
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor->z.fixed -= actor->bounce;
		actor->gravity += actor->bounce >> 2;
		actor->sceneWord = 0;
		state->active = 1;
		event->stepCue = state->hitCue;
	}
	for(i = 0; i < 4; i++) {
		Sint32 id = state->slots[i].id;

		if(id == 0)
			break;
		other = fdActorGet(id);
		if(other->y.fixed == other->groundHeight &&
			fdActorIsBeside(actor, other)) {
			if(state->slots[i].blocked == 0) {
				actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
				actor->z.fixed -= actor->sceneWord;
				actor->gravity += actor->sceneWord >> 2;
				actor->sceneWord = 0;
			} else {
				state->blocked = 1;
			}
		}
	}
}
