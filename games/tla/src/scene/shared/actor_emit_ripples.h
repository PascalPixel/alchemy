/* Ripples on the water over an actor, as over Aqua Rock's way inside once its
 */
/* Water Stone is doused: every sixteenth frame a blue ring */
/* (SPRITE_SPLASH_RING's animation 2) widens from 7.5 pixels under it, rising */
/* a quarter of a pixel a frame for 31 frames (RippleScript). */
void fdActorEmitRipples(ACTOR *actor) {
	s32 phase = FrameCount & 15;

	if(phase == 0) {
		ACTOR *child = scFieldActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
										  actor->y.fixed, actor->z.fixed);

		if(child != 0) {
			FIELD_SPR *sprite = child->spr;

			fdActorStartScript(child, RippleScript);
			child->motionFlg = 0;
			child->y.fixed = -(toFIXED(15) / 2);
			child->layer = 1;
			if(sprite != 0) {
				fdSprStartAnimation(sprite, 2);
				sprite->flg = 0;
				sprite->pri = 2;
			}
		}
	}
}
