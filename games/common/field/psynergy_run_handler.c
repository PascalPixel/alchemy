/* fdPsynergyRunHandler, the same in both games. The Broken Seal's
   field/script_commands_field_psynergy.c and The Lost Age's
   field/script_commands_psynergy_dispatch.c include it where the function
   stands. */

/* Runs one step of the scene's field Psynergy: cast with no target, cast on the
   chosen target (secondActorId), Mind Read's end, or what a lasting Psynergy
   does when it ends. */
void fdPsynergyRunHandler(Sint32 kind)
{
	if(kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
		fdPsynergyRun();
	else if(kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
		fdPsynergyDispatch();
	else if(kind == FIELD_PSYNERGY_HANDLER_FINISH)
		fdPsynergyFinishMindRead();
	else if(kind == FIELD_PSYNERGY_HANDLER_EXTRA)
		fdPsynergyRunExtraHandler();
}
