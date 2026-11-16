/// Clears the flame throwers' work (unless the party keeps its place) and
/// makes the flame whose sprite the crumbs share, keeping its VRAM entry.
void Ride_Create(void) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *actor;
  struct Sprite *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, sizeof(*work));
  actor = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  actor->retainSpriteCache = 2;
  Actor_SelectAnimation(actor, 1);
  animation = actor->sprite;
  work->handle = actor;
  work->flameVramBlock = animation->slot;
  animation->retainCacheEntry = 1;
}
