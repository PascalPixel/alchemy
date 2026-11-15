/// Launches an actor ahead of another, 32 pixels out.
void Actor_LaunchAhead(struct Actor *self) {
  struct Actor *actor = Actor_Get(self->riseCounter);
  s32 angle = (s16)self->userValue;
  struct FieldSprite *sprite = actor->sprite;
  /* volatile: the call reads position[0] before [1] and [2] (as a plain */
  /* array, 2 of 86 differ in tla-en). */
  volatile s32 position[3];

  position[0] = self->x.fixed;
  position[1] = self->y.fixed - px(12);
  position[2] = self->z.fixed;
  FieldVector_AddPolarOffset(32, angle, (struct Vector3 *)position);
  actor->facing = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],
                                       position[2]);
  actor->update = (void (*)(void))Actor_RollAndCrumble;
  actor->steeringFlags = 0;
  actor->speedLimit = px(8);
  actor->acceleration = px(1) / 8;
  sprite->rotation = angle - 0x4000;
  actor->scaleX = 0;
  /* A once-only block: its end holds the order around it (as the plain */
  /* statement, 4 of 86 differ in tla-en). */
  do {
    actor->scaleY = 0;
  } while (0);
  actor->timer = 0;
  actor->watchedActor = self->watchedActor;
  Event_PlaySound(SE_104);
}
