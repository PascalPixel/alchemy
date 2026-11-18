/// Unused: gives a moai its water, drawn in front of it: the pool in the top of
/// its head, rippling (animation 2).
UNUSED void Moai_AddWater(s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (actor != 0) {
    struct SpriteAnimation *water =
        (struct SpriteAnimation *)Sprite_AddAnimation(actor->sprite,
                                                      SPRITE_MOAI_WATER);

    water->colorMode = SPRITE_COLOR_NORMAL;
    water->priority = 0;
    Actor_SelectAnimation(actor, 0);
    Actor_SelectAnimation(actor, 2);
  }
}
