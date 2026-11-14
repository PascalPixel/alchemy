/// Stops the leader while an action waits.
void Leader_StopIfPending(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->busy != 0) {
    Event_SetFlag(FLAG_MENU_OPEN);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(FLAG_MENU_OPEN);
  }
}
