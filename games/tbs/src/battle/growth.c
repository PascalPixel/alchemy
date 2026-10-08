#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "btlpty.h"
#include "mem.h"
#include "types.h"
#include "scene.h"
#include "battle_formation.h"
#include "fixed_math.h"
#include "tbs_edition.h"
#include "btlsmn.h"
#include "system.h"
#include "iwram_call.h"
#include "battle_unit.h"
#include "btlwork.h"
#include "character.h"
#include "btlsys.h"
#include "b5_context.h"

struct FormationCandidate {
    s16 record_id;
    s16 score;
};

struct BattleBaseStats {
    s16 base_hp;
    s16 base_ep;
    s16 hp_gauge;
    s16 ep_gauge;
    u16 base_attack;
    u16 base_defense;
    u16 base_agility;
    u8 base_luck;
    u8 action_count;
    u8 hp_regen;
    u8 ep_regen;
    u8 unknown_12[2];
    struct BattleElementStats elm[4];
};

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin)
{
    s32 match_count = 0;
    struct FormationCandidate *pool =
        (struct FormationCandidate *)Runtime_BumpAllocateAlternatePool(128);
    u16 party[8];
    s32 level_total = 0;
    s32 unit_count = BattleParty_PrepareActiveOwners(party);
    s32 i;
    s32 j;
    s32 chance;
    s32 result;

    if (unit_count > 0) {
        u16 *p = party;

        i = unit_count;
        do {
            u8 level = UnitGetFar(*p)->level;

            i--;
            p++;
            level_total += level;
        } while (i != 0);
    }

    chance = level_total / unit_count;
    chance += (s8)GameFlag_GetByteFar(1016);
    if (chance <= 0)
        chance = 1;
    if (chance > 99)
        chance = 99;

    for (i = 0; i <= 31; i++)
        pool[i].score = -1;

    for (i = 0; (u32)i <= 19; i++) {
        Owner_GetRecordFar(BattleFormation_LevelMatchedEnemies[i]);
        GameFlag_ClearBitFar(BattleFormation_LevelMatchedEnemies[i] + 1536);
    }

    for (i = 0; (u32)i <= 379; i++) {
        s32 score = Party_ComputeEligibleMemberAverage(i);

        if (score >= 0 && score <= chance + 3) {
            s32 min_index = -1;
            s32 min_value;

            for (j = 0, min_value = 999; j <= 31; j++) {
                if (pool[j].score < min_value) {
                    min_value = pool[j].score;
                    min_index = j;
                }
            }

            if (min_index >= 0) {
                pool[min_index].score = score;
                pool[min_index].record_id = i;
                match_count++;
            }
        }
    }

    if (match_count > 32)
        match_count = 32;

    if (match_count != 0) {
        struct FormationCandidate *chosen =
            &pool[(match_count *Random16()) >> 16];

        result = chosen->record_id;
        *out_margin = chance - chosen->score;
    } else {
        *out_margin = match_count;
        result = 1;
    }

    MemFree(pool);
    return result;
}

s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
    struct BattleBaseStats *base;
    struct BtlUnit *state;
    s32 value;
    s32 floor;
    s32 i;

    base = (struct BattleBaseStats *)Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
    state = UnitGetFar(owner);
    Iwram_CopyWords(base, &state->base_hp, sizeof(struct BattleBaseStats));

    value = state->base_hp;
    value += levels * 97 / 10;
    floor = base->base_hp * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->base_hp = value;

    value = state->base_ep;
    value += levels * 15 / 10;
    floor = base->base_ep * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->base_ep = value;

    floor = levels * 123 / 10;
    value = state->base_attack;
    value += floor;
    floor = base->base_attack * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->base_attack = value;

    floor = levels * 33 / 10;
    value = state->base_defense;
    value += floor;
    floor = base->base_defense * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->base_defense = value;

    floor = levels * 51 / 10;
    value = state->base_agility;
    value += floor;
    floor = base->base_agility * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->base_agility = value;

    for (i = 0; i < 4; i++) {
        value = state->base_elm[i].pow + levels * 15;
        floor = base->elm[i].pow * 7 / 10;
        if (value < floor)
            value = floor;
        if (value > 200)
            value = 200;
        state->base_elm[i].pow = value;
    }

    state->level += levels;
    UnitCalcFar(owner);
    MemFree(base);
}

#if EDITION_INTERNATIONAL
#define CH_CNT 9
#else
#define CH_CNT 26
#endif

s32 EnmCntDec(s32 no, s32 n)
{
    struct BtlWork *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = BtlWk;
    num = w->summon_count;
    for (i = 0; i < num; i++) {
        if (w->enemy_classes[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->summon_channels[i] < 0) {
            w->summon_channels[i] = 1;
            w->summon_masks[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = (w->summon_channels[i] + 1) % CH_CNT;
            w->summon_channels[i] = ch;
            if ((w->summon_masks[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->summon_masks[i] |= 1 << w->summon_channels[i];
        return w->summon_channels[i];
    }
    if (num <= 4) {
        w->summon_channels[num] = -1;
        w->enemy_classes[num] = no;
        w->summon_masks[num] = 0;
        w->summon_count = num + 1;
        return CH_CNT;
    }
    return -1;
}

s32 EnmCntRel(s32 actor_id)
{
    struct BtlWork *state = BtlWk;
    struct BtlUnit *actor;
    s32 count = state->summon_count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = UnitGetFar(actor_id);
    if (actor->class_index != 0)
        return;

    class_id = actor->class_id;
    for (index = 0; index < count; index++) {
        if (state->enemy_classes[index] == class_id)
            break;
    }
    if (index == count || state->summon_masks[index] == 0)
        return;

    for (name_length = 0; name_length <= 13; name_length++) {
        if (actor->name[name_length] == 0)
            break;
    }

    bit = 32;
    if (name_length > 0)
        bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
    state->summon_masks[index] &= ~(1 << bit);
}

s32 EnmCntClr(s32 class_id)
{
    struct BtlUnit *summon;
    s32 object_index;
    s32 slot;
    s32 next_slot;
    u8 occupied;
    u8 marker;

    object_index = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = UnitGetFar(object_index + 0x80);
    occupied = summon->side;
    if (occupied != 1) goto next_object;
    if (summon->class_id != class_id) goto next_object;
    slot = 0;
    if (summon->name[0] != 0) goto scan_slots;
    summon->name[0] = marker;
    summon->name[occupied] = slot;
    return;
scan_slots:
    slot++;
    if (slot > 13) return;
    occupied = summon->name[slot];
    if (occupied != 0) goto scan_slots;
    next_slot = slot + 1;
    summon->name[slot] = marker;
    summon->name[next_slot] = occupied;
    return;
next_object:
    object_index++;
    if (object_index <= 5) goto scan_objects;
}
