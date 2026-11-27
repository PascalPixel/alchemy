/* The ship takes flight: it powers up for at most 20 ticks, less over a place
   a step cue answers, then glows in flight. */
void scShipTakeFlight(void)
{
	ACTOR *ship = fdActorGet(syFieldPartyGetLeaderActor());
	Sint32 i;

	fdScriptPlaySnd(SE_POWER_UP);
	scActorSetFlightMode(ship, SHIP_POWERING_UP);
	for(i = 0; i < 20; i++) {
		Sint32 behavior = fdWorldMapGetBlockMetatileBehavior(&ship->x.fixed);

		if(behavior != 0 && sySceneCueFindActiveRecord(CUE_STEP, behavior) != 0)
			break;
		syWait(1);
	}
	scActorSetFlightMode(ship, SHIP_IN_FLIGHT);
}
