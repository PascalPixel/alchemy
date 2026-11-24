#include "whirlwind_catch.h"

/// A whirlwind's update: it travels forward, catches the leader it reaches and
/// ends off screen or at a wall.
void Whirlwind_Update(struct Actor *self) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  FIXED position[3];
  FIXED distance;
  s32 near;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, ANIM_STEP_NORMAL);
  /* FAKEMATCH(dead): the timer stepped through its own address; as
   * ++self->timer. */
  if ((++*&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  ((struct FieldSprite *)self->sprite)->priority =
      ((struct FieldSprite *)leader->sprite)->priority;
  if (MapGrid_GetCellBehavior(self->x.fixed, self->z.fixed,
                              MAPGRID_LAYER_OVERHEAD) != CELL_BEHAVIOR_WALL) {
    position[0] = self->x.fixed;
    position[1] = self->y.fixed;
    position[2] = self->z.fixed;
    Camera_WorldToScreen(position);
    if (position[0] <= toFIXED(DISPLAY_WIDTH + 32)) {
      if (position[0] >= -toFIXED(32)) {
        if (self->sceneCounter == 0) {
          distance = leader->y.fixed - self->y.fixed;
          if (distance < 0)
            distance = self->y.fixed - leader->y.fixed;
          near = 0;
          if (Position_FlatDistance(&leader->x.fixed, &self->x.fixed) <=
                  WHIRLWIND_CATCH_REACH &&
              distance < toFIXED(WHIRLWIND_CATCH_REACH))
            near = 1;
          if (near && !Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
            leader->facing = self->facing;
            leader->watchedActor = self;
            leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
            state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
            gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
            self->sceneCounter = 1;
            self->sceneByte = 1;
          }
        }
        {
          s32 (*multiply)(s32, s32);
          FIXED cosine = Trig_Cos(self->facing);
          s32 speed = self->speedLimit;
          /* FAKEMATCH(dead): the routine address taken inside a once-only
           * block. */
          do {
            multiply = Iwram_MulFX;
          } while (0);
          self->x.fixed += multiply(cosine, speed);
        }
        self->z.fixed += Iwram_MulFX(Math_Sin(self->facing), self->speedLimit);
        return;
      }
    }
  }
  self->sceneByte = 0;
  self->update = NULL;
  Actor_StartScript(self, gWhirlwindEndScript);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  gWhirlwindRiding = 0;
}

#undef WHIRLWIND_CATCH_REACH
