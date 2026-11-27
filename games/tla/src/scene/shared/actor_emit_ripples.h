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
			FIELD_SPR *sprite = child->sprite;

			fdActorStartScript(child, RippleScript);
			child->motionFlags = 0;
			child->y.fixed = -(toFIXED(15) / 2);
			child->layer = 1;
			if(sprite != 0) {
				fdSpriteStartAnimation(sprite, 2);
				sprite->flags = 0;
				sprite->priority = 2;
			}
		}
	}
}
