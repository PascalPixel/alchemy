#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "battle_party.h"
#include "b5_context.h"

struct PlacementList {
    struct DjinnRecoveryEntry entries[72];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};


extern char MsgHpRecover;
extern char MsgHpFull;

u32 BattleEventRuntime_Reset(void);

s32 sub_08124af8(s32 id)
{
    struct BattleUnit *unit = UnitGet(id);
    u8 *count = &unit->refrain;

    if (*count != 0) {
        s32 max;
        s32 before;
        s32 hp;
        s32 gain;

        BattleEventRuntime_Reset();
        hp = unit->hp;
        max = unit->maxHp;
        before = hp;
        hp += max * (*count + 1) / 10;
        if (hp > max)
            hp = max;
        gain = hp - before;
        if (gain != 0) {
            BattleEvPush(0, id);
            if (hp == unit->maxHp) {
                BattleEvPush(4, (u32)&MsgHpFull);
            } else {
                {
                    u32 opcode = 1;

                    /* FAKEMATCH: the barrier builds the opcode before the gain is copied to its argument register, as the native does; plain, the copy comes first (one reordered pair, differ 2). */
                    asm volatile("" : "+l"(opcode));
                    BattleEvPush(opcode, gain);
                }
                BattleEvPush(4, (u32)&MsgHpRecover);
            }
            unit->hp = hp;
            UnitRatioFar(id);
        }
        BattleEv_DispatchQueued();
        if (--unit->refrain == 0)
            return 1;
    }
    return 0;
}

s32 BattleUnit_TickReflect(s32 id)
{
    u8 *value = (u8 *)&((struct BattleUnit *)UnitGet(id))->reflect;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id)
{
    struct BattleUnit *unit = UnitGet(id);
    u8 *value = &unit->agiCnt;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            unit->agiLv = 0;
            return 1;
        }
    }
    return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void)
{
    struct PlacementList *list;
    struct DjinnRecoveryEntry *timed_entry;
    struct DjinnRecoveryEntry *expired_entry;
    s32 index;
    s32 removed;
    s32 initial_count;

    list = &Trade_GetOfferStateFar(0)->list;
    initial_count = list->count;
    index = 0;
    removed = 0;
    if (index < initial_count) {
        timed_entry = list->entries;
        do {
            if (timed_entry->turns > 0 &&
                ((struct BattleUnit *)UnitGet(timed_entry->unitId))->hp != 0) {
                timed_entry->turns--;
            }
            index++;
            timed_entry++;
        } while (index < list->count);
    }
    index = 0;
    if (index < list->count) {
        expired_entry = list->entries;
        do {
            if (expired_entry->turns == 0) {
                u8 id = expired_entry->unitId;

                Djinn_ActivateFar(id, expired_entry->element, expired_entry->index);
                Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
                UnitCalcFar(id);
                removed = 1;
            } else {
                expired_entry++;
                index++;
            }
        } while (index < list->count);
    }
    return removed;
}

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
    s32 turns;

    turns = 19;
    do {
        turns--;
        BattlePlacement_UpdateTimedEntries();
    } while (turns >= 0);
    return 0;
}

void Func_08124cc0(void)
{
}
