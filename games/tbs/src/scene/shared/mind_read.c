// Kolima and Log Rooms A: Mind Read cast in a script.

// The caster begins Mind Read on the target.
static void scStartMindRead(Sint32 casterId, Sint32 targetId)
{
	fdPsynergyLoadResources(ABILITY_MIND_READ, 1);
	fdPsynergySetupActorPair(casterId, targetId);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
	sySynch(1);
}

// Finishes Mind Read and releases its effect actors.
static void scEndMindRead(void)
{
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_FINISH);
	fdPsynergyStopCasterUpdate();
	fdPsynergyEnd();
}
