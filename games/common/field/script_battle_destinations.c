/* fdScriptSetAfterBattleDestination to fdQueueMusic, the same in both games.
   The Broken Seal's field/script_commands_requests.c and The Lost Age's
   field/field_mode_requests.c include it where the functions stand. */

/* Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
   afterBattleEntrance), where the party goes once the coming battle is won. */
void fdScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance)
{
	MainRoutineStateWork.afterBattleScene = sceneId;
	MainRoutineStateWork.afterBattleEntrance = entrance;
}

/* Sets the defeat destination (MainRoutineStateWork.defeatScene and
   defeatEntrance), where the party goes if it loses the coming battle. */
void fdScriptSetDefeatDestination(Sint32 sceneId, Sint32 entrance)
{
	MainRoutineStateWork.defeatScene = sceneId;
	MainRoutineStateWork.defeatEntrance = entrance;
}

/* Makes the script leave for entrance entrance of scene sceneId: sets
   MainRoutineStateWork.next and entrance and raises the exit request
   EXIT_KEEP_DESTINATION, so the field mode leaves on its next pass. */
void fdScriptLeaveFor(Uint16 sceneId, Uint16 entrance)
{
	FIELD_STATE *state = FieldStateWork;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
	state->exit = EXIT_KEEP_DESTINATION;
}

/* Plays SE_STOP_EXIT_SE, then sndId (SOUND_FADE_OUT_ACTION_SE for -1), and
   remembers sndId for fdPlayQueuedMusic in the field state's queuedSnd; -1 is
   remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE now and nothing
   later. */
void fdQueueMusic(Sint32 sndId)
{
	FieldStateWork->queuedSnd = sndId;
	if((Sint16)sndId == -1) {
		sndId = SOUND_FADE_OUT_ACTION_SE;
	}
	sySndPlayStub(SE_STOP_EXIT_SE);
	sySndPlayStub(sndId);
}
