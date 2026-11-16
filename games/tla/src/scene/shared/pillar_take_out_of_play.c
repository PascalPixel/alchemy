/// Keeps a pillar's sprite in VRAM (retainSpriteCache 3) and takes the pillar
/// out of play.
void Pillar_TakeOutOfPlay(s32 id) {
  Actor_Get(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}
