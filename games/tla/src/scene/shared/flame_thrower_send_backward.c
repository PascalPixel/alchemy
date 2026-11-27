/* A flame thrower's flame (FLAME_BACKWARD), sent north from `position` at
   `speed` (scFlameUpdateForward); `offScreen` keeps its sprite's picture loaded
   out of sight. */
void scFlameThrowerSendBackward(
	Sint32 *position, Sint32 speed, Sint32 offScreen)
{
	ACTOR *actor = scFieldActorCreate(
		SPRITE_BLAZE_FLAME, position[0], position[1], position[2]);

	if(actor != NULL) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = -speed;
		scActorSetupScriptedPart(actor, 3);
		actor->update = scFlameUpdateForward;
		if(offScreen)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}
