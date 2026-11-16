/// Whirlwind_Update as the lower floors' copies compile it.
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
  ((struct FieldSprite *)self->sprite)->priority =
      ((struct FieldSprite *)leader->sprite)->priority;
  if (MapGrid_GetCellBehavior(self->x.fixed, self->z.fixed, 2) != 255) {
    position[0] = self->x.fixed;
    position[1] = self->y.fixed;
    position[2] = self->z.fixed;
    Camera_WorldToScreen(position);
    distance = position[0];
    // While it is on screen, by its place across it.
    if (distance <= toFIXED(272)) {
      if (distance >= -toFIXED(32)) {
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
            work->raisedTrigger = 200;
            gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
            self->riseCounter = 1;
            self->riseEnabled = 1;
          }
        }
        /* IwramMulFX reached from IwramIrqMain rather than at its fixed
         * address: */
        /* the address is a linked word read after the cosine, as the native
         */
        /* reads it after the speed (at Iwram_MulFX it is built before). */
        self->x.fixed +=
            ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulFXOffset))(
                Trig_Cos(self->facing), self->speedLimit);
        self->z.fixed +=
            ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulFXOffset))(
                Math_Sin(self->facing), self->speedLimit);
        return;
      }
    }
  }
  self->riseEnabled = 0;
  self->update = NULL;
  Actor_StartScript(self, gWhirlwindEndScript);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  gWhirlwindRiding = 0;
}
