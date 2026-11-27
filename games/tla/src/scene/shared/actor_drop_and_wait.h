/* Lets an actor fall to the ground and waits there: a thump as it lands, ten
   ticks, and it stands still again. */
void fdActorDropAndWait(ACTOR *actor)
{
	Uint8 *flg = &actor->motionFlg;

	*flg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	do {
		sySynch(1);
	} while(actor->velY != 0);
	sySndPlay(SE_THUMP);
	sySynch(10);
	*flg = 0;
}
