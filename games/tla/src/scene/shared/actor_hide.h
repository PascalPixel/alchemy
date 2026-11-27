/* Takes an actor out of play and out of sight: disabled, painted transparent
   and drawn underfoot. */
void scActorHide(Sint32 id)
{
	fdActorDisableById(id);
	fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_HIDDEN);
	fdActorSetSprPriority(id, 1);
	fdActorGetById(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
}
