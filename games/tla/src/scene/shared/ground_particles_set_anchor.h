/// Ties the sparkles to actor `id`, or to none.
void GroundParticles_SetAnchor(s32 id) {
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  struct Actor *actor = Actor_GetById(id);

  work->leader = NULL;
  if (id != 0 && actor != 0)
    work->leader = &actor->x.fixed;
}
