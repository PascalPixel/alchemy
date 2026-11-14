/// Leaves by exit 1 after Func_02004fac and Teleport_PlaySparks, then runs
/// Func_02005010.
void Scene_LeaveByExit(void) {
  Func_02004fac();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Func_02005010();
}
