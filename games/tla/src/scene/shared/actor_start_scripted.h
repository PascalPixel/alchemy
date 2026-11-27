/* Readies a vent's puff: it stops, is drawn behind the actors around it, its
   sprite at OBJ `pri`, shadowless and tinted SPRITE_COLOR_RED_TINT, and
   breaks up (the dust cloud's animation 2) without spinning or growing until
   DustBurstScript releases it, 44 ticks on. */
void scActorStartScripted(ACTOR *actor, Sint32 pri)
{
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	((FIELD_SPR *)actor->spr)->pri = pri;
	fdActorSetSprFlags(actor, 0);
	fdActorSelectAnimation(actor, 2);
	fdActorStartScript(actor, DustBurstScript);
	scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
