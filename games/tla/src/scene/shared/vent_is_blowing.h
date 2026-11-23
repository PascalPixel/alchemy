/// Whether vent index is blowing now.
s32 Vent_IsBlowing(s32 index) {
  struct VentWork *work;

  work = VENT_WORK;
  return VENT_AT(work, index)->entries[0].active;
}
