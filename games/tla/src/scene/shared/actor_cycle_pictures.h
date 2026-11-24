/// Flashes an actor's sprite on its timer: white three frames in four, its
/// own colours on the fourth, redrawn each frame.
void Actor_CyclePictures(struct Actor *actor) {
  struct AnimatedSprite *sprite = (struct AnimatedSprite *)actor->sprite;
  struct SpriteAnimation *animation = sprite->animation;

  switch (actor->timer & 3) {
  case 0:
    animation->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = 2;
    break;
  case 1:
    sprite->dirty = 1;
    sprite->flags = 0;
    break;
  case 2:
    animation->colorMode = SPRITE_COLOR_WHITE;
    sprite->dirty = 1;
    sprite->flags = 0;
    break;
  case 3:
    animation->colorMode = SPRITE_COLOR_NORMAL;
    sprite->dirty = 1;
    sprite->flags = 0;
    break;
  }
  actor->timer++;
}
