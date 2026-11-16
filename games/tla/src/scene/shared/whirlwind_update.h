/// A whirlwind's update: it travels forward, catches the leader it reaches and
/// ends off screen or at a wall.
void Whirlwind_Update(struct Actor *self) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED position[3];
  FIXED distance;
  s32 near;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, ANIMATION_STEP_NORMAL);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x.fixed, self->z.fixed, 2) != 255) {
    position[0] = self->x.fixed;
    position[1] = self->y.fixed;
    position[2] = self->z.fixed;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > toFIXED(272))
      goto stop;
    if (distance < -toFIXED(32))
      goto stop;
    {
      if (self->riseCounter == 0) {
        distance = leader->y.fixed - self->y.fixed;
        if (distance < 0)
          distance = self->y.fixed - leader->y.fixed;
        near = 0;
        if (Position_FlatDistance(&leader->x.fixed, &self->x.fixed) <= 12 &&
            distance < toFIXED(12))
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedActor = self;
          leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
          {
            s32 delay = 200;
            work->raisedTrigger = delay;
          }
          gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
          self->riseCounter = 1;
          self->riseEnabled = 1;
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
stop:
  self->riseEnabled = 0;
  self->update = NULL;
  Actor_StartScript(self, gWhirlwindEndScript);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  gWhirlwindRiding = 0;
}
