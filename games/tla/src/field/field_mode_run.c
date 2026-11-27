// The field mode: running a scene until the party leaves it, and its entrance. Part of
// field_mode.c.

#include "sound/stubs.h"

extern Uint8 MapGridMetatileBuffer[];

// UB: declared returning Sint32; the definition (main_routine.c) returns Sint16.
#if FIX_FUNCTION_SIGNATURES
Sint16 gmFieldGetSceneAttributes(Sint32 scene);
#else
Sint32 gmFieldGetSceneAttributes(Sint32 scene);
#endif
Bool fdLeaderStepOntoRope(void);
Bool fdLeaderStepOffRope(void);
Bool fdLeaderStepOntoTightrope(void);
Bool fdLeaderStepOffTightrope(void);

// The two nibbles of a scene's attributes (gmFieldGetSceneAttributes).
#define NIBBLE_LOW(value)      ((value) & 15)
#define NIBBLE_HIGH(value)     (((value) >> 4) & 15)
// The hazard count's limit each scene starts with, in steps on hazard metatiles: the hazard deals
// its damage each time the count reaches it.
#define HAZARD_LIMIT_DEFAULT   16
// A Teleport destination's map screen marker (WORLD_MAP_PLACE.value's high nibble): 1 for a place
// the party walks to; 2 for Lemuria's, which it reaches aboard the ship.
#define PLACE_MARKER_ON_FOOT   (1 << 4)
#define PLACE_MARKER_ABOARD    (2 << 4)
// The sprite VRAM blocks the Pause menu needs free, or the field's sprites stop drawing while it
// runs.
#define PAUSE_MENU_VRAM_BLOCKS 136
// A map entrance stream's row: the entrance, its x and z, and its flags byte.
#define MAP_ENTRANCE_ROW_BYTES 6

// The field mode: runs the current scene, which the party entered by entrance, until it leaves;
// returns the exit kind it left by, for gmTakeExit, or EXIT_KEEP_DESTINATION when the next scene is
// already in MainRoutineStateWork: a random battle, a party that fell outside battle, Retreat or a
// script's destination.
Sint32 fdModeRun(void)
{
    FIELD_STATE        *state;
    MAIN_ROUTINE_STATE *party;
    ACTOR              *leader;
    FIELD_UNIT         *unit;
    Sint32              result;
    Sint32              arrivalState;
    Sint32             *leaderId;
    Sint32             *steps;
    Sint16             *entrance;
    Uint16             *layer;
    Sint8              *pending;
    Sint32              code;
    Sint32              mode;
    Uint32              value;
    Sint32              talk;
    Sint32              packed;

    // Set up the scene.
    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
    syWordsClearEntry(state, sizeof(FIELD_STATE));
    syGameFlagClear(FLAG_IN_BATTLE);
    MainRoutineStateWork.returnScene         = MainRoutineStateWork.next;
    MainRoutineStateWork.returnEntrance      = MainRoutineStateWork.entrance;
    MainRoutineStateWork.afterBattleScene    = -1;
    MainRoutineStateWork.afterBattleEntrance = -1;
    MainRoutineStateWork.defeatScene         = -1;
    MainRoutineStateWork.defeatEntrance      = -1;
    MainRoutineStateWork.onUpperLevel        = 0;
    syInitEvent();
    fdDjinnResolvePending(0);
    if (MainRoutineStateWork.mapId == SCENE_CLEAR)
    {
        fdWorldMapInitPerspectiveStub();
        MainRoutineStateWork.perspectiveMap = 1;
        mode                                = ACTOR_SYSTEM_WORLD_MAP;
        fdEncounterSelectMapZones(FALSE);
    }
    else
    {
        fdMapGridLoadMapStub(MainRoutineStateWork.mapId);
        mode = ACTOR_SYSTEM_MAP;
        fdEncounterSelectMapZones(TRUE);
    }
    state->mode = mode;
    if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
        TouchCopiesWork.count = 0;
    else
        fdTileCopiesRestore();
    mnWindowSystemInitFieldStub();
    fdActorSystemInitStub(mode);
    fdClearRequests();
    state->cues = (const void *)SCENE_ENTRIES->cues();
    fdApplyEntrance();
    fdActorResetForLeader((Sint32)SCENE_ENTRIES->placements());
    fdPlaceTreasureSpots();
    if (syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
        fdActorRestore();
    if (MainRoutineStateWork.pendingDjinni != 0)
        fdDjinnRestoreActorAfterBattle();
    state->queuedSound = -1;
    if (mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        fdWorldMapUpdateCurrentTileBlockStub();
        fdWorldMapLoadGraphicsStub(MainRoutineStateWork.x, MainRoutineStateWork.z);
    }
    else
    {
        fdMapApplySetFlagEditsStub();
        SCENE_ENTRIES->prepareMap();
        fdMapGridCenterOnFollowedPointStub();
    }
    // XXX (no evidence): what flag 0x11f means; every scene start clears it, The Broken Seal's does
    // not, and nothing in either game's code, tables, debug flag lists or cleared ranges of flags
    // sets or reads it.
    syGameFlagClear(0x11f);
    fdPaletteFadeInit();
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    // The default fade is from the backdrop colour over 16 ticks; the scene's initialization may
    // choose another.
    state->transition           = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    state->transitionFrame      = MAP_TRANSITION_TICKS;
    state->transitionActive     = 0;
    // fdUpdateWalking scales the leader's speed by this (16.16) into the distance counted towards
    // PP, Djinn recovery and poison: toFIXED(1) / 160 counts each of them on by one about every 160
    // pixels walked.
    state->walkingDistanceScale = toFIXED(1) / 160;
    state->walkingDistance      = 0;
    if (MainRoutineStateWork.sceneType == SCENE_TYPE_TOWN)
    {
        state->walkingDistanceScale = 0;
        syGameFlagSet(FLAG_RETREAT_BLOCKED);
    }
    state->prevMetatile = MapGridMetatileBuffer;
    // A fresh entry (not a return from battle) resets the hazard count to its defaults (damage 1
    // every HAZARD_LIMIT_DEFAULT steps), the stopped actor and Cloak.
    if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
    {
        fdSetRestartPoint();
        MainRoutineStateWork.hazardLimit  = HAZARD_LIMIT_DEFAULT;
        MainRoutineStateWork.hazardMode   = HAZARD_COUNT_ON_METATILES;
        MainRoutineStateWork.hazardDamage = 1;
        MainRoutineStateWork.haltedActor  = -1;
        MainRoutineStateWork.cloaked      = 0;
    }
    fdPsynergyResumeCloakCheck();
    value                     = gmFieldGetSceneAttributes(MainRoutineStateWork.next);
    state->sceneAttributeHigh = NIBBLE_HIGH(value);
    state->sceneAttributeLow  = NIBBLE_LOW(value);

    if (MainRoutineStateWork.teleportState == TELEPORT_ARRIVING)
    {
        fdActorGetById(fdPartyGetLeaderActor())->scaleX = 0;
        fdActorSetSpriteFlagsStub(fdActorGetById(fdPartyGetLeaderActor()), 0);
    }
    fdUpdateHoverReady();
    sySynch(2);
    SCENE_ENTRIES->initialize();
    // The scene's initialization may already have taken an exit.
    code = state->exit;
    if (code != 0)
    {
        result      = code;
        state->exit = 0;
        goto done;
    }
    syGameFlagClear(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the area name and give what was left pending.
    if (fdCountRequests() == 0)
    {
        value = state->transitionActive;
        if (value == 0)
        {
            // XXX: why a Teleport arrival holds 6584 bytes of the battle effect's slot through its
            // two 20-tick pauses. No state of that size is known (Teleport's own take 7824), and
            // what the scene's events take from the heap in those ticks is freed with it
            // (syHeapSlotRelease). A trace of HEAP_SLOTS.nextIwram through a Teleport chosen on the
            // map screen, which no render here stages, would settle it.
            if (MainRoutineStateWork.teleportState == TELEPORT_CHOSEN)
            {
                syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, 6584);
                sySynch(20);
                syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
            }
            fdScreenTransitionStart(state->transition, state->transitionFrame);
            state->transitionActive = 1;
            if (MainRoutineStateWork.teleportState == TELEPORT_CHOSEN)
            {
                syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, 6584);
                sySynch(20);
                syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
            }
            PLTT_BACKDROP = 0;
            // An arrival the last scene asked for (fdEntrancePlayArrival) plays through the fade;
            // with none, wait out half of it.
            if (!fdEntrancePlayArrival())
                sySynch((state->transitionFrame + 1) / 2);
        }
        arrivalState = MainRoutineStateWork.teleportState;
        if (arrivalState == TELEPORT_CHOSEN)
        {
            MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
            fdTeleportPlaySparksAndFade();
            if (MainRoutineStateWork.teleportScene != SCENE_WORLD_MAP)
                MainRoutineStateWork.ownerOverride = 0;
            else if ((value = fdFlagEntriesGetValue(MainRoutineStateWork.teleportEntrance))
                     & PLACE_MARKER_ON_FOOT)
                MainRoutineStateWork.ownerOverride = 0;
            else if (value & PLACE_MARKER_ABOARD)
                MainRoutineStateWork.ownerOverride = 1;
            fdScriptSetDestination(MainRoutineStateWork.teleportScene,
                                   MainRoutineStateWork.teleportEntrance);
        }
        else if (arrivalState == TELEPORT_ARRIVING)
        {
            MainRoutineStateWork.teleportState = TELEPORT_NONE;
            fdTeleportPlayWhirl();
        }
        if (syGameFlagGet(FLAG_SHOW_AREA_NAME))
        {
            syGameFlagClear(FLAG_SHOW_AREA_NAME);
            mnAreaNameWindowShowStub(MainRoutineStateWork.next);
        }
        if (MainRoutineStateWork.pendingDjinni != 0)
        {
            // UB: called with a second argument (1) its definition does not take.
#if FIX_FUNCTION_SIGNATURES
            fdDjinnResolvePending(MainRoutineStateWork.pendingDjinni);
#else
            ((void (*)(Sint32, Sint32))fdDjinnResolvePending)(MainRoutineStateWork.pendingDjinni,
                                                              1);
#endif
            MainRoutineStateWork.pendingDjinni = 0;
        }
        if (MainRoutineStateWork.pendingItem != 0)
        {
            fdWorldMapFreeze();
            if (fdPartyCheckItemLimit(MainRoutineStateWork.pendingItem) == 0)
            {
                fdPartyInventoryGiveItem(MainRoutineStateWork.pendingItem, 0);
            }
            fdWorldMapReloadPendingAreaGraphics();
            MainRoutineStateWork.pendingItem = 0;
        }
    }

    party    = &MainRoutineStateWork;
    leaderId = &MainRoutineStateWork.leaderActorId;
    steps    = &MainRoutineStateWork.encounterSteps;
    entrance = &MainRoutineStateWork.entrance;
    layer    = &MainRoutineStateWork.layer;
    for (;;)
    {
        // Note where the leader is, for the requests to use.
        syGameFlagSet(FLAG_FIELD_HANDLING_REQUESTS);
        leader = fdGetPartyActor();
        fdActorStartIdleStub(leader);
        SET_POSITION(party, leader->x.fixed, 0, leader->z.fixed);
        party->heading = leader->facing;
        *layer         = leader->layer;
        // Handle the pending requests, most urgent first.
        while (fdCountRequests() != 0)
        {
            if (state->exit != 0)
            {
                result      = state->exit;
                state->exit = 0;
                goto done;
            }
            if (state->raisedCue != 0)
            {
                state->busy = 1;
                if (state->raisedCue == FIELD_RAISED_CUE_PARTY_FELL)
                {
                    // Characters fell to poison or hazardous ground.
                    fdLeaderStopMoving();
                    fdWorldMapFreeze();
                    for (value = 0; value < state->fallenCount; value++)
                    {
                        fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAN_BACK);
                        mnMessageArgumentPushStub(state->fallenOwners[value],
                                                  MESSAGE_ARGUMENT_ACTOR);
                        mnFieldMessageShowStub((Sint32)MsgFieldStrengthExhausted,
                                               FIELD_MESSAGE_ZOOM);
                    }
                    if (state->survivorCount == 0)
                    {
                        // The whole party fell: back to the restart point with 1 HP.
                        if (*leaderId == ACTOR_FELIX)
                            fdActorSelectAnimationStub(leader, ANIM_LEADER_FALL_ON_FACE);
                        else
                            fdActorSelectAnimationStub(leader, ANIM_COLLAPSE);
                        sySoundPlayStub(MUS_DEFEATED);
                        mnFieldMessageShowStub((Sint32)MsgFieldPartyAnnihilated,
                                               FIELD_MESSAGE_ZOOM);
                        unit     = syUnitGet(*leaderId);
                        unit->hp = 1;
                        UNIT_UPDATE_GAUGES(unit);
                        {
                            Sint32 restartScene = party->restartScene;
                            Sint32 restartEntrance;

                            party->next     = restartScene;
                            restartEntrance = party->restartEntrance;
                            *entrance       = restartEntrance;
                            fdWorldMapSetReturnLeavingScene((Sint16)restartScene,
                                                            (Sint16)restartEntrance);
                        }
                        result = EXIT_KEEP_DESTINATION;
                        fdWorldMapReloadPendingAreaGraphics();
                        goto done;
                    }
                    fdWorldMapReloadPendingAreaGraphics();
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_TELEPORT)
                {
                    fdLeaderStopMoving();
                    fdMapScreenShow(MAP_SCREEN_TELEPORT);
                    if (MainRoutineStateWork.teleportScene != -1)
                        MainRoutineStateWork.teleportState = TELEPORT_CHOSEN;
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_MAP_SCREEN)
                {
                    fdLeaderStopMoving();
                    // Shown once, to look: fdMapScreenShow always returns 0, so the mode is never
                    // flipped to Teleport's.
                    value = MAP_SCREEN_LOOK;
                    while (fdMapScreenShow(value))
                        value ^= 1;
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_WORLD_MAP_ZOOM)
                {
                    fdLeaderStopMoving();
                    fdWorldMapZoomOutWhileLHeld();
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_ROPE_ON)
                {
                    if (state->heldKeys == PerWork.data)
                    {
                        fdLeaderStepOntoRope();
                        state->fieldMenu = 0;
                        state->pauseMenu = 0;
                        state->aButton   = 0;
                        state->encounter = 0;
                        state->psynergy  = 0;
                    }
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_ROPE_OFF)
                {
                    if (state->heldKeys == PerWork.data)
                    {
                        fdLeaderStepOffRope();
                        state->fieldMenu = 0;
                        state->pauseMenu = 0;
                        state->aButton   = 0;
                        state->encounter = 0;
                        state->psynergy  = 0;
                    }
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_TIGHTROPE_ON)
                {
                    if (state->heldKeys == PerWork.data)
                    {
                        fdLeaderStepOntoTightrope();
                        state->fieldMenu = 0;
                        state->pauseMenu = 0;
                        state->aButton   = 0;
                        state->encounter = 0;
                        state->psynergy  = 0;
                    }
                }
                else if (state->raisedCue == FIELD_RAISED_CUE_TIGHTROPE_OFF)
                {
                    if (state->heldKeys == PerWork.data)
                    {
                        fdLeaderStepOffTightrope();
                        state->fieldMenu = 0;
                        state->pauseMenu = 0;
                        state->aButton   = 0;
                        state->encounter = 0;
                        state->psynergy  = 0;
                    }
                }
                else
                {
                    fdRunRaisedCue(state->raisedCue);
                }
                state->busy      = 0;
                state->raisedCue = 0;
            }
            else if (state->stepCue != 0)
            {
                state->busy = 1;
                fdRunStepCueOrExit(state->stepCue);
                state->busy    = 0;
                state->stepCue = 0;
            }
            else if (state->pushMetatile != 0)
            {
                fdRunPushMetatile(state->pushMetatile);
                state->pushMetatile = 0;
            }
            else if (state->encounter != 0)
            {
                // A random encounter: save the actors and go to the battle.
                fdLeaderStopMoving();
                fdActorTableSnapshot();
                party->returnMusic = -1;
                party->next        = MODE_BATTLE;
                *entrance          = state->encounter;
                state->exit        = EXIT_KEEP_DESTINATION;
                fdStartEncounterTransition(state->encounter);
                state->encounterBias = 0;
                state->encounter     = 0;
                *steps               = 0;
            }
            else if (state->aButton != 0)
            {
                // Talk to the actor ahead, or examine the metatile ahead, or open the field menu if
                // neither has a script.
                mnAreaNameWindowHideStub();
                talk = 0;
                if (MainRoutineStateWork.ownerOverride == 0)
                    value = fdFindNearbyTalkTarget(*leaderId);
                else
                    value = fdFindNearbyTalkTarget(ACTOR_FIRST_PLACED);
                if (value != -1)
                {
                    talk = 1;
                    if ((Sint32)value <= FIELD_TREASURE_ACTOR - 1)
                        talk = fdFindActorTalkCue(value) != 0;
                }
                if (talk)
                {
                    state->talk      = value | FIELD_TALK_REQUEST;
                    state->fieldMenu = 0;
                }
                else if ((talk = fdActorGetCueMetatileAheadOfCurrent()) != 0)
                {
                    state->examineMetatile = talk;
                    state->fieldMenu       = 0;
                }
                else
                {
                    state->fieldMenu = 1;
                }
                state->aButton = 0;
            }
            else if (state->fieldMenu != 0)
            {
                // The field menu (B + Select in debug mode opens the flag editor).
                mnAreaNameWindowHideStub();
                fdLeaderStopMoving();
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                fdWorldMapFreeze();
                syGameFlagSet(FLAG_MENU_OPEN);
                if (DebugMode && (PerWork.data & KEY_B) != 0 && ((PerWork.data & KEY_SELECT) != 0))
                {
                    mnDebugFlagEditorRunStub();
                }
                else if (syGameFlagGet(FLAG_MENUS_RAISE_CUE))
                {
                    state->raisedCue = FIELD_RAISED_CUE_MENUS;
                }
                else
                {
                    state->effectTimerRunning = 0;
                    mnFieldMenuRunStub();
                    state->effectTimerRunning = 1;
                    fdUpdateHoverReady();
                }
                fdWorldMapReloadPendingAreaGraphics();
                syGameFlagClear(FLAG_MENU_OPEN);
                gmFlagRefreshLureCapStub();
                state->fieldMenu = 0;
            }
            else if (state->talk != 0)
            {
                fdWorldMapFreeze();
                fdTalkToActor(state->talk & (FIELD_TALK_REQUEST - 1));
                fdWorldMapReloadPendingAreaGraphics();
                state->talk = 0;
            }
            else if (state->examineMetatile != 0)
            {
                fdWorldMapFreeze();
                fdExamineMetatile(state->examineMetatile);
                fdWorldMapReloadPendingAreaGraphics();
                state->examineMetatile = 0;
            }
            else if (state->psynergy != 0)
            {
                packed = state->psynergy & ACT_PACKED;
                mnAreaNameWindowHideStub();
                if (packed == 0)
                {
                    fdWorldMapFreeze();
                    if (TICK_WAIT_BEFORE_PSYNERGY)
                        sySynch(1);
                }
                fdUsePsynergy(state->psynergy);
                if (packed == 0)
                    fdWorldMapReloadPendingAreaGraphics();
                state->psynergy = 0;
            }
            else if (state->item != 0)
            {
                fdWorldMapFreeze();
                fdUseItem(state->item, state->itemSlot);
                fdWorldMapReloadPendingAreaGraphics();
                state->item = 0;
            }
            else if (state->pauseMenu != 0)
            {
                // The Pause menu (in debug mode, B opens the scene select and L the palette
                // editor).
                sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                fdLeaderStopMoving();
                fdWorldMapFreeze();
                syGameFlagSet(FLAG_MENU_OPEN);
                if (*(volatile Uint8 *)&DebugMode && (PerWork.data & KEY_B))
                {
                    mnDebugSceneSelectRunStub();
                }
                else if (*(volatile Uint8 *)&DebugMode && (PerWork.data & KEY_L))
                {
                    fdPaletteEditorRun();
                }
                else if (syGameFlagGet(FLAG_MENUS_RAISE_CUE))
                {
                    state->raisedCue = FIELD_RAISED_CUE_MENUS;
                }
                else
                {
                    fdScriptEndAllLastingPsynergy();
                    mnAreaNameWindowHideStub();
                    party->returnMusic = sySoundGetCurrentBGMStub();
                    if (!syGameFlagGet(FLAG_IN_BATTLE_ARENA))
                    {
                        // With fewer than PAUSE_MENU_VRAM_BLOCKS of the sprite VRAM blocks free,
                        // sprite drawing stays suspended while the menu runs, and a tick passes so
                        // that the field's sprites give their VRAM back before it opens.
                        ACTOR_SYSTEM_STATE *system     = RamHeapSlots->actorSystem;
                        Uint8              *owner      = VramCacheBlockOwners;
                        Sint32              freeBlocks = 0;
                        Sint32              n;

                        for (n = 0; n < VRAM_BLOCK_COUNT; n++)
                        {
                            if (*owner++ == VRAM_BLOCK_OWNER_FREE)
                                freeBlocks++;
                        }
                        if (freeBlocks - PAUSE_MENU_VRAM_BLOCKS < 0)
                        {
                            system->suspended = 1;
                            sySynch(1);
                        }
                        mnPauseMenuRunStub(0);
                        system->suspended = 0;
                    }
                    else
                    {
                        // "The Pause Menu is not available now."
                        mnFieldMessageShowStub((Sint32)MsgFieldPauseUnavailable,
                                               FIELD_MESSAGE_ZOOM);
                    }
                }
                fdWorldMapReloadPendingAreaGraphics();
                syGameFlagClear(FLAG_MENU_OPEN);
                state->pauseMenu = 0;
            }
        }
        // Hand the leader back to the player, with the control order for how the party moves, and
        // run the walking update until the next request.
        syGameFlagClear(FLAG_FIELD_HANDLING_REQUESTS);
        if ((leader = fdGetPartyActor()) != NULL)
            fdActorStartMovementModeScriptStub(leader);
        pending = &MainRoutineStateWork.sandActive;
        do
        {
            sySynch(1);
            leader = fdGetPartyActor();
            if (DebugMode == 0 || !syGameFlagGet(FLAG_DEBUG_STEPS_OFF))
            {
                if (*pending == 0 || (leader->timer == 0 && *(Sint16 *)&leader->userValue == 0))
                    fdUpdateWalking(
                        leader->layer, leader->x.fixed, leader->y.fixed, leader->z.fixed);
            }
        } while (fdCountRequests() == 0);
    }

done:
    if (state->transitionActive != 0)
    {
        fdScreenTransitionFinish(state->transition, state->transitionFrame);
        state->transitionActive = 0;
        sySynch(state->transitionFrame);
    }
    sySynch(1);
    syHeapSlotRelease(HEAP_SLOT_FIELD);
    return result;
}

// Returns the metatile behavior of the map metatile 16 pixels ahead of the party leader when there
// is something to search there: a fixed search spot (METATILE_BEHAVIOR_JAR to
// METATILE_BEHAVIOR_WELL) whose ground is level with the leader or up to 16 pixels above it, or a
// behavior the scene has an examine cue for; aboard the ship on the World Map,
// SHIP_SEA_EXAMINE_CODE before sea of collision 3. Returns 0 otherwise.
Sint32 fdActorGetCueMetatileAheadOfCurrent(void)
{
    FIELD_STATE     *state;
    MAP_STATE       *map;
    ACTOR           *leader;
    POLAR_OFFSET_POS probePosition;
    MAP_METATILE    *metatile;
    Sint32           behavior;
    Sint32           groundHeight;
    Sint32           result;

    result = 0;
    leader = fdGetPartyActor();
    state  = FieldStateWork;
    map    = RamHeapSlots->mapGrid;
    if (leader != 0)
    {
        probePosition.vector.x = leader->x.fixed;
        probePosition.vector.y = leader->y.fixed;
        probePosition.vector.z = leader->z.fixed;
        {
            Sint32 *position = probePosition.coordinates;

            syVectorAddPolarOffset(toFIXED(16), leader->facing, position);
        }
        // World Map metatiles are 32 pixels square, 32 to a row; map metatiles 16 pixels square,
        // MAPGRID_SIZE to a row.
        if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
        {
            Sint32 x, z;

            x = probePosition.vector.x;
            z = probePosition.vector.z;
            metatile =
                &((MAP_METATILE *)syRamMapBlocks)
                    [((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) & (WORLD_MAP_VIEW_METATILES - 1))
                     + (((z / toFIXED(WORLD_MAP_METATILE_PIXELS)) & (WORLD_MAP_VIEW_METATILES - 1))
                        * WORLD_MAP_VIEW_METATILES)];
        }
        else
        {
            Sint32 x, z;

            metatile  = map->layers[0].metatiles;
            x         = probePosition.vector.x;
            z         = probePosition.vector.z;
            metatile += (x / toFIXED(MAPGRID_METATILE_PIXELS))
                      + (z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE;
        }
        behavior = metatile->collisionCode;
        if (behavior >= METATILE_BEHAVIOR_JAR && behavior <= METATILE_BEHAVIOR_WELL)
        {
            groundHeight = fdMapGridGetTerrainHeightStub(
                leader->layer, probePosition.vector.x, probePosition.vector.z);
            if (groundHeight >= leader->y.fixed && groundHeight <= leader->y.fixed + toFIXED(16))
            {
                result = behavior;
            }
        }
        else if (fdFindCue(CUE_EXAMINE, behavior) != 0)
        {
            result = behavior;
        }
        if (result == 0 && state->mode == ACTOR_SYSTEM_WORLD_MAP)
        {
            if (MainRoutineStateWork.ownerOverride != 0
                && fdWorldMapGetCollisionStub(probePosition.coordinates) == 3)
                result = SHIP_SEA_EXAMINE_CODE;
        }
    }
    return result;
}

// Places the party at the entrance it came in by, from the scene's entrance rows, unless it keeps
// its position, and sets the map's scroll bounds, at least a screen (240 x 160 pixels) wide and
// tall. With findEntrance it looks the entrance up and returns 0 when there is none; without, it
// takes the scene's first row.
Bool fdPlaceAtEntranceRow(Sint32 findEntrance)
{
    MAP_STATE            *map           = RamHeapSlots->mapGrid;
    Sint32                entrance      = MainRoutineStateWork.entrance;
    const SCENE_ENTRANCE *entranceEntry = SCENE_ENTRIES->entrances();
    Bool                  foundEntrance = FALSE;
    Sint32                left;

    if (findEntrance)
    {
        for (; entranceEntry->entrance != -1; entranceEntry++)
        {
            if (entranceEntry->entrance == entrance
                && (entranceEntry->requiredFlag == -1
                    || syGameFlagGet(entranceEntry->requiredFlag)))
            {
                foundEntrance = TRUE;
                break;
            }
        }
        if (!foundEntrance)
            return FALSE;
    }

    // Camera bounds apply even when returning to a saved party position.
    if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
    {
        SET_POSITION(&MainRoutineStateWork,
                     toFIXED(entranceEntry->x),
                     toFIXED(entranceEntry->y),
                     toFIXED(entranceEntry->z));
        MainRoutineStateWork.heading = entranceEntry->facing;
        MainRoutineStateWork.layer   = 0;
    }

    left = entranceEntry->cameraLeft;
    if (left != -1)
        map->minX = toFIXED(left);
    if (entranceEntry->cameraTop != -1)
        map->minZ = toFIXED(entranceEntry->cameraTop);
    if (entranceEntry->cameraRight != -1)
        map->maxX = toFIXED(entranceEntry->cameraRight);
    if (entranceEntry->cameraBottom != -1)
        map->maxZ = toFIXED(entranceEntry->cameraBottom);

    if (map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
        map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
    if (map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
        map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
    return TRUE;
}

// A map entrance's flags byte: the party's facing in sixteenths of a turn, then whether it stands 8
// pixels up and whether the camera keeps the bounds it has.
#define MAP_ENTRANCE_FACING_MASK 15
#define MAP_ENTRANCE_RAISED      (1 << 4)
#define MAP_ENTRANCE_KEEP_BOUNDS (1 << 5)

// Reads the map's entrance stream (MapScene) for the current entrance, places the party there
// unless FLAG_KEEP_PARTY_POSITION keeps it, then sets the camera bounds from the first box that
// holds it.
Bool fdPlaceAtEntrance(void)
{
    MAP_STATE          *map   = RamHeapSlots->mapGrid;
    MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
    FIXED               xFixed;
    Uint8              *cursor = MapScene;
    Sint32             *angle;
    Uint16             *layer;
    Sint32              entrance = state->entrance;
    Sint32              height;
    Sint32              x;
    Sint32              z;
    FIXED               zFixed;
    Sint32              base;
    Sint32              flags;
    Sint32              left;
    Sint32              top;
    Sint32              bottom;
    Sint32              right;
    Sint32              leftEdge, topEdge, rightEdge, bottomEdge;

    cursor = MapScene + (Uint16)fdStreamReadU16(&cursor);
    angle  = &MainRoutineStateWork.heading;
    layer  = &MainRoutineStateWork.layer;
    for (;; cursor += MAP_ENTRANCE_ROW_BYTES)
    {
        if (*(Sint8 *)cursor == -1)
        {
            *angle = FACING_SOUTH;
            {
                Uint32 cleared = 0;

                *layer         = cleared;
            }
            return FALSE;
        }
        if (*(Sint8 *)cursor == entrance)
        {
            cursor++;
            x      = fdStreamReadS16(&cursor);
            z      = fdStreamReadS16(&cursor);
            xFixed = toFIXED(x);
            zFixed = toFIXED(z);
            height = FIXEDtoINT(fdMapGridGetTerrainHeightStub(0, xFixed, zFixed));
            flags  = *cursor;
            base   = (Sint32)MapScene;
            if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
            {
                MainRoutineStateWork.x = xFixed;
                MainRoutineStateWork.z = zFixed;
                *angle                 = (flags & MAP_ENTRANCE_FACING_MASK) * (TRIG_TURN / 16);
                *layer                 = 0;
                if (flags & MAP_ENTRANCE_RAISED)
                    MainRoutineStateWork.y = toFIXED(8);
            }
            if (flags & MAP_ENTRANCE_KEEP_BOUNDS)
                return TRUE;

            cursor = MapSceneBoundsOffset;
            cursor = (Uint8 *)((Uint16)fdStreamReadU16(&cursor) + base);
            while (*(Sint8 *)cursor != -1)
            {
                {
                    left     = *(Sint8 *)cursor;
                    leftEdge = left * MAPGRID_METATILE_PIXELS;
                    cursor++;
                    top     = *(Sint8 *)cursor;
                    topEdge = top * MAPGRID_METATILE_PIXELS;
                    cursor++;
                    right     = *(Sint8 *)cursor;
                    rightEdge = right * MAPGRID_METATILE_PIXELS;
                    cursor++;
                    bottom     = *(Sint8 *)cursor;
                    bottomEdge = bottom * MAPGRID_METATILE_PIXELS;
                    cursor++;
                    if (x >= leftEdge && x <= rightEdge && z - height >= topEdge
                        && z - height <= bottomEdge)
                    {
                        map->minX = FIELD_METATILE_ORIGIN(left);
                        map->minZ = FIELD_METATILE_ORIGIN(top);
                        map->maxX = FIELD_METATILE_ORIGIN(right);
                        map->maxZ = FIELD_METATILE_ORIGIN(bottom);
                        if (map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
                            map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
                        if (map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
                            map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
                        break;
                    }
                }
            }
            return TRUE;
        }
    }
}

// Reads a little-endian halfword at *cursor and moves the cursor past it.
Uint32 fdStreamReadU16(Uint8 **cursor)
{
    Uint32 low  = *(*cursor)++;
    Uint32 high = *(*cursor)++;

    return low | (high << 8);
}

// Reads a signed little-endian halfword at *cursor and moves the cursor past it.
Sint16 fdStreamReadS16(Uint8 **cursor)
{
    Uint32 value;

    value  = *(*cursor)++;
    value |= *(*cursor)++ << 8;
    return value;
}

// Places the party where it enters: by the scene's entrance row for MainRoutineStateWork.entrance,
// else on a map by the map's own entrance stream (fdPlaceAtEntrance), else by the scene's first
// entrance row.
void fdApplyEntrance(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    Sint32       done;

    done = fdPlaceAtEntranceRow(TRUE);
    if (state->mode == ACTOR_SYSTEM_MAP && done == 0)
        done = fdPlaceAtEntrance();
    if (done == 0)
        fdPlaceAtEntranceRow(FALSE);
}

#undef NIBBLE_LOW
#undef NIBBLE_HIGH
#undef HAZARD_LIMIT_DEFAULT
#undef PLACE_MARKER_ON_FOOT
#undef PLACE_MARKER_ABOARD
#undef PAUSE_MENU_VRAM_BLOCKS
#undef MAP_ENTRANCE_FACING_MASK
#undef MAP_ENTRANCE_RAISED
#undef MAP_ENTRANCE_KEEP_BOUNDS
#undef MAP_ENTRANCE_ROW_BYTES
