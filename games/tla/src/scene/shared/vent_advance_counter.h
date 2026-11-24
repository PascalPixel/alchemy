/// Steps the vents' counter, wrapping at `limit`.
s32 Vent_AdvanceCounter(s32 limit) {
  struct VentState *state = VENT_STATE;

  if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
    if (++state->counter >= limit)
      state->counter = 0;
  }
  return state->counter;
}
