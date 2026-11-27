/* While the leader slides on ice, turns him toward the held direction, at most
 */
/* a sixteenth of a turn a frame; with none held, or a menu open, he looks */
/* around. */
void fdActorSteerByPad(ACTOR *actor) {
	s32 angle;
	s32 turn;

	if(actor->tmr != 0) {
		actor->tmr--;
		return;
	}
	actor->steeringFlg = 0;
	if(fdEventGetFlag(FLAG_MENU_OPEN))
		angle = -1;
	else
		angle = SlideOnIce_SteerAngles[(InputWork.held >> 4) & 0xf];
	if(angle == -1) {
		fdActorSelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
		return;
	}
	turn = (s16)(angle - actor->facing);
	if(turn > FACING_STEP)
		turn = FACING_STEP;
	if(turn < -FACING_STEP)
		turn = -FACING_STEP;
	actor->facing += turn;
	fdActorSelectAnimation(actor, ANIM_WALK);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
}
