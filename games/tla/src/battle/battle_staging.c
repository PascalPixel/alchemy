// Playing one action of a turn: the staging its ability calls for (an attack, a cast, a summon over
// a faded field, a flight), and the extra strikes that Daedalus, Megaera and Iris add. Part of
// battle.c.

#include "battle_effect/battle_effect_ids.h"

// A party member's attack effect, staged by id range (EFFECT_BY_ID_RANGE): ATTACK_EFFECT_SLASH for
// Isaac, Garet, Felix, Jenna and Piers, ATTACK_EFFECT_WAVE for Ivan, Mia and Sheba, as their
// BattleSprites rows give, any other none.
Sint32 btUnitGetPartyAttackEffect(Sint32 characterId)
{
    Sint32 effect = 0;

    switch (characterId)
    {
        case ISAAC: effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_SLASH; break;
        case GARET: effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_SLASH; break;
        case IVAN : effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_WAVE; break;
        case MIA  : effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_WAVE; break;
        case FELIX:
        case JENNA:
        case PIERS: effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_SLASH; break;
        case SHEBA: effect = EFFECT_BY_ID_RANGE | ATTACK_EFFECT_WAVE; break;
    }
    return effect;
}

// Called by btRun at the start of every turn; does nothing.
void btTurnStartNoOperation(void)
{
}

// The plan of the extra action Daedalus queues for the turn's end (COMMAND_DAEDALUS):
// ABILITY_DAEDALUS_STRIKE from a random unit of the queuing side against every unit of the other,
// staged as the summon SUMMON_EFFECT_DAEDALUS_STRIKE. source is the queued BATTLE_ACTION_RECORD,
// read as halfwords: [0] its unit, [3] its command. Declared Sint32, though it returns no value;
// its one caller reads none.
Sint32 btStagingPlanDaedalus(Uint16 *src, BATTLE_PLAN *plan)
{
    Uint16 ids[10];
    Sint32 count;
    Sint32 i;
    Sint32 side = BATTLE_SIDE_MASK_ENEMIES;
    Sint32 own;

    if (src[0] > ALLY_MAX)
        side = BATTLE_SIDE_MASK_PARTY;
    count = btUnitListLivingIds(side, ids);
    // The plan and BATTLE_STATE.skipPlayback after it.
    syWordsClearEntry(plan, sizeof(*plan) + sizeof(Sint32));
    own = 0;
    if (src[0] <= ALLY_MAX)
        own = 1;
    own           = btGetUnitAtPlace(btAIChooseRandomTarget(own));
    plan->element = ELEMENT_MARS;
    plan->staging = STAGING_DAEDALUS;
    {
        Sint32 command = src[3];

        plan->unitId   = own;
        plan->command  = command;
    }
    for (i = 0; i < count; i++)
    {
        plan->targetIds[i]        = ids[i];
        plan->targetOffset[i]     = 0;
        plan->targetMultiplier[i] = 1;
        plan->targetCritical[i]   = 0;
        plan->targetHit[i]        = 1;
    }
    plan->targetCount  = count;
    plan->abilityId    = ABILITY_DAEDALUS_STRIKE;
    plan->battleEffect = EFFECT_SUMMON | SUMMON_EFFECT_DAEDALUS_STRIKE;
}

// The plan of Megaera's second part (STAGING_MEGAERA): High Impact (ABILITY_HIGH_IMPACT) from the
// acting unit on every living unit of its own side.
void btStagingPlanHighImpact(BATTLE_PLAN *plan)
{
    Uint16 ids[10];
    Sint32 count;
    Sint32 i;
    Sint32 side = BATTLE_SIDE_MASK_PARTY;

    if (plan->unitId > ALLY_MAX)
        side = BATTLE_SIDE_MASK_ENEMIES;
    count = btUnitListLivingIds(side, ids);
    for (i = 0; i < count; i++)
    {
        plan->targetIds[i]        = ids[i];
        plan->targetOffset[i]     = 0;
        plan->targetMultiplier[i] = 1;
        plan->targetCritical[i]   = 0;
        plan->targetHit[i]        = 1;
    }
    plan->targetCount  = count;
    plan->abilityId    = ABILITY_HIGH_IMPACT;
    plan->battleEffect = 1 << EFFECT_VARIANT_SHIFT | BATTLE_EFFECT_CRUCIBLE_FIRE;
}

// The plan of Iris's second part (STAGING_IRIS): Aurora Field (ABILITY_AURORA_FIELD) over the
// acting unit's whole side, reserves included for the party.
void btStagingPlanAuroraField(BATTLE_PLAN *plan)
{
    Uint16 ids[10];
    Sint32 count;
    Sint32 i;

    if (plan->unitId <= ALLY_MAX)
    {
        count  = btPartyPrepareMembers(ids);
        count += btPartyPrepareReserveOwners(&ids[count]);
    }
    else
    {
        count = btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, ids);
    }
    for (i = 0; i < count; i++)
    {
        plan->targetIds[i]        = ids[i];
        plan->targetOffset[i]     = 0;
        plan->targetMultiplier[i] = 1;
        plan->targetCritical[i]   = 0;
        plan->targetHit[i]        = 1;
    }
    plan->targetCount  = count;
    plan->abilityId    = ABILITY_AURORA_FIELD;
    plan->battleEffect = EFFECT_ON_DOWNED | 2 << EFFECT_VARIANT_SHIFT | BATTLE_EFFECT_REVERSE_STAR;
    plan->hauntDamage  = 0;
}

// Sends every battler back to its spot.
void btPartyReturnAllToSpots(void)
{
    Uint16 ids[UNIT_MAX];
    Sint32 count = btUnitListIds(BATTLE_SIDE_MASK_BOTH, ids);
    Sint32 i;

    for (i = 0; i < count; i++)
        btMotionReturnToSpot(ids[i]);
}

// Plays one action of the turn after delay ticks: an enemy picks its ability now, the action is
// built and staged, and every unit returns to its spot. Returns 1 when the party got away, 0
// otherwise, and -1 when the unit is already down.
Sint32 btStagingDispatchAction(BATTLE_ACTION_RECORD *request, Sint32 delay)
{
    BATTLE_BACKGROUND_STATE *view;
    BATTLE_STATE            *battle;
    BATTLE_CAMERA           *camera;
    UNIT                    *unit;
    Uint16                   actingUnit[2];
    Bool                     fled;
    Sint32                   result;

    battle = RamHeapSlots->battleState;
    fled   = FALSE;
    if (request->unitId == BATTLE_UNIT_LIST_END)
        return 0;
    unit = syUnitGet(request->unitId);
    if (unit->hp == 0)
        return -1;
    battle->actingAnimation = POSE_CAST;
    // An enemy settles its ability now, as it acts.
    if (unit->classId == CLASS_NONE)
        btAIChooseAction(request, 1);

    // The camera eases back over 60 ticks, the background at full brightness, the far depth well
    // past the battlefield.
    view               = RamHeapSlots->battleBg;
    view->ticks        = 60;
    view->keepRange    = 0;
    camera             = RamHeapSlots->cameraState;
    battle->brightness = toFIXED(1);
    syInitMatrix();
    syLookAtLoad(camera->eye, camera->focus);
    {
        Sint32 ratio = syDivFX(BATTLE_CAMERA_DISTANCE, toFIXED(3) / 4);

        sySetScreenDist(BATTLE_CAMERA_DISTANCE, ratio, BATTLE_FAR_DEPTH);
    }
    if (delay != 0)
    {
        view->targetYaw = YAW_PARTY_ACTION;
        sySynch(delay);
    }
    // Build the action, with the acting unit highlighted, and stage it.
    if (request->command == COMMAND_DAEDALUS)
    {
        btStagingPlanDaedalus((Uint16 *)request, &battle->plan);
        result = 0;
    }
    else
    {
        actingUnit[0] = request->unitId;
        actingUnit[1] = BATTLE_UNIT_LIST_END;
        btHighlightSetModes(actingUnit, HIGHLIGHT_SELECTED);
        result = btActionRunBuild(request, &battle->plan);
    }
    if (result == 0)
    {
        switch (battle->plan.staging)
        {
            case 0                   : /* an ability with no staging plays nothing */ break;
            case STAGING_MESSAGES    : btStagingRunMessages(&battle->plan, 0); break;
            case STAGING_ATTACK      : btStagingRunAttack(&battle->plan, 0); break;
            case STAGING_BEAST_ATTACK: btStagingRunAttack(&battle->plan, 2); break;
            case STAGING_SUMMON      : btStagingRunFullScreen(&battle->plan, STAGING_HIDE_ACTOR); break;
            case STAGING_FULL_SCREEN : btStagingRunFullScreen(&battle->plan, 0); break;
            case STAGING_DAEDALUS:
                btStagingRunFullScreen(&battle->plan, STAGING_HIDE_ACTOR | STAGING_NO_CAST);
                break;
            case STAGING_CAST:
            case STAGING_BEAST_PSYNERGY:
                btStagingRunPsynergyCast(&battle->plan, CAST_PSYNERGY);
                break;
            case STAGING_DJINNI    : btStagingRunPsynergyCast(&battle->plan, CAST_DJINNI); break;
            case STAGING_ENEMY_CAST:
            case STAGING_BEAST_CAST: btStagingRunPsynergyCast(&battle->plan, CAST_ENEMY); break;
            case STAGING_ITEM      : btStagingRunItem(&battle->plan, 0, request); break;
            case STAGING_TEAM_STRIKE:
            {
                Uint8 swap                      = battle->plan.targetIds[0];

                battle->events.keepPendingSound = 1;
                battle->plan.targetIds[0]       = battle->plan.partnerId;
                battle->skipPlayback            = 1;
                btStagingRunPsynergyCast(&battle->plan, CAST_DJINNI);
                battle->skipPlayback      = 0;
                battle->plan.targetIds[0] = swap;
                swap                      = battle->plan.unitId;
                battle->plan.unitId       = battle->plan.partnerId;
                battle->plan.partnerId    = swap;
                battle->plan.battleEffect =
                    btUnitGetPartyAttackEffect(syUnitGet(battle->plan.unitId)->enemyId);
                // A partner striking its own side: "<partner>, stop hitting yourself!" at itself,
                // "<partner> attacks an ally!" at another.
                if (((battle->plan.unitId ^ battle->plan.targetIds[0]) >> 7) == 0)
                {
                    mnMessageArgumentPushStub(battle->plan.unitId, MESSAGE_ARGUMENT_ACTOR);
                    if (battle->plan.unitId == battle->plan.targetIds[0])
                        mnBattleMessageShowAndWaitStub((Sint32)&MsgStopHittingYourself);
                    else
                        mnBattleMessageShowAndWaitStub((Sint32)&MsgAttacksAlly);
                }
                btStagingRunAttack(&battle->plan,
                                   battle->plan.abilityId == ABILITY_MELD ? ATTACK_WITH_PARTNER
                                                                          : 0);
                swap                   = battle->plan.unitId;
                battle->plan.unitId    = battle->plan.partnerId;
                battle->plan.partnerId = swap;
                break;
            }
            case STAGING_DJINN_RECOVERY:
            {
                Sint32 kept;

                battle->skipPlayback = 1;
                btStagingRunPsynergyCast(&battle->plan, CAST_DJINNI);
                battle->skipPlayback = 0;
                // "Djinn are recovering more quickly!"
                mnBattleMessageShowAndWaitStub((Sint32)&MsgDjinnRecoverFaster);
                kept = btDjinnRecoveryCountDownSide(battle->plan.targetIds[0] > ALLY_MAX);
                // "But no Djinn are in Recovery!"
                if (kept == 0)
                    mnBattleMessageShowAndWaitStub((Sint32)(MsgDjinnRecoverFaster + 1));
                if (kept <= 1)
                    btMessageWaitForAdvance();
                break;
            }
            case STAGING_MEGAERA:
                btStagingRunFullScreen(&battle->plan, STAGING_HIDE_ACTOR);
                if (btUnitListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
                    break;
                if (btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0)
                    break;
                btStagingPlanHighImpact(&battle->plan);
                btStagingRunPsynergyCast(&battle->plan, CAST_DJINNI);
                break;
            case STAGING_IRIS:
                btStagingRunFullScreen(&battle->plan, STAGING_HIDE_ACTOR);
                btStagingPlanAuroraField(&battle->plan);
                btStagingRunPsynergyCast(&battle->plan, CAST_DJINNI);
                break;
            case STAGING_FLEE:
                if (btStagingRunFlee(&battle->plan) != 0)
                    fled = TRUE;
                if (fled != 0)
                    goto finish;
                break;
        }
    }
    else
    {
        if (result == ACTION_REPORTED)
        {
            btMessageWaitForAdvance();
            sySynch(3);
        }
        btHighlightSetModes(0, HIGHLIGHT_OFF);
    }

    // The action's end, then every unit back to its spot. A party that fled skips this and the
    // action's mark.
    btMotionDestroyAllSlotObjects();
    btUnitProcessActionEnd(&battle->plan);
    btObjectPut();
    btPlacementRelayoutEnemies();
    btPartyReturnAllToSpots();
    request->unitId = BATTLE_UNIT_LIST_END;
finish:
    btBackgroundSetMode(BATTLE_BACKGROUND_ZOOMED, (RamHeapSlots->battleState)->bg, 0);
    sySoundPlayStub(SOUND_FADE_OUT_ACTION_SE);
    return fled;
}

// Fills a battle effect command from an action run: its kind and strength, the user and the targets
// still standing, or all of them for an effect that revives.
void btStagingBuildTargetList(BATTLE_PLAN *plan, BATTLE_PRESENTATION_STATE *effect)
{
    Sint16 *outputTarget;
    Uint32  flags;
    Sint32  livingTargets;
    Sint32  targetIndex;
    Uint8  *inputTarget;

    livingTargets   = 0;
    effect->prelude = 0;
    flags           = plan->battleEffect;
    effect->kind    = flags & EFFECT_ID_MASK;
    effect->variant = (flags & EFFECT_VARIANT_MASK) >> EFFECT_VARIANT_SHIFT;
    effect->actor   = plan->unitId;
    targetIndex     = 0;
    if (targetIndex < plan->targetCount)
    {
        inputTarget  = plan->targetIds;
        outputTarget = effect->actors;
        do
        {
            if (btUnitIsListed(*inputTarget)
                && (syUnitGet(*inputTarget)->hp != 0 || (plan->battleEffect & EFFECT_ON_DOWNED)))
            {
                *outputTarget++ = *inputTarget;
                livingTargets++;
            }
            targetIndex++;
            inputTarget++;
        } while (targetIndex < plan->targetCount);
    }
    if (livingTargets == 0)
    {
        effect->actors[0] = plan->targetIds[0];
        livingTargets     = 1;
    }
    effect->firstTarget = plan->targetIds[0];
    effect->count       = livingTargets;
    effect->range       = 1;
}

// Plays a unit's attempt to run: the party runs off one by one unless the roll failed ("But there's
// no escape!"); an enemy gets away 7 times in 10. Returns 1 when the party got away.
Sint32 btStagingRunFlee(BATTLE_PLAN *plan)
{
    Bool                     completed;
    BATTLE_BACKGROUND_STATE *view    = RamHeapSlots->battleBg;
    Uint32                   unitId  = plan->unitId;
    BATTLE_STATE            *battle  = RamHeapSlots->battleState;
    Sint32                   escaped = 0;

    completed                        = FALSE;
    view->targetYaw                  = YAW_PARTY_ACTION;
    view->busy                       = 1;
    btHighlightSetModes(0, HIGHLIGHT_OFF);

    if (unitId <= ALLY_MAX)
    {
        // The roll was made as the commands were gathered: a failed one handed the enemies the turn
        // as if they had struck first.
        escaped = completed;
        if (battle->firstStrike != FIRST_STRIKE_ENEMIES)
            escaped = 1;

        if (!escaped)
        {
            mnBattleMessageShowAndWaitStub((Sint32)MsgNoEscape);
            btMessageWaitForAdvance();
        }
        else
        {
            Sint16 unitIds[UNIT_MAX];
            Sint32 index = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (Uint16 *)unitIds) - 1;

            while (index != -1)
            {
                UNIT *unit = syUnitGet(unitIds[index]);

                if (unit->stun == 0 && unit->sleep == 0)
                {
                    btMotionSetupEscapeObject(unitIds[index]);
                    sySynch(8);
                }
                index--;
            }
            completed = TRUE;
        }
    }
    else if ((syRandomBelow(10)) <= 6)
    {
        Sint16 runner[2];

        runner[0] = plan->unitId;
        runner[1] = BATTLE_UNIT_LIST_END;
        btMotionSetupEscapeObject(runner[0]);
        sySynch(8);
        btObjectRemoveFromLists(plan->unitId);
        btObjectMarkForRelease(plan->unitId);
    }
    else
    {
        mnBattleMessageShowAndWaitStub((Sint32)MsgNoEscape);
        btMessageWaitForAdvance();
    }

    view->busy = 0;
    return completed;
}

// Plays a summon or a full-screen Psynergy: the unit glows in its element and the other side fades
// out while the effect plays, then everyone fades back in. STAGING_HIDE_ACTOR hides the unit too,
// as the summons do.
Sint32 btStagingRunFullScreen(BATTLE_PLAN *plan, Sint32 mode)
{
    Uint16                    units[UNIT_MAX];
    BATTLE_PRESENTATION_STATE state;
    Uint32                    actor;
    Uint32                    target;
    Uint32                    count;
    Uint32                    i;
    Uint32                    kept;
    MOTION_OBJECT            *object;

    // The camera turns to the acting unit's side for a summon, else to the usual view.
    btStagingBuildTargetList(plan, &state);
    actor  = plan->unitId;
    target = plan->targetIds[0];
    if (plan->battleEffect & EFFECT_SUMMON)
    {
        BATTLE_BACKGROUND_STATE *view = RamHeapSlots->battleBg;

        if (actor <= ALLY_MAX)
            view->targetYaw = YAW_PARTY_ACTION;
        else
            view->targetYaw = YAW_BEHIND_ENEMIES;
        view->ticks = 60;
    }
    else
    {
        BATTLE_BACKGROUND_STATE *view = RamHeapSlots->battleBg;
        Sint32                   yaw;

        yaw = YAW_ENEMY_ACTION;
        if (actor <= ALLY_MAX)
            yaw = YAW_PARTY_ACTION;
        if (view->targetYaw != yaw)
            view->targetYaw = yaw;
    }

    // The unit raises its hand and glows; the side not targeted fades out.
    btHighlightSetModes(0, HIGHLIGHT_OFF);
    mnPartyStatusDrawStub(RamHeapSlots->battleState->partyStatus & ~PARTY_STATUS_SHOW_PP);
    object     = btObjectGet(actor)->object;
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    i          = 0;
    count      = btUnitListIds(BATTLE_SIDE_MASK_BOTH, units);
    for (; i != count; i++)
    {
        if (units[i] == BATTLE_UNIT_REMOVED)
            continue;
        if ((mode & STAGING_NO_CAST) || units[i] != actor)
        {
            Uint32 unit = units[i];

            if ((target <= ALLY_MAX) != (unit <= ALLY_MAX))
                btHighlightSetSpriteMode(unit, SPRITE_SEMI_TRANSPARENT);
        }
        else
        {
            fdActorSelectAnimationStub((ACTOR *)object, RamHeapSlots->battleState->actingAnimation);
        }
    }
    if (!(mode & STAGING_NO_CAST))
    {
        sySoundPlayStub(SE_CAST);
        btElementGlowPlay(state.actor, plan->element, GLOW_STAR_FIELD, 0);
    }
    if (mode & STAGING_HIDE_ACTOR)
        btHighlightSetSpriteMode(actor, SPRITE_SEMI_TRANSPARENT);
    for (i = 0; i != BLEND_WEIGHT_MAX; i++)
    {
        REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
        sySynch(1);
    }

    // A failed cast: "blocked" (sealed) or too little PP, shown at once.
    if (plan->failure != ACTION_FAILURE_NONE)
    {
        if (plan->failure == ACTION_FAILURE_SEALED)
        {
            btPlaybackPush(PLAYBACK_UNIT_NAME, plan->unitId);
            btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgPsynergyBlocked);
        }
        else
        {
            btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgCastLacksPp);
        }
        btPlaybackRunAll();
        btBackgroundRestorePalette();
    }
    else
    {
        // Only the target side stays on the field.
        kept = 0;
        for (i = 0; i < count; i++)
        {
            Uint32 unit = units[i];

            if (unit == actor)
            {
                if (!(mode & STAGING_HIDE_ACTOR))
                    units[kept++] = actor;
            }
            else if ((target <= ALLY_MAX) == (unit <= ALLY_MAX))
            {
                units[kept++] = unit;
            }
        }
        units[kept] = BATTLE_UNIT_LIST_END;
        btActorSpawnObjectsForList((Sint16 *)units, 0);
        for (i = 0; i < plan->targetCount; i++)
            units[i] = plan->targetIds[i];
        units[i] = BATTLE_UNIT_LIST_END;
        // Each target's colour modes are saved beside the effect command.
        for (i = 0; i != state.count; i++)
        {
            SPR   *animation = btActorGetSprite(btObjectGet(state.actors[i])->object, 0);
            Uint32 child;

            for (child = 0; child != animation->count - 1; child++)
                state.params[i][child] = animation->entries[child]->colorMode;
        }
        // The side the battle effect plays on: the targets'.
        if (plan->battleEffect & EFFECT_SUMMON)
        {
            if (target <= ALLY_MAX)
                state.side = EFFECT_SIDE_PARTY;
            else
                state.side = EFFECT_SIDE_ENEMIES;
        }
        else
        {
            if (plan->targetIds[0] <= ALLY_MAX)
                state.side = EFFECT_SIDE_PARTY;
            else
                state.side = EFFECT_SIDE_ENEMIES;
        }
        if (plan->battleEffect & EFFECT_OTHER_SIDE)
            state.side ^= 1;
        // The battle effect runs to its end while playback, an event, plays each target's outcome
        // from the tick the blow lands.
        sySetEvent(btPlaybackRun, EVENT_PASS_TICK);
        if (plan->battleEffect & EFFECT_SUMMON)
            btEffectRunSummonStub((Sint32 *)&state);
        else
            BATTLE_EFFECT_RUN(&state, plan->battleEffect);
        btPlaybackWaitForReady();
    }

    // Everyone back: the faded units fade in over 16 ticks and every colour mode is cleared.
    btObjectPut();
    count      = btUnitListIds(BATTLE_SIDE_MASK_BOTH, units);
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    for (i = 0; i != count; i++)
    {
        Uint32 unit;

        if (units[i] == BATTLE_UNIT_REMOVED)
            continue;
        // Gaia's caster stays out of the fade back in, and so, as kind SUMMON_EFFECT_KIRIN, does
        // Kirin's summoner: it is back at once where Tiamat's fades in with the rest.
        if (!(mode & STAGING_NO_CAST) && units[i] == actor && state.kind == BATTLE_EFFECT_GAIA)
            continue;
        unit = units[i];
        if ((target <= ALLY_MAX) != (unit <= ALLY_MAX))
            btHighlightSetSpriteMode(unit, SPRITE_SEMI_TRANSPARENT);
    }
    for (i = 0; i != BLEND_WEIGHT_MAX; i++)
    {
        REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
        sySynch(1);
    }
    for (i = 0; i != count; i++)
        btHighlightSetSpriteMode(units[i], SPRITE_OPAQUE);
    btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
    sySynch(1);
    return 0;
}

// Plays an action with no battle effect (STAGING_MESSAGES): each target's outcome is applied and
// its messages shown in turn. With no targets, it only waits for the message to advance.
UNREAD_RESULT btStagingRunMessages(BATTLE_PLAN *plan, Sint32 unused)
{
    Sint32 targetIndex;

    btHighlightSetModes(0, HIGHLIGHT_OFF);
    if (plan->targetCount == 0)
    {
        btMessageWaitForAdvance();
    }
    else
    {
        for (targetIndex = 0; targetIndex < plan->targetCount; targetIndex++)
        {
            btAbilityApplyToTarget(plan, targetIndex);
            btPlaybackRunAll();
        }
    }
    sySynch(1);
}
