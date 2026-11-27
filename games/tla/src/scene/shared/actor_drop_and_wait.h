/* Lets an actor fall to the ground and waits there: a thump as it lands, ten */
/* frames, and it stands still again. */
void fdActorDropAndWait(ACTOR *actor) {
	u8 *flags = &actor->motionFlg;

	*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	do {
		sySynch(1);
	} while(actor->velY != 0);
	sySndPlay(SE_THUMP);
	sySynch(10);
	*flags = 0;
}
