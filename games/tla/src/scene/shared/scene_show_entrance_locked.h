/// "The entrance is locked" (MsgShamanEntranceLocked).
void Scene_ShowEntranceLocked(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Event_End();
}
