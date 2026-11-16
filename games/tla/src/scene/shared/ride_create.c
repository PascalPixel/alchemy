/// Clears the flame throwers' work (unless the party keeps its place) and
/// makes the flame whose sprite the crumbs share, keeping its VRAM entry.
void Ride_Create(void) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *object;
  struct Sprite *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, sizeof(*work));
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->flameVramBlock = animation->slot;
  animation->retainCacheEntry = 1;
}
