/// Keeps a pillar's sprite in VRAM, its picture composed, and takes the pillar
/// out of play.
void Pillar_TakeOutOfPlay(s32 id) {
  Actor_Get(id)->retainSpriteCache =
      ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
  Actor_DisableById(id);
}
