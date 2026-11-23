/// Takes an actor out of play and out of sight: disabled, painted transparent
/// and drawn underfoot.
void Actor_Hide(s32 id) {
  Actor_DisableById(id);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_HIDDEN);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}
