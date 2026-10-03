#include "TYPES.H"
#include "EDITION.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_SUMMON.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "SYSTEM.H"

struct BattleUnit *Owner_GetState(s32);
void Animation_ApplyChildArgumentFar(void *, s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
s32 BattleActor_RemoveFromLists(s32 actor)
#else
void BattleActor_RemoveFromLists(s32 actor)
#endif
{
    struct BattleSession *work;
    s32 i;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    /* FAKEMATCH: retain r2 and integer-load/pointer-store operands; natural loops split the removal exit. */
    register s32 slot asm("r2");
#endif
    u32 j;
    s32 unit;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    work = Ram_HeapSlots->battle_work;
    Owner_GetState(actor)->status_12a = BATTLE_UNIT_ABSENT;
    i = (u32)&((struct BattleSession *)0)->party_units;
    slot = i;
    unit = *(s16 *)(slot + (u32)work);
    /* FAKEMATCH: keep the initial signed list read ahead of splitting the removal exits. */
    asm("" : "+r" (slot), "+r" (i) : "r" (unit));
    if (unit == actor)
        goto party_removed;
    if (unit != BATTLE_UNIT_LIST_END) {
        do {
            i += sizeof(s16);
            slot = i;
            unit = *(s16 *)(slot + (u32)work);
            if (unit == actor)
                goto party_removed;
        } while (unit != BATTLE_UNIT_LIST_END);
    }
#else
    work = gBattleWork;
    Owner_GetStateFar(actor)->status_12a = BATTLE_UNIT_ABSENT;
    for (i = 0; ; i++) {
        if (work->party_units[i] == actor) {
            work->party_units[i] = BATTLE_UNIT_REMOVED;
            goto removed;
        }
        if (work->party_units[i] == BATTLE_UNIT_LIST_END)
            break;
    }
#endif
    /* FAKEMATCH: the enemy scan is a goto loop inside a block that runs once,
     * which keeps the loop pass off it. Written as a for like the party scan
     * it compiles to a pointer walk with the 0xfe held in a register. */
    do {
        j = 0;
again:
        unit = work->enemy_units[j];
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
        if (unit == actor)
            goto enemy_removed;
#else
        if (unit == actor) {
            work->enemy_units[j] = BATTLE_UNIT_REMOVED;
            goto removed;
        }
#endif
        j++;
        if (unit == BATTLE_UNIT_LIST_END)
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            break;
#else
            return;
#endif
        goto again;
    } while (0);
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    return -1;
enemy_removed:
    work->enemy_units[j] = BATTLE_UNIT_REMOVED;
    goto removed;
party_removed:
    *(s16 *)((u8 *)work + slot) = BATTLE_UNIT_REMOVED;
#endif
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->actions[j].unit_id == actor)
            work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
    }
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    return 0;
#endif
}

void BattleMotion_InitializeActorRecords(s32 id)
{
    void *items[4];
    struct BattleUnit *state;
    struct AnimationObject *item;
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    /* FAKEMATCH: ordering the child load swaps r2/r3 unless it stays in r2. */
    register struct AnimationEntry *child asm("r2");
#else
    struct AnimationEntry *child;
#endif
    s32 index;

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    state = Owner_GetState(id);
#else
    state = Owner_GetStateFar(id);
#endif
    index = 0;
    while ((item = GetMotionRecord(GetBattleObjectSlot(id)->object, index)) != 0) {
        if (state->status_12a != BATTLE_UNIT_ENEMY)
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            Animation_ApplyChildArgumentFar(item, 4);
#else
            AnimationObjects_SelectAnimationFar(item, 4);
#endif
        else
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            Animation_ApplyChildArgumentFar(item, 5);
#else
            AnimationObjects_SelectAnimationFar(item, 5);
#endif
        index++;
    }

    if (state->status_12a == BATTLE_UNIT_ENEMY) {
        index = 0;
        while ((item = GetMotionRecord(GetBattleObjectSlot(id)->object, index)) != 0) {
            child = item->entries[0];
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            /* FAKEMATCH: load the child before the index shift; ordinary loops reverse them. */
            asm("" : "+r" (child), "+r" (index));
#endif
            items[index] = item;
            child->param = 6;
            child->frame = 0xff;
            index++;
        }
        WaitFrames(4);
        BattleActor_RemoveFromLists(id);
        Map_RenderAllAnimatedTileFramesFar(items, index);
        ActivateBattleObjectSlot(id);
    }
}
