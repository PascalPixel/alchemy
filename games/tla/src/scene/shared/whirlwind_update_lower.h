#include "whirlwind_catch.h"

/* Unused: the lower floors' copy of a whirlwind's update (whirlwind_update.h),
 */
/* for nothing in these rooms spawns a whirlwind. It travels forward, catches */
/* the leader it reaches and ends off screen or at a wall. */
void fdWhirlwindUpdate(ACTOR *self) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED position[3];
	FIXED distance;
	s32 near;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0) {
		fdActorSetAnimationSpeed(self, 0);
		return;
	}
	fdActorSetAnimationSpeed(self, ANIM_STEP_NORMAL);
	self->tmr++;
	if((self->tmr & 31) == 31)
		fdEventPlaySnd(SE_WATER_LEVEL);
	((FIELD_SPR *)self->spr)->pri = ((FIELD_SPR *)leader->spr)->pri;
	if(fdMapGridGetCellBehavior(self->x.fixed, self->z.fixed,
								MAPGRID_LAYER_OVERHEAD) != CELL_BEHAVIOR_WALL) {
		position[0] = self->x.fixed;
		position[1] = self->y.fixed;
		position[2] = self->z.fixed;
		fdCameraWorldToScr(position);
		distance = position[0];
		if(distance <= toFIXED(DISPLAY_WIDTH + 32)) {
			if(distance >= -toFIXED(32)) {
				if(self->sceneCnt == 0) {
					distance = leader->y.fixed - self->y.fixed;
					if(distance < 0)
						distance = self->y.fixed - leader->y.fixed;
					near = 0;
					if(scPosFlatDistance(&leader->x.fixed, &self->x.fixed) <=
						   WHIRLWIND_CATCH_REACH &&
					   distance < toFIXED(WHIRLWIND_CATCH_REACH))
						near = 1;
					if(near && !fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
						leader->facing = self->facing;
						leader->watchedActor = (FIELD_OBJECT *)self;
						leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
						state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
						MainRoutineStateWork.movementMode =
							LEADER_MOVEMENT_WALK;
						self->sceneCnt = 1;
						self->sceneByte = 1;
					}
				}
				self->x.fixed +=
					IwramMulFX(syCos(self->facing), self->speedLimit);
				self->z.fixed +=
					IwramMulFX(syMathSin(self->facing), self->speedLimit);
				return;
			}
		}
	}
	self->sceneByte = 0;
	self->update = NULL;
	fdActorStartScript(self, WhirlwindEndScript);
	fdEventPlaySnd(SE_SPARKLING_PASSAGE);
	WhirlwindRiding = 0;
}

#undef WHIRLWIND_CATCH_REACH
