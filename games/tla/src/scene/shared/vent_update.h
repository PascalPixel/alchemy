/// Runs the room's hook, then steps the vents: each active one puffs every
/// ten frames.
void Vent_Update(void) {
  struct VentState *state = VENT_STATE;
  struct VentEntry *entry = state->entries;
  s32 i;

  state->hook();
  for (i = sizeof(state->entries) / sizeof(*entry) - 1; i >= 0; i--, entry++) {
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
