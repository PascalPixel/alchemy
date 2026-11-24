/// Stops the leader while an action waits.
void Leader_StopIfPending(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (state->busy != 0) {
    Event_SetFlag(FLAG_MENU_OPEN);
    Leader_StopMoving();
    Event_EndAllLastingPsynergy();
    Event_ClearFlag(FLAG_MENU_OPEN);
  }
}
