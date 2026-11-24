/// The ship takes flight: it powers up for at most 20 frames, less over a place
/// a step trigger answers, then glows in flight.
void Ship_TakeFlight(void) {
  struct Actor *ship = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(SE_POWER_UP);
  Actor_SetFlightMode(ship, SHIP_POWERING_UP);
  for (i = 0; i < 20; i++) {
    s32 behavior = WorldMap_GetBlockCellBehavior(&ship->x.fixed);

    if (behavior != 0 &&
        SceneEvent_FindActiveRecord(TRIGGER_STEP, behavior) != 0)
      break;
    System_Wait(1);
  }
  Actor_SetFlightMode(ship, SHIP_IN_FLIGHT);
}
