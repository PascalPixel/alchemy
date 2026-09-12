#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "gs1_edition.h"
#include "battle_summon.h"
#include "battle_formation.h"

/* battle/formation/select_level_matched_candidate.c */
u32 Random16(void);

struct BattleUnitLevel {
    u8 reserved_00[0x0f];
    u8 level;
};

struct FormationCandidate {
    s16 record_id;
    s16 score;
};

struct BattleUnitLevel *Runtime_GetObject(s32 unit_id);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);

s32 Party_ComputeEligibleMemberAverage(s32 record_id);
struct BattleActorDefinition *Battle_Run(s32 actor_id);

extern struct Entry gRom[];

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin)
{
    s32 match_count = 0;
    struct FormationCandidate *pool =
        (struct FormationCandidate *)Runtime_BumpAllocateAlternatePool(128);
    u16 party[8];
    s32 level_total = 0;
    s32 unit_count = Battle_Check(party);
    s32 i;
    s32 j;
    s32 chance;
    s32 result;

    if (unit_count > 0) {
        u16 *p = party;

        i = unit_count;
        do {
            u8 level = Runtime_GetObject((s32)*p)->level;

            i--;
            p++;
            level_total += level;
        } while (i != 0);
    }

    chance = FixedPoint_Ratio(level_total, unit_count);
    chance += (s8)GameFlag_GetByte(1016);
    if (chance <= 0)
        chance = 1;
    if (chance > 99)
        chance = 99;

    for (i = 0; i <= 31; i++)
        pool[i].score = -1;

    for (i = 0; (u32)i <= 19; i++) {
        Battle_Run(gRom[i]);
        GameFlag_Clear(gRom[i] + 1536);
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

    Battle_Do(pool);
    return result;
}

/* battle/summon/take_charge.c */
#if defined(GS1_EDITION_JA)
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

/* 効果音チャンネル管理。番号表を線形探索し、既存なら次の空きビットを */
/* 剰余で回して確保、無ければ表末尾に新規登録する。 */

struct Snd_080c1df4 {
    u8 pad00[0x10];
    u16 no[6];      /* 0x10 */
    u32 use[6];     /* 0x1c */
    s8 ch[6];       /* 0x34 */
    u8 pad3a[6];
    u8 num;         /* 0x40 */
};

extern struct SummonChargeState *gBattleWork;

s32 Modulo(s32, s32);

s32 Summon_TakeCharge(s32 no)
{
    struct Snd_080c1df4 *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = gBattleWork;
    num = w->num;
    for (i = 0; i < num; i++) {
        if (w->no[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->ch[i] < 0) {
            w->ch[i] = 1;
            w->use[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = Modulo(w->ch[i] + 1, CH_CNT);
            w->ch[i] = ch;
            if ((w->use[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->use[i] |= 1 << w->ch[i];
        return w->ch[i];
    }
    if (num <= 4) {
        w->ch[num] = -1;
        w->no[num] = no;
        w->use[num] = 0;
        w->num = num + 1;
        return CH_CNT;
    }
    return -1;
}

/* battle/summon/release_charge.c */
struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];
    u32 used_masks[6];
    s8 channels[6];
    u8 unknown_3a[6];
    u8 count;
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[282];
    u8 class_id;
    u8 unavailable;
};



s32 Summon_ReleaseCharge(s32 actor_id)
{
    struct SummonChargeState *state = gBattleWork;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = Battle_Run(actor_id);
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

/* battle/summon/reset_charge.c */

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
    summon = Runtime_GetObject(object_index + 0x80);
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

/* battle/formation/select_random_available_member.c */

s32 BattleFormation_SelectRandomAvailableMember(s32 record_id)
{
    s32 choices[5];
    struct BattleFormationRecord *record;
    s32 *choices_base;
    s32 *choice_cursor;
    u8 *maximum_count;
    u8 *member_id;
    u32 count;
    s32 remaining;

    count = 0;
    if ((u32)record_id >= 380)
        record_id = 1;

    choices_base = choices;
    record = &gRom[record_id];
    member_id = record->member_ids;
    maximum_count = record->maximum_counts;
    choice_cursor = choices_base;
    remaining = 4;
    do {
        if (*maximum_count++ != 0) {
            *choice_cursor++ = *member_id + 8;
            count++;
        }
        member_id++;
        remaining--;
    } while (remaining >= 0);

    return choices[(Random16() * count) >> 16];
}

/* battle/summon/get_entry_byte3_kind.c */
extern const u8 gRom[];

u32 Summon_GetEntryByte3Kind(s32 arg0)
{
  u32 kind;
  u8 *p;
  p = (u8 *)((arg0 * 8) + (s32)gRom);
  kind = ((u8)(*((u8 *)(p + 3)))) >> 5;
  if (((s32)kind) > 4)
  {
    kind = -1U;
  }
  return kind;
}

/* battle/summon/get_entry_value.c */
struct Entry {
    u16 value;
    u8 rest[6];
};


s32 Summon_GetEntryValue(s32 index)
{
    if ((u32)index > 171)
        return gRom[0].value;
    return gRom[index].value;
}

/* battle/summon/get_entry_flag1_field.c */
struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};


s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 171)
        return gRom[0].value;
    return ((u32)gRom[index].flags1 << 27) >> 28;
}

/* battle/summon/is_entry_flagged.c */
struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};


s32 Summon_IsEntryFlagged(s32 index)
{
    s32 result;

    if ((u32)index > 171)
        return 0;
    result = 0;
    if ((u32)gRom[index].flags0 << 31)
        result = 1;
    return result;
}

/* battle/get_entry_field2_low_bits.c */

u32 Battle_GetEntryField2LowBits(u32 no)
{
    u8 *tbl;
    u8 *p;
    u32 bits;
    u32 val;

    if (no > 0xABU) {
        return 1U;
    }
    tbl = gRom;
    p = tbl + (no * 8);
    bits = (u32)p[2] << 0x1B;
    val = bits >> 0x1C;
    {
        u32 ret;
        if (val != 0U) {
            ret = val;
        } else {
            ret = 1U;
        }
        return ret;
    }
}

/* battle/get_entry_field2_high_bits.c */

u32 Battle_GetEntryField2HighBits(u32 no)
{
    u32 bits;
    u32 ret;
    u8 *tbl;

    if (no > 0xABU) {
        return 0U;
    }
    tbl = gRom;
    bits = tbl[(no * 8) + 2] >> 5;
    if (bits != 0) {
        ret = bits;
    } else {
        ret = 0;
    }
    return ret;
}

/* battle/summon/is_entry_secondary_flagged.c */
struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};


s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return ((u32)gRom[index].flags1 << 31) >> 31;
}

/* battle/summon/get_entry_byte4.c */

s32 Summon_GetEntryByte4(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return gRom[index * 8 + 4];
}
