/// A splash at the actor's feet as it wades, on its sprite's priority.
void Actor_SpawnWadingSplash(struct Actor *actor) {
  struct Actor *child = FieldActor_Create(SPRITE_WADING_SPLASH, actor->x.fixed,
                                          actor->y.fixed + toFIXED(4),
                                          actor->z.fixed + toFIXED(4));
  struct FieldSprite *source = actor->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->groundHeight = actor->groundHeight;
    Actor_StartScript(child, gWadingSplash_Script);
    child->motionFlags = 0;
    if (sprite != 0) {
      Sprite_StartAnimation(sprite, 1);
      sprite->flags = 0;
      sprite->priority = source->priority;
    }
  }
}
