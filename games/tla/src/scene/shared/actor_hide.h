/// Hides an actor behind the room.
void Actor_Hide(s32 id) {
  Actor_DisableById(id);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_HIDDEN);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}
