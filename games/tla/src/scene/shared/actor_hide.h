// Takes an actor out of play and out of sight: disabled, painted transparent and drawn underfoot.
void scActorHide(Sint32 id)
{
	fdActorDisableById(id);
	fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_HIDDEN);
	fdActorSetSpritePriority(id, 1);
	fdActorGetById(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}
