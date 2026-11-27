// While the leader slides on ice, turns him toward the held direction, at most a sixteenth of a
// turn a tick; with none held, or a menu open, he looks around.
void fdActorSteerByPad(ACTOR *actor)
{
	Sint32 angle;
	Sint32 turn;

	if (actor->timer != 0)
	{
		actor->timer--;
		return;
	}
	actor->steeringFlags = 0;
	if (fdScriptGetFlag(FLAG_MENU_OPEN))
		angle = -1;
	else
		angle = SlideOnIceSteerAngles[(PerWork.data >> 4) & 0xf];
	if (angle == -1)
	{
		fdActorSelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
		return;
	}
	turn = (Sint16)(angle - actor->facing);
	if (turn > FACING_STEP)
		turn = FACING_STEP;
	if (turn < -FACING_STEP)
		turn = -FACING_STEP;
	actor->facing += turn;
	fdActorSelectAnimation(actor, ANIM_WALK);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
}
