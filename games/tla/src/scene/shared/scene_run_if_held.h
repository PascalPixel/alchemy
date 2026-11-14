/// Runs a chain of calls on the held actor when there is one.
void Scene_RunIfHeld(void) {
  struct Actor *object;

  if (Leader_CanPushObjectAhead() != 0) {
    object = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner));
    Pillars_LowerHeldCells(gMadraCatacombs_ClimbablePillar);
    Leader_PushObjectAhead();
    MadraCatacombs_SettleClimbablePillar(object);
  }
}
