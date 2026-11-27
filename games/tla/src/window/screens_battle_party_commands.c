// An ability classified for target selection, and a reserve member chosen for a party slot. Part of
// screens.c.

// Classifies ability `act`'s effect for target selection: the hint mnBattleTargetRunSelection shows
// on a party member and starts on (enum BATTLE_TARGET_HINT).
Sint32 mnBattleClassifyEntryKind(const ABILITY_DEFINITION *act)
{
    Sint32 abilityKind;
    Sint32 targetKind;

    targetKind  = TARGET_HINT_NONE;
    abilityKind = act->flags & ABILITY_FORMULA_MASK;
    if (abilityKind == ABILITY_FORMULA_HEAL_HP)
        targetKind = TARGET_HINT_HP;
    if (abilityKind == ABILITY_FORMULA_HEAL_PP)
        targetKind = TARGET_HINT_PP;
    if (act->effect == ABILITY_EFFECT_CURE_POISON)
        targetKind = TARGET_HINT_CURE_POISON;
    if (act->effect == ABILITY_EFFECT_RESTORE)
        targetKind = TARGET_HINT_RESTORE;
    if (act->effect == ABILITY_EFFECT_CURE_ALL)
        targetKind = TARGET_HINT_CURE_ALL;
    if (gmBattleIsReviveEffectStub(act->effect) != 0)
        targetKind = TARGET_HINT_REVIVE;
    return targetKind;
}

// Lets the player choose which reserve member takes a party slot in battle, previewing each choice
// in place: left and right cycle through the reserve, A confirms and swaps the two in
// MainRoutineStateWork.party, and B puts the slot's member back.
Sint32 mnBattlePartyChooseReserve(Sint32 slot)
{
    Uint16             owners[6];
    Uint16             selected[2];
    Sint32             count     = btPartyPrepareReserveOwnersStub(owners);
    Sint32             selection = 0;
    Bool               redraw    = TRUE;
    BATTLE_STATE      *battle    = RamHeapSlots->battleState;
    Sint32             result    = 0;
    BATTLE_MENU_STATE *menu      = RamHeapSlots->battleMenuState;
    Sint32             current;
    Sint32             newId;
    Sint32             i;

    if (count <= 0)
        return -1;
    // mnBattleTargetRunSelection's PLACE_PARTY | place: the place is its low byte.
    slot    &= 0xff;
    current  = battle->partyUnits[slot];
    sySoundPlayStub(SE_MENU_CONFIRM);
    for (;;)
    {
        newId = battle->partyUnits[slot] = owners[selection];
        if (PerWork.push & KEY_B)
        {
            battle->partyUnits[slot] = current;
            result                   = -1;
            sySoundPlayStub(SE_MENU_CANCEL);
            break;
        }
        // Japanese also takes A on the first tick, before the first preview has been drawn.
#if PARTY_SWAP_WAITS_FOR_PREVIEW
        if ((PerWork.push & KEY_A) && !redraw)
        {
#else
        if (PerWork.push & KEY_A)
        {
#endif
            // The preview already stands in the slot: a swap that would leave no one standing is
            // refused.
            if (btUnitListLivingIdsStub(BATTLE_SIDE_MASK_PARTY, 0) == 0)
            {
                sySoundPlayStub(SE_MENU_ERROR);
                continue;
            }
            sySoundPlayStub(SE_CHIME);
            menu->swappedUnit  = current;
            menu->swappedPlace = slot;
            for (i = 0; i < 8; i++)
            {
                if (MainRoutineStateWork.party[i] == current)
                    MainRoutineStateWork.party[i] = newId;
                else if (MainRoutineStateWork.party[i] == newId)
                    MainRoutineStateWork.party[i] = current;
            }
            break;
        }
        if (PerWork.repeat & KEY_LEFT)
        {
            selection = (selection + count - 1) % count;
            sySoundPlayStub(SE_MENU_CONFIRM);
            redraw = TRUE;
        }
        else if (PerWork.repeat & KEY_RIGHT)
        {
            selection = (selection + 1) % count;
            sySoundPlayStub(SE_MENU_CONFIRM);
            redraw = TRUE;
        }
        if (redraw)
        {
            BATTLE_OBJECT *from;
            BATTLE_OBJECT *to;
            ACTOR         *actor;
            ACTOR         *place;

            redraw = FALSE;
            newId = battle->partyUnits[slot] = owners[selection];
            from                             = btObjectGetStub(current);
            to                               = btObjectGetStub(newId);
            to->anchorX                      = from->anchorX;
            place                            = (ACTOR *)from->object;
            actor                            = (ACTOR *)to->object;
            to->anchorZ                      = from->anchorZ;
            fdActorSetPosAndResetMotionStub(actor, place->x.fixed, place->y.fixed, place->z.fixed);
            mnPartyStatusDraw(RamHeapSlots->battleState->partyStatus);
            btObjectPutStub();
            selected[0] = newId;
            selected[1] = BATTLE_UNIT_LIST_END;
            btHighlightSetModesStub(selected, HIGHLIGHT_PULSE);
            mnPartyStatusHighlightMembers(selected);
        }
        sySynch(1);
    }
    selected[0] = BATTLE_UNIT_LIST_END;
    btHighlightSetModesStub(selected, HIGHLIGHT_OFF);
    mnPartyStatusHighlightMembers(selected);
    mnPartyStatusDraw(RamHeapSlots->battleState->partyStatus);
    btObjectPutStub();
    sySynch(1);
    return result;
}

// Lists the units still standing on the sides in sideMask, party first, into unitIds when it is not
// null, and returns how many.
Sint32 btUnitListLivingStub(Sint32 sideMask, Uint16 *unitIds);

// The first half of The Broken Seal's BattleBuildPartyActions: each action of a living party member
// who is asleep, confused, charmed or stunned is lost (COMMAND_NONE) and written at *cursor, which
// moves past them, their count going to *forcedCount; every other member's actions are listed in
// ready to be chosen. Returns how many are listed.
Sint32 mnBattlePartySplitChoosers(Uint16 *ready, BATTLE_ACTION_RECORD **cursor, Sint32 *forcedCount)
{
    Uint16               *unitIds    = syHeapAllocateEWRAM(9);
    BATTLE_ACTION_RECORD *entries    = *cursor;
    Sint32                count      = btUnitListLivingStub(BATTLE_SIDE_MASK_PARTY, unitIds);
    Sint32                readyCount = 0;
    Sint32                forced     = 0;
    Sint32                i;
    Sint32                action;

    for (i = 0; i < count; i++)
    {
        UNIT *unit = syUnitGet(unitIds[i]);

        for (action = 0; action < unit->actionsPerTurn; action++)
        {
            if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 || unit->stun != 0)
            {
                BATTLE_ACTION_RECORD *entry = &entries[forced];

                entry->unitId               = unitIds[i];
                entry->priority             = unit->agility;
                entry->command              = COMMAND_NONE;
                entry->param                = 0;
                // The other side's place 0, as a lost action's target.
                entry->target               = PLACE_ENEMY;
                forced++;
            }
            else
            {
                ready[readyCount++] = unitIds[i];
            }
        }
    }
    *forcedCount  = forced;
    entries      += forced;
    *cursor       = entries;
    return readyCount;
}
