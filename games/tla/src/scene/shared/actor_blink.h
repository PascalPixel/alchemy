/* Blinks an actor red, four frames in every eight: the update of a leader a */
/* flame or a fireball has hurt. */
void fdActorBlink(ACTOR *actor) {
	switch(FrameCount & 7) {
	case 0:
		fdActorSetSprColMode(actor, SPRITE_COLOR_RED_TINT);
		break;
	case 4:
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
		break;
	}
}
