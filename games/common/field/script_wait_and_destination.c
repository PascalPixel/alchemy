/* fdScriptWaitTransitionTicks and fdScriptSetDestination, the same in both
   games. The Broken Seal's field/script_commands_script.c and The Lost Age's
   field/field_mode_script.c include it where the functions stand. */

/* The script command "wait for screen": waits as long as the transition runs
   (transitionFrm). */
void fdScriptWaitTransitionTicks(void)
{
	sySynch(FieldStateWork->transitionFrm);
}

/* Makes the script leave for the given entrance of the given scene: raises the
   exit request EXIT_KEEP_DESTINATION and writes MainRoutineStateWork.next and
   entrance. */
void fdScriptSetDestination(Sint32 sceneId, Sint32 entrance)
{
	FieldStateWork->exit = EXIT_KEEP_DESTINATION;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
}
