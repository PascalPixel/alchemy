#include "TYPES.H"
#include "EDITION.H"

#include "BATTLE_WORK.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PRESENTATION.H"
#include "OWNER_STATE.H"

extern struct BattlePresentationTransition *gTransitionWork;
void BattleUnit_ResetGuardLevels(void);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void Camera_ConfigureScene(s32 scene);
void UiWork_FinalizeSharedSlotFar(void);
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *actions);
s32 BattleEscape_CheckSuccess(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
s32 BattlePres_WaitSync(void);
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *actions);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
void Camera_InitDefaultTransform(void);

void Palette_CopyBanksWithBrightnessOffset(s32 offset)
{
    s32 iter;
    s32 bank;
    s32 col;
    s32 mask;
    s32 idx;

    bank = 15;
    iter = 0;
    mask = 31;
    do
    {
        for (col = 0; col <= 15; col++)
        {
            u32 color;
            s32 blue;
            s32 green;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            /* FAKEMATCH: the red-mask copy requires r1 before the channel shifts. */
            register s32 red __asm__("r1");
#else
            s32 red;
#endif
            s32 value;

            idx = bank * 16 + col;
            color = ((u16 *)0x05000000)[idx];
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            /* FAKEMATCH: copy the red mask after the color load, before the channel shifts. */
            __asm__("add %0, %1, #0" : "=r" (red) : "r" (mask), "r" (color));
#endif
            blue = (color >> 10) & mask;
            green = (color >> 5) & mask;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            red &= color;
#else
            red = color & mask;
#endif
            blue += offset;
            green += offset;
            red += offset;
            if (blue > 31)
                blue = 31;
            if (green > 31)
                green = 31;
            if (red > 31)
                red = 31;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            value = blue << 10;
            value |= green << 5;
            value |= red;
            ((s16 *)0x04FFFFE0)[idx] = (s16)value;
        }
        iter++;
        bank = 5;
    } while (iter <= 1);
}

/* Builds and sorts this turn's actions, setting guard levels for defensive
   actions. Returns the action count, or -1 when the link fails. */
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions)
{
    struct BattleUnit *character;
    struct BattleSession *battle;
    u8 *mode;
    s32 count;
    s32 added;
    s32 i;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    battle = Ram_HeapSlots->battle_work;
#else
    battle = gBattleWork;
#endif
    {
        struct BattleActionRecord *queued = battle->actions;
        u32 n;

        for (n = 0; n < 20; n++) {
            queued[n].unit_id = BATTLE_UNIT_LIST_END;
            queued[n].priority = 0x8000;
        }
    }

    BattleUnit_ResetGuardLevels();
    Palette_CopyBanksWithBrightnessOffset(8);
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    GameFlag_SetBit(0x16b);
#else
    GameFlag_SetBitFar(0x16b);
#endif
    mode = &battle->encounter_mode;
    Camera_ConfigureScene(0);
    UiWork_FinalizeSharedSlotFar();

    if (*mode != 2) {
        count = BattlePres_BuildUnitEntries(actions);
        if (count < 0)
            goto finish;
        if (count != 0 && actions[0].command == 99 && BattleEscape_CheckSuccess() == 0)
            *mode = 2;
    } else {
        count = 0;
    }

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    if (Ram_HeapSlots->battle_work->two_sided != 0) {
#else
    if (gBattleWork->two_sided != 0) {
#endif
        added = BattlePresentation_AppendLinkedActions(actions, count);
        if (BattlePres_WaitSync() < 0) {
            count = -1;
            goto finish;
        }
        count += added;
        if (added < 0) {
            count = -1;
            goto finish;
        }
    } else {
        count += BattlePres_BuildOpponentEntries(actions + count);
    }

    BattleQueue_SortByPriority(actions, count);
    if (count > 0) {
        struct BattleActionRecord *action = actions;

        i = count;
        do {
            if (action->command == 3 || action->command == 7) {
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
                character = Owner_GetState(action->unit_id);
                character->guard_level = action->command == 3 ? 1 : 3;
#else
                character = Owner_GetStateFar(action->unit_id);
                character->guard_level = action->command == 3 ? 1 : 2;
#endif
            }
            action++;
            i--;
        } while (i != 0);
    }

finish:
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    GameFlag_ClearBit(0x16b);
#else
    GameFlag_ClearBitFar(0x16b);
#endif
    Camera_InitDefaultTransform();
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    Ram_HeapSlots->transition_work->target_yaw = 0x2000;
#else
    gTransitionWork->target_yaw = 0x2000;
#endif
    return count;
}

/* A tagged slot names a member by its low nibble; bit 7 selects the enemy. */
s16 Battle_GetTaggedSlotValue(s32 slot)
{
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    struct BattleSession *battle = Ram_HeapSlots->battle_work;
#else
    struct BattleSession *battle = gBattleWork;
#endif

    if ((slot & 0x80) != 0)
        return battle->enemy_units[slot & 0xf];
    return battle->party_units[slot & 0xf];
}

/* Removed members are skipped; the end marker means no matching slot. */
s32 Battle_FindTaggedSlotByValue(u32 value)
{
    /* FAKEMATCH: indexing the two arrays introduces index shifts and moves the
       session load. Keep the existing separate byte walks over its members. */
    s32 index;
    s32 tag;
    s32 offset;
    char *base;
    s16 item;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    base = (char *)Ram_HeapSlots->battle_work;
#else
    base = (char *)gBattleWork;
#endif
    if (value <= 7) {
        tag = 0x80;
        index = 0;
        tag <<= 1;
        offset = (u32)&((struct BattleSession *)0)->party_units;
first:
        item = *(s16 *)(offset + (u32)base);
        if (item == BATTLE_UNIT_LIST_END)
            return -1;
        if (item == BATTLE_UNIT_REMOVED)
            goto next_first;
        if (item == value)
            return index | tag;
next_first:
        offset += sizeof(s16);
        index++;
        goto first;
    }

    tag = 0xc0;
    index = 0;
    base += sizeof(s16);
    tag <<= 1;
    offset = (u32)&((struct BattleSession *)0)->enemy_units - sizeof(s16);
second:
    item = *(s16 *)(offset + (u32)base);
    if (item == BATTLE_UNIT_LIST_END)
        return -1;
    if (item == BATTLE_UNIT_REMOVED)
        goto next_second;
    if (item == value)
        return index | tag;
next_second:
    offset += sizeof(s16);
    index++;
    goto second;
}
