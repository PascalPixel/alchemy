/// Sets the speed of vent index's gust, in 16.16 pixels a frame.
void Vent_SetGust(s32 index, s32 value) {
  struct VentState *state;

  state = VENT_STATE;
  VENT_AT(state, index)->entries[0].value = value;
}
