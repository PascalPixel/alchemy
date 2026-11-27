/* Readies a vent's puff: it stops, is drawn behind the actors around it, its
   sprite at OBJ `priority`, shadowless and tinted SPRITE_COLOR_RED_TINT, and
   breaks up (the dust cloud's animation 2) without spinning or growing until
   DustBurstScript releases it, 44 ticks on. */
void scActorStartScripted(ACTOR *actor, Sint32 priority)
{
	actor->motionFlags = 0;
	actor->collisionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	((FIELD_SPR *)actor->sprite)->priority = priority;
	fdActorSetSpriteFlags(actor, 0);
	fdActorSelectAnimation(actor, 2);
	fdActorStartScript(actor, DustBurstScript);
	scFieldActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
	actor->timer = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
