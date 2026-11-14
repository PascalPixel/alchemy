/// Whirlwind_Update as the lower floors' copies compile it.
void Whirlwind_UpdateLower(struct Actor *self) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  ((struct FieldSprite *)self->sprite)->priority =
      ((struct FieldSprite *)leader->sprite)->priority;
  if (MapGrid_GetCellBehavior(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Position_FlatDistance(&leader->x, &self->x) <= 12 &&
            distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          work->raisedTrigger = 200;
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      /* IwramMulQ16 reached from IwramIrqMain rather than at its fixed
       * address: */
      /* the address is a linked word read after the cosine, as the native
       */
      /* reads it after the speed (at Iwram_MulQ16 it is built before). */
      self->x += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Trig_Cos(self->facing), self->speedLimit);
      self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;

    /* FAKEMATCH(temporary): one word zero clears the ride byte, then the
     * update. */
    *((u8 *)self + 0x63) = zero;
    self->update = (void *)zero;
  }
  Actor_StartScript(self, gWhirlwindEndScript);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  gWhirlwindRiding = 0;
}
