/* A flame's update as it flies along z, slowing, growing and turning as it
   goes: on reaching the leader where he stands it sets the way he is knocked
   back, passes under his feet and rises away, and raises the hit cue; on
   reaching a slot's actor it rises away the same, unless the slot is blocked,
   which marks the state blocked. */
void scFlameUpdateForward(ACTOR *flame)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	ACTOR			*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE		*event = Ram_HeapSlots->fieldState;
	FIXED			 dx = flame->bounce;
	FIXED			 dz;
	Sint32			 i;
	Sint32			 id;

	flame->x.fixed += dx;
	flame->y.fixed += flame->gravity;
	dz = flame->sceneWord;
	flame->z.fixed += dz;
	flame->bounce = dx - dx / 18;
	flame->sceneWord = dz - dz / 16;
	flame->scaleX += flame->speedLimit;
	flame->scaleY += flame->acceleration;
	((FIELD_SPR *)flame->spr)->rotation += flame->tmr;
	if(actor->y.fixed == actor->groundHeight &&
		fdActorIsTouching(flame, actor) &&
		MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		if(flame->sceneWord != 0) {
			state->dir.x = 0;
			if(flame->sceneWord > 0)
				state->dir.z = 1;
			else
				state->dir.z = -1;
		}
		flame->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		{
			Sint32 speed = flame->sceneWord;

			flame->sceneWord = 0;
			flame->z.fixed -= speed;
			flame->gravity += speed >> 2;
		}
		state->active = 1;
		event->stepCue = state->hitCue;
	}
	for(i = 0; i < 4 && (id = state->slots[i].id) != 0; i++) {
		actor = fdActorGet(id);

		if(actor->y.fixed == actor->groundHeight &&
			fdActorIsBeside(flame, actor)) {
			if(state->slots[i].blocked == 0) {
				FIXED speed;

				flame->priFlg = ACTOR_PRIORITY_UNDERFOOT;
				speed = flame->sceneWord;
				flame->sceneWord = 0;
				flame->z.fixed -= speed;
				flame->gravity += speed >> 2;
			} else {
				state->blocked = 1;
			}
		}
	}
}
