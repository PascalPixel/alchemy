/// Searches round the leader for an event, for up to 20 frames.
void Leader_Search(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(SE_POWER_UP);
  Actor_SetFlightMode(actor, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = WorldMap_GetBlockPlace(&actor->x.fixed);

    if (hit != 0 && SceneEvent_FindActiveRecord(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Actor_SetFlightMode(actor, 2);
}
