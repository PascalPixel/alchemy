/// Puts the leader on map layer 0.
void Leader_SetLayer(void) {
  struct Actor *object0;

  object0 = Actor_Get(gMainRoutineWork.leaderActorId);
  object0->terrainId = 0;
  return;
}
