/* @file */
/* The plain push of Altmiller Cave's fallen columns and Crossbone Isle's two */
/* logs on the grass. */

/* Walking into a pushed thing moves it a cell on, if the way beyond is clear.
 */
static void scPushedActorPushIfClear(void) {
	PUSHED_ACTOR_PROBE probe;

	fdEventStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		scPushedActorRoll(probe);
	}
	fdEventEnd();
}
