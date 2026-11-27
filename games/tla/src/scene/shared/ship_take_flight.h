/* The ship takes flight: it powers up for at most 20 frames, less over a place
 */
/* a step trigger answers, then glows in flight. */
void syShipTakeFlight(void) {
	ACTOR *ship = fdActorGet(syFieldPartyGetLeaderActor());
	s32 i;

	fdEventPlaySnd(SE_POWER_UP);
	scActorSetFlightMode(ship, SHIP_POWERING_UP);
	for(i = 0; i < 20; i++) {
		s32 behavior = fdWorldMapGetBlockCellBehavior(&ship->x.fixed);

		if(behavior != 0 &&
		   sySceneEventFindActiveRecord(TRIGGER_STEP, behavior) != 0)
			break;
		syWait(1);
	}
	scActorSetFlightMode(ship, SHIP_IN_FLIGHT);
}
