/// Steps the vents: each active one puffs every ten frames.
void Vents_Update(void) {
  struct VentWork *work = VENT_WORK;
  struct VentEntry *entry = work->entries;
  s32 i;

  work->hook();
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->active != 0) {
      if (entry->timer <= 0) {
        Vent_Puff(&entry->x, entry->value);
        entry->timer = 9;
      } else {
        entry->timer--;
      }
    }
  }
}
