/// Stops the leader while an action waits.
void Leader_StopIfPending(void) {
  u8 *event = Ram_HeapSlots->fieldWork;

  if (S16_AT(event, 0xca2)) {
    Flag_Set(FLAG_MENU_OPEN);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Flag_Clear(FLAG_MENU_OPEN);
  }
}
