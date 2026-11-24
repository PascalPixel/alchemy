/// Sets a vent's actor.
void Vent_SetId(s32 i, s32 id) {
  struct VentState *state = VENT_STATE;

  if ((u32)i <= 3)
    state->ids[i] = id;
}
