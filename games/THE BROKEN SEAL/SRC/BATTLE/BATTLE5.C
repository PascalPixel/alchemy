#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_FORMATION.H"
#include "FIXED_MATH.H"
#include "TBS_EDITION.H"
#include "BATTLE_SUMMON.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(s32, s32, InterruptHandler);

extern struct BattleFormationRecord BattleFormation_Records[];

struct Object_080c1a34 {
    u8 padding_00[15];
    u8 value;
};

struct Object_080c1a34 *Owner_GetRecordFar(s32 id);
s32 GameFlag_IsSet(s32 flag);

s32 GameFlag_ClearBitFar(s32 id);
void Runtime_BumpFree(void *ptr);
s32 BattleParty_PrepareActiveOwners(u16 *out_units);

struct BattleUnitLevel {
    u8 reserved_00[0x0f];
    u8 level;
};

struct FormationCandidate {
    s16 record_id;
    s16 score;
};

struct BattleUnitLevel *Owner_GetStateFar(s32 unit_id);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
s32 Party_ComputeEligibleMemberAverage(s32 record_id);
extern u16 RomBytes_080c73f8[];
s32 GameFlag_GetByteFar(s32 id);

struct OwnerElementStats {
    s16 power;
    s16 resist;
};

struct OwnerStats {
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u8 luck;
    u8 unknown_0f[5];
    struct OwnerElementStats elements[4];
};

struct OwnerState {
    u8 unknown_00[15];
    u8 level;
    struct OwnerStats stats;
};

void Owner_RecalculateStatsFar(s32 owner);
void Runtime_BumpFree(void *block);

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];   /* 0x10 */
    u32 used_masks[6];  /* 0x1c */
    s8 channels[6];     /* 0x34 */
    u8 unknown_3a[6];
    u8 count;           /* 0x40 */
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[282];
    u8 class_id;
    u8 unavailable;
};

extern struct SummonChargeState *gBattleWork;

void Runtime_RemoveIrqHandlerSlot2(void)
{
  int no;
  unsigned long long handler;
  handler = 2;
  no = handler;
  handler = 0;
  Runtime_SetIrqHandler(no, 0, (InterruptHandler)handler);
}

s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
    volatile u8 scratch[28];
    struct BattleFormationRecord *record;
    s32 member_index;
    s32 level_sum;
    s32 eligible_count;

    eligible_count = 0;
    level_sum = 0;
    record = &BattleFormation_Records[record_id];
    (void)scratch;

    member_index = 0;
    if (record->minimum_counts[0] == 0) {
        u8 *present;

        present = record->minimum_counts;
        do {
            member_index++;
            if ((u32)member_index > 4)
                break;
            present++;
        } while (*present == 0);
    }
    if (member_index == 5)
        return -1;

    member_index = 0;
    do {
        if (record->maximum_counts[member_index] != 0) {
            struct Object_080c1a34 *object;
            s32 member;

            member = record->member_ids[member_index];
            object = Owner_GetRecordFar(member + 8);
            if (object != 0) {
                if (object->value <= 3 ||
                    GameFlag_IsSet(372) != 0 ||
                    GameFlag_IsSet(member + 1544) != 0) {
                    level_sum += object->value;
                    eligible_count++;
                } else {
                    return -2;
                }
            }
        }
        member_index++;
    } while ((u32)member_index <= 4);

    if (eligible_count == 0)
        return -3;
    return level_sum / eligible_count;
}

/* battle/formation/select_level_matched_candidate.c */
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
            u8 level = Owner_GetStateFar((s32)*p)->level;

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
        Owner_GetRecordFar(RomBytes_080c73f8[i]);
        GameFlag_ClearBitFar(RomBytes_080c73f8[i] + 1536);
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
                pool[min_index].score = (s16)score;
                pool[min_index].record_id = (s16)i;
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

    Runtime_BumpFree(pool);
    return result;
}

/* Raise an owner by levels: each stat grows by a fixed tenth-step per level,
   never below 70% of its value before the change and never past its cap.
   Declared s32 without a return statement, as the ROM keeps r0 live. */
s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
    struct OwnerStats *base;
    struct OwnerState *state;
    struct OwnerStats *stats;
    s32 value;
    s32 floor;
    s32 i;

    base = Runtime_BumpAllocateAlternatePool(sizeof(struct OwnerStats));
    state = Owner_GetStateFar(owner);
    stats = &state->stats;
    Iwram_CopyWords(base, stats, sizeof(struct OwnerStats));

    value = stats->max_hp;
    value += levels * 97 / 10;
    floor = base->max_hp * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    stats->max_hp = value;

    value = state->stats.max_pp;
    value += levels * 15 / 10;
    floor = base->max_pp * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->stats.max_pp = value;

    floor = levels * 123 / 10;
    value = state->stats.attack;
    value += floor;
    floor = base->attack * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.attack = value;

    floor = levels * 33 / 10;
    value = state->stats.defense;
    value += floor;
    floor = base->defense * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.defense = value;

    floor = levels * 51 / 10;
    value = state->stats.agility;
    value += floor;
    floor = base->agility * 7 / 10;
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.agility = value;

    for (i = 0; i < 4; i++) {
        value = state->stats.elements[i].power + levels * 15;
        floor = base->elements[i].power * 7 / 10;
        if (value < floor)
            value = floor;
        if (value > 200)
            value = 200;
        state->stats.elements[i].power = value;
    }

    state->level += levels;
    Owner_RecalculateStatsFar(owner);
    Runtime_BumpFree(base);
}

/* 召喚チャージ管理。クラスごとに使用中チャンネルのビットを持ち、 */
/* 取得・解放・名前印のリセットを行う。 */
#if defined(TBS_EDITION_JA)
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

/* 番号表を線形探索し、既存なら次の空きビットを剰余で回して確保、 */
/* 無ければ表末尾に新規登録する。 */
s32 Summon_TakeCharge(s32 no, s32 n)
{
    struct SummonChargeState *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = gBattleWork;
    num = w->count;
    for (i = 0; i < num; i++) {
        if (w->class_ids[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->channels[i] < 0) {
            w->channels[i] = 1;
            w->used_masks[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = (w->channels[i] + 1) % CH_CNT;
            w->channels[i] = ch;
            if ((w->used_masks[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->used_masks[i] |= 1 << w->channels[i];
        return w->channels[i];
    }
    if (num <= 4) {
        w->channels[num] = -1;
        w->class_ids[num] = no;
        w->used_masks[num] = 0;
        w->count = num + 1;
        return CH_CNT;
    }
    return -1;
}

s32 Summon_ReleaseCharge(s32 actor_id)
{
    struct SummonChargeState *state = gBattleWork;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = Owner_GetStateFar(actor_id);
    if (actor->unavailable != 0)
        return;

    class_id = actor->class_id;
    for (index = 0; index < count; index++) {
        if (state->class_ids[index] == class_id)
            break;
    }
    if (index == count || state->used_masks[index] == 0)
        return;

    for (name_length = 0; name_length <= 13; name_length++) {
        if (actor->name[name_length] == 0)
            break;
    }

    bit = 32;
    if (name_length > 0)
        bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
    state->used_masks[index] &= ~(1 << bit);
}

s32 Summon_ResetCharge(s32 class_id)
{
    u8 *summon;
    s32 object_index;
    s32 slot;
    s32 next_slot;
    u8 occupied;
    u8 marker;

    object_index = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = (u8 *)Owner_GetStateFar(object_index + 0x80);
    occupied = summon[298];
    if (occupied != 1) goto next_object;
    if (summon[296] != class_id) goto next_object;
    slot = 0;
    if (summon[0] != 0) goto scan_slots;
    summon[0] = marker;
    summon[occupied] = slot;
    return;
scan_slots:
    slot++;
    if (slot > 13) return;
    occupied = summon[slot];
    if (occupied != 0) goto scan_slots;
    next_slot = slot + 1;
    summon[slot] = marker;
    summon[next_slot] = occupied;
    return;
next_object:
    object_index++;
    if (object_index <= 5) goto scan_objects;
}
