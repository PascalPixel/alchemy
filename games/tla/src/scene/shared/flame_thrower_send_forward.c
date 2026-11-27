/* A flame thrower's flame (FLAME_FORWARD), sent south from `position` at */
/* `speed` (syFlameUpdateForward); `offScreen` keeps its sprite's picture */
/* loaded out of sight. */
void scFlameThrowerSendForward(s32 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position[0],
									  position[1], position[2]);

	if(actor != NULL) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = speed;
		fdActorResetScriptedFor(actor, 2);
		actor->update = syFlameUpdateForward;
		if(offScreen)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}
