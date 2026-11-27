// Vale's storm and Vale's homes: going indoors through a door.

// Takes the exit in the backdrop fade over 16 ticks, first stopping the storm's lightning while the
// storm rages (FLAG_VALE_STORM).
static void scValeRequestBackdropExit(Sint32 exitNumber)
{
    FIELD_STATE *state;
    if (gmFlagGet(FLAG_VALE_STORM) != 0)
    {
        fdWeatherStopLightning();
    }
    state                  = FieldStateWork;
    state->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    state->transitionFrame = MAP_TRANSITION_TICKS;
    syImportScriptSetExitRequest(exitNumber);
}
