/// Sets the speed of vent index's gust, in 16.16 pixels a frame.
void Vent_SetGust(s32 index, s32 value) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].value = value;
}
