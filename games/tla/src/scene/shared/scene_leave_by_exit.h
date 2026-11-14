/// Leaves by exit 1 after Teleport_StartShimmer and Teleport_PlaySparks, then
/// runs Teleport_StopShimmer.
void Scene_LeaveByExit(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}
