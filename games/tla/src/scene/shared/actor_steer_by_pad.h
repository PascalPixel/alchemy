/// While the leader slides on ice, turns him toward the held direction, at most
/// a sixteenth of a turn a frame; with none held, or a menu open, he looks
/// around.
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
    angle = gSlideOnIce_SteerAngles[(gInput.held >> 4) & 0xf];
  if (angle == -1) {
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
    return;
  }
  turn = (s16)(angle - actor->facing);
  if (turn > FACING_STEP)
    turn = FACING_STEP;
  if (turn < -FACING_STEP)
    turn = -FACING_STEP;
  actor->facing += turn;
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
}
