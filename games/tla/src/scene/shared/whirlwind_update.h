#include "system.h"
#include "whirlwind_catch.h"

/* A whirlwind's update: it travels forward, catches the leader it reaches and
   ends off screen or at a wall. */
void scWhirlwindUpdate(ACTOR *self)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED		 pos[3];
	FIXED		 distance;
	Sint32		 near;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0) {
		fdActorSetAnimationSpeed(self, 0);
		return;
	}
	fdActorSetAnimationSpeed(self, ANIM_STEP_NORMAL);
	self->tmr++;
	if((self->tmr & 31) == 31)
		fdScriptPlaySnd(SE_WATER_LEVEL);
	((FIELD_SPR *)self->spr)->pri = ((FIELD_SPR *)leader->spr)->pri;
	if(fdMapGridGetMetatileBehavior(self->x.fixed, self->z.fixed,
		   MAPGRID_LAYER_OVERHEAD) != METATILE_BEHAVIOR_WALL) {
		pos[0] = self->x.fixed;
		pos[1] = self->y.fixed;
		pos[2] = self->z.fixed;
		fdCameraWorldToScreen(pos);
		if(pos[0] <= toFIXED(DISPLAY_WIDTH + 32)) {
			if(pos[0] >= -toFIXED(32)) {
				if(self->sceneCnt == 0) {
					distance = leader->y.fixed - self->y.fixed;
					if(distance < 0)
						distance = self->y.fixed - leader->y.fixed;
					near = 0;
					if(scPosFlatDistance(&leader->x.fixed, &self->x.fixed) <=
							WHIRLWIND_CATCH_REACH &&
						distance < toFIXED(WHIRLWIND_CATCH_REACH))
						near = 1;
					if(near && !fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
						leader->facing = self->facing;
						leader->watchedActor = (FIELD_OBJECT *)self;
						leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
						state->raisedCue = RAISED_RIDE_WHIRLWIND;
						MainRoutineStateWork.movementMode =
							LEADER_MOVEMENT_WALK;
						self->sceneCnt = 1;
						self->sceneByte = 1;
					}
				}
				self->x.fixed +=
					syIWRAMMulFX(syCos(self->facing), self->speedLimit);
				self->z.fixed +=
					syIWRAMMulFX(syMathSin(self->facing), self->speedLimit);
				return;
			}
		}
	}
	self->sceneByte = 0;
	self->update = NULL;
	fdActorStartScript(self, WhirlwindEndScript);
	fdScriptPlaySnd(SE_SPARKLING_PASSAGE);
	WhirlwindRiding = 0;
}

#undef WHIRLWIND_CATCH_REACH
