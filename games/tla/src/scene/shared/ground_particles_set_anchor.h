/// Lets the rain fall about actor `id`, or, for 0, nowhere on screen.
void GroundParticles_SetAnchor(s32 id) {
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  struct Actor *actor = Actor_GetById(id);

  work->leader = NULL;
  if (id != 0 && actor != 0)
    work->leader = &actor->x.fixed;
}
