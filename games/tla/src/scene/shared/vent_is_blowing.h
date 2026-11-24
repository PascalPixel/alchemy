/// Whether vent index is blowing now.
s32 Vent_IsBlowing(s32 index) {
  struct VentState *state;

  state = VENT_STATE;
  return VENT_AT(state, index)->entries[0].active;
}
