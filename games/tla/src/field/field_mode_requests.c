// Battles and destinations a script requests, and the queued sound. Part of field_mode.c.

// Sets up a battle with the formation at row and column of the encounter table.
void fdRequestBattleFromEncounterTable(Sint32 row, Sint32 column)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    state->encounter   = fdEncounterGetFormationAtSlot(row, column);
    if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
        fdEncounterResolveWorldMapZoneAndBackdrop(&fdActorLookup(fdPartyGetLeaderActor())->x.fixed);
    fdEncounterSelectBattleMusic(row, column);
}

// The low bits of fdRequestDjinnBattle's flags: the Djinni's actor.
#define DJINN_ACTOR_MASK 0xff

// Sets up the battle with Djinni djinn, DJINN_PER_ELEMENT to an element; an element's last Djinni
// stands for the one of The Broken Seal's chosen for it (SCENE_STATE_CHOSEN_DJINN; none: nothing
// happens). The low 8 bits of flags are the Djinni's actor, which hops first unless DJINN_NO_HOP is
// set.
void fdRequestDjinnBattle(Uint32 flags, Uint32 djinn)
{
    FIELD_STATE *state   = RamHeapSlots->fieldState;
    Uint32       skipHop = flags & DJINN_NO_HOP;
    Uint32       element;
    Sint32       index;

    flags   &= DJINN_ACTOR_MASK;
    element  = djinn / DJINN_PER_ELEMENT;
    index    = djinn % DJINN_PER_ELEMENT;

    if (index == DJINN_PER_ELEMENT - 1)
    {
        index = SceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];
        if (index == 0)
            return;
        index--;
    }
    djinn = element * DJINN_PER_ELEMENT + index;
    if (skipHop == 0)
        fdDjinnRunBattleStartHop(flags);
    // DJINN_NO_HOP rides along: it tells fdDjinnRestoreActorAfterBattle to put the Djinni back 32
    // pixels north of the leader, not at its placement's position.
    MainRoutineStateWork.pendingDjinni = (djinn + DJINNI_BATTLE_SCRIPT_FIRST) | skipHop;
    state->encounter                   = fdEncounterGetDjinnFormation(djinn);
    if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
        fdEncounterResolveWorldMapZoneAndBackdrop(&fdActorLookup(fdPartyGetLeaderActor())->x.fixed);
    fdEncounterSelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

// Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
// afterBattleEntrance), where the party goes once the coming battle is won.
void fdScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance)
{
    MainRoutineStateWork.afterBattleScene    = sceneId;
    MainRoutineStateWork.afterBattleEntrance = entrance;
}

// Sets the defeat destination (MainRoutineStateWork.defeatScene and defeatEntrance), where the
// party goes if it loses the coming battle.
void fdScriptSetDefeatDestination(Sint32 sceneId, Sint32 entrance)
{
    MainRoutineStateWork.defeatScene    = sceneId;
    MainRoutineStateWork.defeatEntrance = entrance;
}

// Makes the script leave for entrance entrance of scene sceneId: sets MainRoutineStateWork.next and
// entrance and raises the exit request EXIT_KEEP_DESTINATION, so the field mode leaves on its next
// pass.
void fdScriptLeaveFor(Uint16 sceneId, Uint16 entrance)
{
    FIELD_STATE *state            = FieldStateWork;
    MainRoutineStateWork.next     = sceneId;
    MainRoutineStateWork.entrance = entrance;
    state->exit                   = EXIT_KEEP_DESTINATION;
}

// Plays SE_STOP_EXIT_SE, then soundId (SOUND_FADE_OUT_ACTION_SE for -1), and remembers soundId for
// fdPlayQueuedMusic in the field state's queuedSound; -1 is remembered as none, so it plays
// SOUND_FADE_OUT_ACTION_SE now and nothing later.
void fdQueueMusic(Sint32 soundId)
{
    FieldStateWork->queuedSound = soundId;
    if ((Sint16)soundId == -1)
    {
        soundId = SOUND_FADE_OUT_ACTION_SE;
    }
    sySoundPlayStub(SE_STOP_EXIT_SE);
    sySoundPlayStub(soundId);
}

// A script that does nothing.
void fdScriptNoOperation(void)
{
}

// Plays the sound fdQueueMusic remembered, if any.
void fdPlayQueuedMusic(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    if (state != NULL)
    {
        Sint16 sound = state->queuedSound;

        if (sound != -1)
            sySoundPlayStub(sound);
    }
}

// Shows message in a script and clears FLAG_NOTHING_HAPPENS; when someone in the party has Scoop,
// the field menu then opens.
void fdScriptShowMessageAndOfferScoop(Sint32 msg)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    mnFieldMessageShowStub(msg, FIELD_MESSAGE_ZOOM);
    syGameFlagClear(FLAG_NOTHING_HAPPENS);
    fdScriptEnd();
    if (gmPartyFindOwnerWithActionStub(ABILITY_SCOOP) != -1)
        FieldStateWork->fieldMenu = 1;
}

// Plays sound when the actor is in view: within 152 pixels across and 112 up or down the screen of
// the actor the camera follows.
void fdActorPlaySoundInView(ACTOR *actor, Sint32 sound)
{
    ACTOR *focus = FieldStateWork->camera;

    if (actor == 0 || focus == 0)
        return;
    if (ABS(focus->x.fixed - actor->x.fixed) > toFIXED(152))
        return;
    if (ABS((focus->z.fixed - focus->y.fixed) - (actor->z.fixed - actor->y.fixed)) > toFIXED(112))
        return;
    sySoundPlayStub(sound);
}

// Counts one more weird answer Felix gives (MainRoutineStateWork.weirdAnswers).
void fdScriptCountAnswer(void)
{
    MainRoutineStateWork.weirdAnswers++;
}
