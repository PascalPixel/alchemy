// Finding and running a scene's cues: talking, step cues, examine cues. Part of field_mode.c.

#include "field_mode_internal.h"

// What an examine cue is on (EXAMINE), the control bits 8..4 EXAMINE_CUE() packs it into.
#define EXAMINE_TARGET(kind) (((kind) >> 4) & 0x1f)
// The item a FIND_ITEM search result holds.
#define FIND_ITEM_MASK       0xfff
// The PP a Psynergy Stone gives each party member, enough to fill it: "The party's PP is fully
// restored!"
#define PSYNERGY_STONE_PP    999

// Returns the actor placement of actor id in the scene's actor placement tables, or NULL: for a
// party character (0 to 7) the row with its sprite, and for a placed actor (8 on) the row it was
// created from, counting the placed rows in order.
ACTOR_PLACE *fdFindActorPlacement(Sint32 id)
{
    ACTOR_PLACE *placement;
    Sint32       tableIndex;
    Sint32       placedActorId = ACTOR_FIRST_PLACED;

    for (tableIndex = 0; tableIndex < (Sint32)ARRAY_COUNT(FieldStateWork->tables); tableIndex++)
    {
        placement = FieldStateWork->tables[tableIndex];
        if (placement == NULL)
            continue;
        if (id < ACTOR_FIRST_PLACED)
        {
            for (; (Sint16)placement->sprite != -1; placement++)
            {
                if ((Sint16)placement->sprite == id)
                {
#if FIX_NULL_DEREFERENCE
                    return placement;
#else
                    goto found;
#endif
                }
            }
        }
        else
        {
            for (; (Sint16)placement->sprite != -1; placement++)
            {
                if ((Sint16)placement->sprite >= ACTOR_FIRST_PLACED)
                {
                    if (placedActorId == id)
                    {
#if FIX_NULL_DEREFERENCE
                        return placement;
#else
                        goto found;
#endif
                    }
                    placedActorId++;
                }
            }
        }
    }
#if FIX_NULL_DEREFERENCE
    return NULL;
#else
found:
    // UB: with the last table absent and no row matched, placement is NULL here and is read all the
    // same.
    if ((Sint16)placement->sprite == -1)
        return NULL;
    return placement;
#endif
}

// Returns whether a script condition holds: -1 always holds; a condition with CONDITION_FLAG_SET
// holds while that flag is set, any other while its flag is clear.
Sint32 fdGameFlagIsConditionActive(Sint32 condition)
{
    Sint32 flagSet;

    if (condition == -1)
    {
        return 1;
    }
    if (CONDITION_FLAG_SET & condition)
    {
        return gmFlagGetStub(condition);
    }
    flagSet = gmFlagGetStub(condition);
    return flagSet == 0;
}

// Returns 1 for an examine cue (CUE_EXAMINE) that holds whatever its condition: one with more flags
// than its kind, unless its result is a mimic battle (FIND_MIMIC).
Uint32 fdExamineCueIgnoresCondition(Sint32 flags, Sint32 result)
{
    if ((flags & CUE_KIND_MASK) != CUE_EXAMINE)
        return 0;
    if ((flags & EXAMINE_CUE_MASK) == CUE_EXAMINE)
        return 0;
    if ((result & FIND_TYPE_MASK) != FIND_MIMIC)
        return 1;
    return 0;
}

// Returns the first of the scene's cues of kind for target whose condition holds, or NULL.
const CUE *fdFindCue(Sint32 kind, Sint32 target)
{
    FIELD_STATE *state = FieldStateWork;
    const CUE   *cue   = state->cues;
    Sint32       flags;
    Uint32       facing = ((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;

    flags               = cue->kind;
    while (flags != -1)
    {
        // DEAD: flags already holds trigger->kind; the ROM keeps the second load of it once the
        // condition holds.
        if ((flags & CUE_KIND_MASK) == kind && (Uint8)cue->subject == target
            && (fdExamineCueIgnoresCondition(flags, cue->value) != 0
                || (fdGameFlagIsConditionActive(cue->condition) != 0 && (flags = cue->kind, 1))))
        {
            Sint32 matches          = 0;
            Sint32 minimumPushTicks = FIELD_PUSH_TICKS;
            Sint32 touchRequirements;

            if ((flags & STEP_CUE_FLAG_QUICK) != 0)
                minimumPushTicks = 2;
            touchRequirements = flags & (STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING);

            switch (touchRequirements)
            {
                case 0: matches = 1; break;
                case STEP_CUE_FLAG_WALKING:
                    if (state->blockedSteps > minimumPushTicks)
                    {
                        matches = 1;
                    }
                    break;
                case CUE_FLAG_FACING:
                    // Bits 15 to 12 are a facing in sixteenths of a turn; the leader's facing must
                    // be within an eighth of a turn of it.
                    flags   &= CUE_FACING_MASK;
                    matches  = (Uint16)((flags - facing) + (DEGtoANG(45) - 1)) <= DEGtoANG(90) - 2;
                    break;
                case STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING:
                    if (state->blockedSteps > minimumPushTicks)
                    {
                        flags &= CUE_FACING_MASK;
                        matches =
                            (Uint16)((flags - facing) + (DEGtoANG(45) - 1)) <= DEGtoANG(90) - 2;
                    }
                    break;
            }
            if (matches)
            {
                return cue;
            }
        }
        cue++;
        if (cue->kind == -1)
            break;
        flags = cue->kind;
    }
    return 0;
}

// Unused: runs the loaded overlay's initialize entry.
UNUSED void fdOverlayCallInit(void)
{
    OverlayArea.initialize();
}

// Returns the talk cue of actor id, or 0: for the actor stopped with Halt
// (MainRoutineStateWork.haltedActor), its CUE_TALK_HALTED script if it has one, the script for
// talking to it while it is stopped (Crossbone Isle's Bane has one).
const CUE *fdFindActorTalkCue(Sint32 id)
{
    const CUE *talkCue       = fdFindCue(CUE_TALK, id);
    Sint32     haltedActorId = MainRoutineStateWork.haltedActor;

    if (haltedActorId == id)
    {
        const CUE *haltedCue = fdFindCue(CUE_TALK_HALTED, haltedActorId);

        if (haltedCue != 0)
        {
            return haltedCue;
        }
    }
    return talkCue;
}

// Talks to actor id: shows its script's message or calls its script function, with the actor
// stopped and turned as its placement says. The actor Halt has stopped answers its CUE_TALK_HALTED
// script if it has one; else it says its talk message in halting speech, or, in place of a talk
// function, one of its sprite's two Halted lines.
Sint32 fdTalkToActor(Sint32 id)
{
    ACTOR_PLACE *placement = fdFindActorPlacement(id);
    Sint32       result    = -1;
    ACTOR       *actor     = fdActorGetById(id);
    const CUE   *cue;
    Bool         actorHalted = FALSE;
    Uint32       savedAngle;
    Sint32       decodeFlags = 0;

    if (MainRoutineStateWork.haltedActor == id)
    {
        actorHalted = TRUE;
        cue         = fdFindCue(CUE_TALK_HALTED, id);
        if (cue == 0)
        {
            cue         = fdFindCue(CUE_TALK, id);
            decodeFlags = MESSAGE_DECODE_HALTING_SPEECH;
            if (cue == 0)
            {
                return -1;
            }
            // A function talk cue is not run for a stopped actor: it says one of the two lines of
            // its sprite's Halted pair, at random.
            if (cue->value >= CUE_RESULT_MESSAGE_END)
            {
                Sint32 haltedLines = fdSpeakerGetHaltedLines(id);
                Uint32 random      = syRandom();
                Sint32 msg         = MsgPonderEllipsis + haltedLines * 2 + (random * 2 >> 16);
                fdScriptStart();
                fdScriptSetMessageId(msg);
                fdScriptShowMessageAndWait(id, 0);
                fdScriptEnd();
                goto finish;
            }
        }
    }
    else
    {
        cue = fdFindCue(CUE_TALK, id);
    }

    if (cue != 0 && cue->value != 0)
    {
        if (!actorHalted)
        {
            // Stop the actor; with any talk facing but TALK_FACING_KEEP, also stop the leader and
            // make each watch the other.
            actor->paused = 1;
            fdActorSetAnimationSpeedStub(actor, 0);
            savedAngle = actor->facing;
            if ((Uint8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK
                || (Sint8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH)
            {
                ACTOR *leader   = fdActorGetById(MainRoutineStateWork.leaderActorId);

                leader->targetX = leader->x.fixed;
                leader->targetY = leader->y.fixed;
                leader->targetZ = leader->z.fixed;
                SET_VELOCITY(leader, 0, 0, 0);
                fdActorTurnToFaceEachOther(id, MainRoutineStateWork.leaderActorId, 0);
            }
        }
        // A result below CUE_RESULT_MESSAGE_END is a message; above, a function to call.
        if (cue->value < CUE_RESULT_MESSAGE_END)
        {
            mnMessageSetDecodeFlagsStub(decodeFlags);
            fdScriptStart();
            fdScriptSetMessageId(cue->value);
            fdScriptShowMessageAndWait(id, 0);
            fdScriptEnd();
        }
        else
        {
            fdLeaderStopMoving();
            ((void (*)(Sint32))cue->value)(id);
        }
        if (!actorHalted)
        {
            // An actor whose script has ended goes back to what its talk facing says: watching the
            // leader or its old angle.
            if (actor->script[actor->scriptCursor] == SCRIPT_COMMAND_END)
            {
                if ((Sint8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH)
                {
                    actor->watchedActor =
                        (union ACTOR_OR_EFFECT *)fdActorGetById(MainRoutineStateWork.leaderActorId);
                    actor->steeringFlags |= ACTOR_FACES_MOTION;
                    fdActorSetMovementType(actor, (Sint32)ActorFaceWatchedScript);
                }
                else if ((Sint8)placement->talkFacing == TALK_FACING_PARTY_AND_BACK)
                {
                    actor->timer = savedAngle;
                    fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
                }
            }
            actor->paused = 0;
            fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
        }
        result = 0;
    }
finish:
    if (actorHalted)
    {
        fdPsynergyResumeHaltedActor(MainRoutineStateWork.haltedActor);
        MainRoutineStateWork.haltedActor = -1;
    }
    return result;
}

// Runs the scene's raised cue (CUE_RAISED) for value: its message or its function. Returns 0 when a
// script ran, -1 when there was none.
Sint32 fdRunRaisedCue(Sint32 value)
{
    Sint32     result;
    Sint32     handled;
    const CUE *cue;

    cue     = fdFindCue(CUE_RAISED, value);
    handled = -1;
    if (cue != NULL)
    {
        result = cue->value;
        if (result != 0)
        {
            if (result < CUE_RESULT_MESSAGE_END)
            {
                fdScriptStart();
                fdScriptSetMessageId(cue->value);
                fdScriptShowMessageAndWait(-1, 0);
                handled = 0;
                fdScriptEnd();
            }
            else
            {
                ((void (*)(Sint32))result)(value);
                handled = 0;
            }
        }
    }
    return handled;
}

// Runs the step cue (CUE_STEP) of what the leader touched, or else takes its exit (CUE_EXIT): plays
// the exit's sound (and effect) and records where it leads.
Sint32 fdRunStepCueOrExit(Sint32 value)
{
    const CUE   *touch;
    const CUE   *exit;
    Sint32       exitId;
    Sint32       handled;
    FIELD_STATE *state;

    touch   = fdFindCue(CUE_STEP, value);
    handled = -1;
    state   = FieldStateWork;
    if ((touch != 0) && (touch->value != 0))
    {
        if (touch->kind & STEP_CUE_FLAG_WALKING)
        {
            state->blockedSteps = 0;
        }
        if (touch->value < CUE_RESULT_MESSAGE_END)
        {
            fdScriptStart();
            fdScriptSetMessageId(touch->value);
            fdScriptShowMessageAndWait(-1, 0);
            handled = 0;
            fdScriptEnd();
        }
        else
        {
            ((void (*)(Sint32))touch->value)(value);
            handled = 0;
        }
    }
    else
    {
        exit = fdFindCue(CUE_EXIT, value);
        if (exit != 0)
        {
            // An EXIT_STYLE chooses how the leader leaves; the animated styles' flags make the next
            // scene play the matching arrival.
            exitId = exit->kind & EXIT_STYLE_MASK;
            switch (exitId)
            {
                case EXIT_STYLE_NORMAL: sySoundPlayStub(SE_MAP_EXIT); break;
                case EXIT_STYLE_STAIRS_UP:
                    sySoundPlayStub(SE_STAIRS_UP);
                    fdExitStartStairsUp();
                    break;
                case EXIT_STYLE_STAIRS_DOWN:
                    sySoundPlayStub(SE_STAIRS_DOWN);
                    fdExitStartStairsDown();
                    break;
            }
            state->exit = exit->value;
            handled     = 0;
        }
    }
    return handled;
}

// Runs the script of a metatile the leader pushes against (METATILE_BEHAVIOR_LEDGE to
// METATILE_BEHAVIOR_JUMP_FORWARD, and METATILE_BEHAVIOR_JUMP_FORWARD_UNREACHED), once it has pushed
// for more than FIELD_PUSH_TICKS ticks; the count then restarts.
Sint32 fdRunPushMetatile(Sint32 behavior)
{
    FIELD_STATE *state  = FieldStateWork;
    Sint32       leader = MainRoutineStateWork.leaderActorId;

    switch (behavior)
    {
        case METATILE_BEHAVIOR_LEDGE:
            if (state->blockedSteps > FIELD_PUSH_TICKS && (PerData & KEY_DOWN))
            {
                fdActorJumpDownLedge(leader, ANIM_LEADER_SPRING, 0);
                state->blockedSteps = 0;
            }
            break;
        case METATILE_BEHAVIOR_JUMP_FORWARD_UNREACHED:
        case METATILE_BEHAVIOR_JUMP_FORWARD:
            if (state->blockedSteps > FIELD_PUSH_TICKS)
            {
                fdTryJumpForward();
                state->blockedSteps = 0;
            }
            break;
        case METATILE_BEHAVIOR_CLIMB:
            if (state->blockedSteps > FIELD_PUSH_TICKS)
            {
                if (PerData & KEY_DOWN)
                {
                    fdClimbDown();
                }
                else if (PerData & KEY_UP)
                {
                    fdClimbUp();
                }
                state->blockedSteps = 0;
            }
            break;
    }
    return 0;
}

// Searches the metatile ahead with the given metatile behavior. The fixed search spots,
// METATILE_BEHAVIOR_JAR to METATILE_BEHAVIOR_WELL, show their two messages.
Sint32 fdExamineMetatile(Sint32 tile)
{
    const CUE *cue;
    Sint32     leader;
    Uint32     msgOffset;
    Sint16     condition;
    Uint32     searchResult;
    Sint32     rewardType;
    Bool       canOpen;

    leader    = MainRoutineStateWork.leaderActorId;
    // Each fixed spot has its own pair of lines, chosen through FieldExamineCueMessageOffsets; the
    // unsigned compare also rejects codes below METATILE_BEHAVIOR_JAR.
    msgOffset = tile - METATILE_BEHAVIOR_JAR;
    if (msgOffset <= METATILE_BEHAVIOR_WELL - METATILE_BEHAVIOR_JAR)
    {
        fdLeaderStopMoving();
        tile = FieldExamineCueMessageOffsets[msgOffset];
        mnFieldMessageShowStub(MsgTileChecked + tile, FIELD_MESSAGE_ZOOM);
        mnFieldMessageShowStub(MsgTileResult + tile, FIELD_MESSAGE_ZOOM);
    }
    else
    {
        cue = fdFindCue(CUE_EXAMINE, tile);
        if (cue != NULL)
        {
            Sint32 flags = cue->kind;

            // Bits 8 to 4, the search target (EXAMINE), choose what the leader checks ("checked the
            // chest...") unless CUE_SUBJECT_OWN_EFFECT says the script has its own message; if the
            // script leaves FLAG_NOTHING_HAPPENS set, the matching result line follows ("but the
            // chest was empty.").
            condition    = cue->condition;
            msgOffset    = EXAMINE_TARGET(flags);
            if ((cue->subject & CUE_SUBJECT_OWN_EFFECT) == 0 && msgOffset != 0)
            {
                fdLeaderStopMoving();
                mnFieldMessageShowStub(MsgTileChecked + msgOffset, FIELD_MESSAGE_ZOOM);
                gmFlagSetStub(FLAG_NOTHING_HAPPENS);
            }
            else
            {
                gmFlagClearStub(FLAG_NOTHING_HAPPENS);
            }
            // The value is a function address (bits 27 to 24 set) or a FIND_ value whose top 12
            // bits give its type: an item, coins, a Psynergy Stone, a message or a mimic.
            searchResult = cue->value;
            if ((searchResult & FIND_FUNCTION_BITS) != 0
                || (searchResult & FIND_TYPE_MASK) == FIND_MESSAGE(0))
            {
                if ((searchResult & FIND_FUNCTION_BITS) != 0)
                {
                    // A function.
                    if (fdGameFlagIsConditionActive(condition) != 0)
                        ((void (*)(Sint32))cue->value)(MainRoutineStateWork.leaderActorId);
                    if (gmFlagGetStub(FLAG_NOTHING_HAPPENS) != 0)
                        mnFieldMessageShowStub(MsgTileResult + msgOffset, FIELD_MESSAGE_ZOOM);
                }
                else
                {
                    if (fdGameFlagIsConditionActive(condition) != 0)
                        mnFieldMessageShowStub((Uint16)cue->value, FIELD_MESSAGE_ZOOM);
                    else
                        // "but found nothing."
                        mnFieldMessageShowStub(MsgButFoundNothing, FIELD_MESSAGE_ZOOM);
                }
            }
            else
            {
                // A chest; its condition flag is set once it has been opened.
                fdScriptStart();
                if (fdGameFlagIsConditionActive(condition) != 0)
                {
                    // FIND_CHEST_LOCKED marks a chest the party's own characters cannot open
                    // ("Can't open it").
                    searchResult = cue->value;
                    canOpen      = TRUE;
                    if ((searchResult & FIND_CHEST_LOCK_MASK) == FIND_CHEST_LOCKED
                        && leader <= ALLY_MAX)
                        canOpen = FALSE;
                    if (canOpen != 0)
                    {
                        // A EXAMINE_CUE(EXAMINE_CHEST) script is a chest the map shows as a
                        // treasure spot.
                        if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                            fdTreasureSpotOpenChest(tile);
                        rewardType = cue->value & FIND_TYPE_MASK;
                        if (rewardType == FIND_PSYNERGY_STONE)
                        {
                            // A Psynergy Stone: restores all PP.
                            void *object;

                            if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                                fdTreasureSpotShowOpened(tile);
                            object = fdTreasureSpotGetActor(tile);
                            fdItemDisplayRaiseAboveLeader(object);
                            sySoundPlayStub(MUS_TREASURE_FOUND);
                            mnMessageArgumentPushStub((Uint16)cue->value, MESSAGE_ARGUMENT_NUMBER);
                            // "Isaac got a Psynergy Stone."
                            mnFieldMessageShowStub(MsgGotPsynergyStone,
                                                   FIELD_MESSAGE_ZOOM
                                                       | FIELD_MESSAGE_WAITING_FOR_SOUND);
                            fdPartyAddPp(PSYNERGY_STONE_PP, 0);
                            mnPartyStatusCreateWindowStub(PARTY_STATUS_SHOW_PP);
                            sySoundPlayStub(SE_RECOVERY);
                            // "The party's PP is fully restored!"
                            mnFieldMessageShowStub(MsgGotPsynergyStone + 1, FIELD_MESSAGE_ZOOM);
                            mnPartyStatusCloseStub();
                            fdActorSelectAnimationStub(object, 2);
                            sySoundPlayStub(SE_GLITTER);
                            fdScriptWait(30);
                            // "The Psynergy Stone disappeared..."
                            mnFieldMessageShowStub(MsgGotPsynergyStone + 2, FIELD_MESSAGE_ZOOM);
                            fdTreasureSpotHide(tile);
                            if (condition != -1)
                                gmFlagSetStub(condition);
                        }
                        else if (rewardType == FIND_MIMIC)
                        {
                            // A mimic: a battle with its slot of ENCOUNTER_ZONE_MIMIC.
                            FIELD_STATE *state = FieldStateWork;

                            if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                                fdTreasureSpotLaunchMimic(tile);
                            // The chest's flag (with CONDITION_FLAG_SET) waits in pendingDjinni,
                            // which fdModeRun resolves when the scene is entered again after the
                            // battle.
                            if (condition != -1)
                            {
                                MainRoutineStateWork.pendingDjinni = condition | CONDITION_FLAG_SET;
                            }
                            // BATTLE_TYPE_NO_ESCAPE (battle/battle_turn.c): no first strike or
                            // surprise, and every attempt to run fails; the command menu still
                            // offers it, refusing outright only for BATTLE_TYPE_SCRIPTED and
                            // BATTLE_TYPE_LINK.
                            state->encounter = fdEncounterGetFormationAtSlot(ENCOUNTER_ZONE_MIMIC,
                                                                             (Uint16)cue->value);
                            SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
                            fdEncounterSelectBattleMusic(ENCOUNTER_ZONE_MIMIC, (Uint16)cue->value);
                            sySoundPlayStub(MainRoutineStateWork.battleMusic);
                            // "The treasure chest was really a Mimic!"
                            mnFieldMessageShowStub(MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
                        }
                        else if (rewardType == FIND_COINS(0))
                        {
                            // Coins.
                            void *object;

                            object =
                                fdItemDisplayShowItemInFront(MainRoutineStateWork.leaderActorId, 0);
                            sySynch(30);
                            if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                                fdTreasureSpotShowOpened(tile);
                            fdItemDisplayRaiseAboveLeader(object);
                            sySoundPlayStub(MUS_TREASURE_FOUND);
                            mnMessageArgumentPushStub((Uint16)cue->value, MESSAGE_ARGUMENT_NUMBER);
                            // "Isaac got [number] coins."
                            mnFieldMessageShowStub(
                                MsgGotCoins, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
                            gmPartyAdjustCoinsStub((Uint16)cue->value);
                            if (condition != -1)
                                gmFlagSetStub(condition);
                            fdActorReleaseStub(object);
                        }
                        else
                        {
                            // An item, which stays in the chest when nobody can carry it.
                            void  *object;
                            Sint32 recipient;

                            object = fdItemDisplayShowItemInFront(
                                MainRoutineStateWork.leaderActorId, cue->value & FIND_ITEM_MASK);
                            sySynch(30);
                            recipient = gmPartyInventoryAddStub((Uint16)cue->value);
                            // msgOffset now masks the inventory entry the chest gave.
                            msgOffset = INVENTORY_QUANTITY_MASK | INVENTORY_ENTRY_MASK;
                            if (recipient == -1)
                            {
                                mnMessageArgumentPushStub(cue->value & FIND_ITEM_MASK,
                                                          MESSAGE_ARGUMENT_ITEM);
                                // "Isaac found [item]."
                                mnFieldMessageShowStub(MsgFoundItem, FIELD_MESSAGE_ZOOM);
                                // "But Isaac's party can't carry any more, so they left it behind."
                                mnFieldMessageShowStub(MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
                                fdActorReleaseIfPresent(object);
                                if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                                    fdTreasureSpotCloseChest(tile);
                            }
                            else
                            {
                                if ((cue->kind & EXAMINE_CUE_MASK) == EXAMINE_CUE(EXAMINE_CHEST))
                                    fdTreasureSpotShowOpened(tile);
                                fdItemDisplayRaiseAboveLeader(object);
                                sySoundPlayStub(MUS_TREASURE_FOUND);
                                mnMessageArgumentPushStub(cue->value & msgOffset,
                                                          MESSAGE_ARGUMENT_ITEM);
                                if (recipient == MainRoutineStateWork.leaderActorId)
                                {
                                    // "Isaac got [item]."
                                    mnFieldMessageShowStub(MsgGotItem,
                                                           FIELD_MESSAGE_ZOOM
                                                               | FIELD_MESSAGE_WAITING_FOR_SOUND);
                                }
                                else
                                {
                                    mnMessageArgumentPushStub(recipient, MESSAGE_ARGUMENT_ACTOR);
                                    // "Isaac gave [item] to [member]."
                                    mnFieldMessageShowStub(MsgGaveItemToMember,
                                                           FIELD_MESSAGE_ZOOM
                                                               | FIELD_MESSAGE_WAITING_FOR_SOUND);
                                }
                                if (condition != -1)
                                    gmFlagSetStub(condition);
                                fdActorReleaseStub(object);
                            }
                        }
                    }
                    else
                    {
                        // "but Isaac can't open it..."
                        mnFieldMessageShowStub(MsgCantOpenIt, FIELD_MESSAGE_ZOOM);
                    }
                }
                else
                {
                    // Already opened: the result line for the check message.
                    mnFieldMessageShowStub(MsgTileResult + msgOffset, FIELD_MESSAGE_ZOOM);
                }
                fdScriptEnd();
                fdPlayQueuedMusic();
            }
        }
        else
        {
            // "Isaac checked on the ground..."
            mnFieldMessageShowStub(MsgCheckedGround, FIELD_MESSAGE_ZOOM);
            // "but didn't find anything."
            mnFieldMessageShowStub(MsgGroundDidntFindAnything, FIELD_MESSAGE_ZOOM);
        }
        gmFlagClearStub(FLAG_NOTHING_HAPPENS);
    }
    return 0;
}

#undef EXAMINE_TARGET
#undef FIND_ITEM_MASK
#undef PSYNERGY_STONE_PP
