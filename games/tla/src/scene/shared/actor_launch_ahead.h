/// Launches the actor this one carries (Actor_Attach) 32 pixels ahead of it,
/// the way `userValue` faces, rolling at up to 8 pixels a frame
/// (Actor_RollAndCrumble), with the launch sound.
void Actor_LaunchAhead(struct Actor *self) {
  struct Actor *actor = Actor_Get(self->launchedActorId);
  s32 angle = self->userValue;
  struct FieldSprite *sprite = actor->sprite;
  FIXED position[3];

  position[0] = self->x.fixed;
  position[1] = self->y.fixed - toFIXED(12);
  position[2] = self->z.fixed;
  FieldVector_AddPolarOffset(32, angle, position);
  actor->facing = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],
                                       position[2]);
  actor->update = (ObjectUpdate)Actor_RollAndCrumble;
  actor->steeringFlags = 0;
  actor->speedLimit = toFIXED(8);
  actor->acceleration = toFIXED(1) / 8;
  sprite->rotation = angle - DEGtoANG(90);
  actor->scaleX = 0;
  actor->scaleY = 0;
  actor->timer = 0;
  actor->watchedActor = self->watchedActor;
  Event_PlaySound(SE_LAUNCH);
}
