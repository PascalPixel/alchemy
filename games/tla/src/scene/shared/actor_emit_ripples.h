/* Ripples on the water over an actor, as over Aqua Rock's way inside once its
   Water Stone is doused: every sixteenth tick a blue ring
   (SPRITE_SPLASH_RING's animation 2) widens from 7.5 pixels under it, rising a
   quarter of a pixel a tick for 31 ticks (RippleScript). */
void scActorEmitRipples(ACTOR *actor)
{
	Sint32 phase = TickCount & 15;

	if(phase == 0) {
		ACTOR *child = scFieldActorCreate(
			SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed, actor->z.fixed);

		if(child != 0) {
			FIELD_SPR *spr = child->spr;

			fdActorStartScript(child, RippleScript);
			child->motionFlg = 0;
			child->y.fixed = -(toFIXED(15) / 2);
			child->layer = 1;
			if(spr != 0) {
				fdSprStartAnimation(spr, 2);
				spr->flg = 0;
				spr->pri = 2;
			}
		}
	}
}
