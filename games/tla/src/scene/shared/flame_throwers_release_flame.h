// Releases the flame whose picture the crumbs share (scFlameThrowersCreate).
void scFlameThrowersReleaseFlame(void)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	fdActorRelease(state->sharedFlame);
	state->sharedFlame = NULL;
}
