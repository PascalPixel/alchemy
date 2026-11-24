/// Pressing A at a locked door shows "It seems to be locked."
/// (MsgShamanEntranceLocked): at Shaman Village's houses until Moapa is beaten,
/// and at Contigo's shut doors while its people are away with the wings.
void Scene_ShowEntranceLocked(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);
  Event_End();
}
