/// Turns an actor toward the direction pad, at most a sixteenth of a turn a
/// frame.
void Actor_SteerByPad(struct Actor *actor) {
  s32 angle;
  s32 turn;

  if (actor->timer != 0) {
    actor->timer--;
    return;
  }
  actor->steeringFlags = 0;
  if (Event_GetFlag(FLAG_MENU_OPEN))
    angle = -1;
  else
    angle = gSlideOnIce_SteerAngles[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Actor_SelectAnimation(actor, 9);
    return;
  }
  turn = (s16)(angle - actor->facing);
  if (turn > TRIG_TURN / 16)
    turn = TRIG_TURN / 16;
  if (turn < -(TRIG_TURN / 16))
    turn = -(TRIG_TURN / 16);
  actor->facing += turn;
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
}
