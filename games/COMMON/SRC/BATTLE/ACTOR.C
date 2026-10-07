#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "EDITION.H"
#include "BTLPTY.H"
#include "BTLSYS.H"
#include "BTLTYPE.H"
#include "BTLWORK.H"
#include "BTLSMN.H"
#include "MOTOBJ.H"
#include "ANIMSPR.H"
#include "SYSTEM.H"

struct BtlUnit *UnitGet(s32);
void Animation_ApplyChildArgumentFar(void *, s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);

#if GAME_TLA
s32 BattleActor_RemoveFromLists(s32 actor)
#else
void BattleActor_RemoveFromLists(s32 actor)
#endif
{
    struct BtlWork *work;
    s32 i;
#if GAME_TLA
    /* FAKEMATCH: retain r2 and integer-load/pointer-store operands; natural loops split the removal exit. */
    register s32 slot asm("r2");
#endif
    u32 j;
    s32 unit;

#if GAME_TLA
    work = Ram_HeapSlots->battle_work;
    UnitGet(actor)->side = BATTLE_UNIT_ABSENT;
    i = (u32)&((struct BtlWork *)0)->party_units;
    slot = i;
    unit = *(s16 *)(slot + (u32)work);
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
    work = BtlWk;
    UnitGetFar(actor)->side = BATTLE_UNIT_ABSENT;
    for (i = 0; ; i++) {
        if (work->party_units[i] == actor) {
            work->party_units[i] = BATTLE_UNIT_REMOVED;
            goto removed;
        }
        if (work->party_units[i] == BATTLE_UNIT_LIST_END)
            break;
    }
#endif
    /* FAKEMATCH: the goto loop avoids the pointer walk and saved 0xfe constant produced by a for loop. */
    do {
        j = 0;
again:
        unit = work->enemy_units[j];
#if GAME_TLA
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
#if GAME_TLA
            break;
#else
            return;
#endif
        goto again;
    } while (0);
#if GAME_TLA
    return -1;
enemy_removed:
    work->enemy_units[j] = BATTLE_UNIT_REMOVED;
    goto removed;
party_removed:
    *(s16 *)((u8 *)work + slot) = BATTLE_UNIT_REMOVED;
#endif
removed:
    EnmCntRel(actor);
    for (j = 0; j < 20; j++) {
        if (work->actions[j].unit_id == actor)
            work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
    }
#if GAME_TLA
    return 0;
#endif
}

void BattleMotion_InitializeActorRecords(s32 id)
{
    void *items[4];
    struct BtlUnit *state;
    struct AnimationObject *item;
#if GAME_TLA
    /* FAKEMATCH: ordering the child load swaps r2/r3 unless it stays in r2. */
    register struct AnimationEntry *child asm("r2");
#else
    struct AnimationEntry *child;
#endif
    s32 index;

#if GAME_TLA
    state = UnitGet(id);
#else
    state = UnitGetFar(id);
#endif
    index = 0;
    while ((item = GetMotionRecord(BtlObjGet(id)->object, index)) != 0) {
        if (state->side != BATTLE_UNIT_ENEMY)
#if GAME_TLA
            Animation_ApplyChildArgumentFar(item, 4);
#else
            AnimationObjects_SelectAnimationFar(item, 4);
#endif
        else
#if GAME_TLA
            Animation_ApplyChildArgumentFar(item, 5);
#else
            AnimationObjects_SelectAnimationFar(item, 5);
#endif
        index++;
    }

    if (state->side == BATTLE_UNIT_ENEMY) {
        index = 0;
        while ((item = GetMotionRecord(BtlObjGet(id)->object, index)) != 0) {
            child = item->entries[0];
#if GAME_TLA
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
