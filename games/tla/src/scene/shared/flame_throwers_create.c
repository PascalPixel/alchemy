/// Clears the flame throwers' state (unless the party keeps its place) and
/// makes the flame whose sprite the crumbs share, keeping its VRAM entry.
void FlameThrowers_Create(void) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;
  struct Actor *actor;
  struct Sprite *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(state, sizeof(*state));
  actor = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  actor->retainSpriteCache = 2;
  Actor_SelectAnimation(actor, 1);
  animation = actor->sprite;
  state->handle = actor;
  state->flameVramBlock = animation->slot;
  animation->retainCacheEntry = 1;
}
