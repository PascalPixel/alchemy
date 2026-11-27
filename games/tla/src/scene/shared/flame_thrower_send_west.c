/* A flame thrower's flame (FLAME_WEST), sent west from `position` */
/* at `speed` (syFlameUpdateSideways); `offScreen` keeps its sprite's picture */
/* loaded out of sight. */
void scFlameThrowerSendWest(s32 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position[0],
									  position[1], position[2]);

	if(actor != NULL) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = -speed;
		fdActorSetupScriptedPart(actor, 3);
		actor->update = syFlameUpdateSideways;
		if(offScreen)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}
