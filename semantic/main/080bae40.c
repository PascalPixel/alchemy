#include "layout_guard.h"
#include "types.h"

struct TargetRequest_080bae40 {
    u8 roster_mode;
    u8 action_flags;
    u8 padding02;
    u8 filter_mode;
    u8 padding04[4];
    u8 ranked_selection;
    u8 padding09;
    u16 fallback_enabled;
};

struct BattleRoster_080bae40 {
    u8 padding00[0x58];
    s16 first[7];
    s16 second[7];
};

struct CombatantState_080bae40 {
    u8 padding000[0x34];
    s16 maximum_value;
    u8 padding036[2];
    s16 current_value;
    s16 reserve_value;
    u8 padding03c[0xec];
    u8 class_id;
    u8 padding129[3];
    s8 modifiers[4];
    u8 padding130;
    s8 condition;
    u8 stage_data[6];
    u8 status138;
    u8 status139;
    u8 status13a;
    u8 status13b;
    u8 status13c;
    u8 status13d;
    u8 padding13e[2];
    u8 status140;
    u8 status141;
};

struct ClassDefinition_080bae40 {
    u8 padding00[0x35];
    s8 selection_class;
};

LAYOUT_OFFSET_GUARD(
    TargetRequest080bae40_FilterMode,
    struct TargetRequest_080bae40,
    filter_mode,
    0x03);
LAYOUT_OFFSET_GUARD(
    TargetRequest080bae40_RankedSelection,
    struct TargetRequest_080bae40,
    ranked_selection,
    0x08);
LAYOUT_OFFSET_GUARD(
    TargetRequest080bae40_FallbackEnabled,
    struct TargetRequest_080bae40,
    fallback_enabled,
    0x0a);
LAYOUT_OFFSET_GUARD(
    BattleRoster080bae40_First,
    struct BattleRoster_080bae40,
    first,
    0x58);
LAYOUT_OFFSET_GUARD(
    BattleRoster080bae40_Second,
    struct BattleRoster_080bae40,
    second,
    0x66);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_MaximumValue,
    struct CombatantState_080bae40,
    maximum_value,
    0x34);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_CurrentValue,
    struct CombatantState_080bae40,
    current_value,
    0x38);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_ClassId,
    struct CombatantState_080bae40,
    class_id,
    0x128);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_Modifiers,
    struct CombatantState_080bae40,
    modifiers,
    0x12c);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_Condition,
    struct CombatantState_080bae40,
    condition,
    0x131);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_Stages,
    struct CombatantState_080bae40,
    stage_data,
    0x132);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_Status138,
    struct CombatantState_080bae40,
    status138,
    0x138);
LAYOUT_OFFSET_GUARD(
    CombatantState080bae40_Status140,
    struct CombatantState_080bae40,
    status140,
    0x140);
LAYOUT_OFFSET_GUARD(
    ClassDefinition080bae40_SelectionClass,
    struct ClassDefinition_080bae40,
    selection_class,
    0x35);

extern struct BattleRoster_080bae40 *Data_03001e74;

u32 Func_08004458(void);
struct CombatantState_080bae40 *Func_08077008(s32 actor);
struct ClassDefinition_080bae40 *Func_08077198(u8 class_id);
s32 Func_080772b8(s32 filter_mode);

s32 LoadS32_080bae40(const s32 *values, s32 index) {
    return values[index];
}

void StoreS32_080bae40(s32 *values, s32 index, s32 value) {
    values[index] = value;
}

u16 LoadU16_080bae40(const u16 *values, s32 index) {
    return values[index];
}

void StoreU16_080bae40(u16 *values, s32 index, u16 value) {
    values[index] = value;
}

static u8 StageMarker_080bae40(
    const struct CombatantState_080bae40 *state,
    s32 index
) {
    return state->stage_data[index * 2];
}

static s8 StageValue_080bae40(
    const struct CombatantState_080bae40 *state,
    s32 index
) {
    return (s8)state->stage_data[index * 2 + 1];
}

static s32 CountPrimaryStatuses_080bae40(
    const struct CombatantState_080bae40 *state
) {
    s32 count = 0;

    if (state->status138 != 0) count++;
    if (state->status139 != 0) count++;
    if (state->status13a != 0) count++;
    if (state->status13c != 0) count++;
    if (state->status13d != 0) count++;
    if (state->status141 != 0) count++;
    return count;
}

static s32 FilterMatches_080bae40(
    const struct CombatantState_080bae40 *state,
    s32 mode
) {
    s32 matches = 0;

    switch (mode) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        matches = state->condition != 0;
        break;
    case 4:
        matches = CountPrimaryStatuses_080bae40(state);
        break;
    case 5:
    case 0x38:
    case 0x39:
        if (state->current_value == 0) matches = 100;
        break;
    case 6:
    case 7:
        if (StageValue_080bae40(state, 0) <= 3) matches = 1;
        if (StageMarker_080bae40(state, 0) == 1) matches++;
        break;
    case 8:
    case 9:
        if (StageValue_080bae40(state, 0) >= -3) matches = 1;
        if (StageMarker_080bae40(state, 0) == 1) matches++;
        break;
    case 10:
    case 11:
        if (StageValue_080bae40(state, 1) <= 3) matches = 1;
        if (StageMarker_080bae40(state, 1) == 1) matches++;
        break;
    case 12:
    case 13:
        if (StageValue_080bae40(state, 1) >= -3) matches = 1;
        if (StageMarker_080bae40(state, 1) == 1) matches++;
        break;
    case 14:
    case 15:
        if (StageValue_080bae40(state, 2) <= 3) matches = 1;
        if (StageMarker_080bae40(state, 2) == 1) matches++;
        break;
    case 16:
    case 17:
        if (StageValue_080bae40(state, 2) >= -3) matches = 1;
        if (StageMarker_080bae40(state, 2) == 1) matches++;
        break;
    case 18:
        matches = state->condition == 0;
        break;
    case 19:
        matches = state->condition <= 1;
        break;
    case 23:
        matches = state->status13b == 0;
        break;
    case 24:
        matches = state->status13c == 0;
        break;
    case 26:
        matches = state->status140 == 0;
        break;
    case 28:
        matches = state->status141 == 0;
        break;
    case 0x21: {
        s32 index;
        for (index = 0; index < 3; index++) {
            if (StageValue_080bae40(state, index) > 0) matches++;
        }
        for (index = 0; index < 4; index++) {
            if (state->modifiers[index] > 0) matches++;
        }
        break;
    }
    case 0x3d:
    case 0x3e:
        matches = state->current_value < state->maximum_value;
        break;
    case 0x40:
        matches = CountPrimaryStatuses_080bae40(state);
        if (state->status140 != 0) matches++;
        if (state->condition != 0) matches++;
        break;
    default:
        matches = 1;
        break;
    }

    if (state->current_value == 0 && Func_080772b8(mode) == 0) {
        matches = 0;
    }
    return matches;
}

static s32 FallbackMatches_080bae40(
    const struct CombatantState_080bae40 *state,
    const struct TargetRequest_080bae40 *request
) {
    switch ((request->action_flags & 0x0f) - 1) {
    case 0:
        return state->current_value != 0 &&
            state->current_value < state->maximum_value;
    case 1:
    case 4:
    case 5:
    case 7:
    case 8:
        return request->fallback_enabled != 0 && state->current_value != 0;
    case 2:
    case 3:
        return state->current_value != 0;
    case 9:
        return state->reserve_value != 0;
    default:
        return 0;
    }
}

static s32 RankedPairOutOfOrder_080bae40(
    s32 first,
    s32 second,
    u32 actor
) {
    struct CombatantState_080bae40 *first_state =
        Func_08077008(first);
    struct CombatantState_080bae40 *second_state =
        Func_08077008(second);
    struct CombatantState_080bae40 *actor_state =
        Func_08077008((s32)actor);
    s8 selection_class =
        Func_08077198(actor_state->class_id)->selection_class;

    if (selection_class == 0) {
        return first_state->current_value < second_state->current_value;
    }
    return first_state->maximum_value < second_state->maximum_value;
}

static void SortRankedCandidates_080bae40(
    s32 ids[6],
    s32 tokens[6],
    s32 count,
    u32 actor
) {
    s32 outer;

    for (outer = 0; outer < count; outer++) {
        s32 current;

        for (current = outer; current < count - 1; current++) {
            if (RankedPairOutOfOrder_080bae40(
                    ids[current], ids[current + 1], actor)) {
                s32 id = ids[current];
                s32 token = tokens[current];

                ids[current] = ids[current + 1];
                ids[current + 1] = id;
                tokens[current] = tokens[current + 1];
                tokens[current + 1] = token;
            }
        }
    }
}

static s32 ChooseRankedToken_080bae40(
    const s32 tokens[6],
    s32 count
) {
    u32 roll;

    switch (count) {
    case 1:
        return tokens[0];
    case 2:
        roll = 11U * Func_08004458() >> 16;
        return roll <= 5 ? tokens[0] : tokens[1];
    case 3:
        roll = 15U * Func_08004458() >> 16;
        if (roll <= 5) return tokens[0];
        if (roll <= 10) return tokens[1];
        return tokens[2];
    case 4:
        roll = 18U * Func_08004458() >> 16;
        if (roll <= 5) return tokens[0];
        if (roll <= 10) return tokens[1];
        if (roll <= 14) return tokens[2];
        return tokens[3];
    default:
        return -1;
    }
}

/* Select an encoded battle-roster entry for an action. */
s32 Func_080bae40(u32 actor, const struct TargetRequest_080bae40 *request) {
    struct BattleRoster_080bae40 *battle = Data_03001e74;
    s32 candidate_ids[6];
    u16 candidate_tokens[6];
    s32 selected_tokens[6];
    s32 candidate_count = 0;
    s32 selected_count = 0;
    s32 index;

    if (request->roster_mode != 0) {
        s32 special_roster =
            request->roster_mode == 2 || request->roster_mode == 4;
        s32 use_first_roster = actor <= 7 ? special_roster : !special_roster;
        const s16 *roster =
            use_first_roster ? battle->first : battle->second;
        u16 token_base = use_first_roster ? 0x100 : 0x180;

        for (index = 0; roster[index] != 0xff; index++) {
            s16 candidate = roster[index];

            if (candidate == 0xfe) continue;
            if (request->roster_mode == 4 && candidate != (s32)actor) continue;
            StoreS32_080bae40(candidate_ids, candidate_count, candidate);
            StoreU16_080bae40(
                candidate_tokens,
                candidate_count,
                (u16)(token_base | index));
            candidate_count++;
        }
    }

    if (candidate_count == 0) {
        return -2;
    }

    for (index = 0; index < candidate_count; index++) {
        struct CombatantState_080bae40 *state =
            Func_08077008(LoadS32_080bae40(candidate_ids, index));
        s32 matches = FilterMatches_080bae40(state, request->filter_mode);

        if (matches == 0) {
            matches = FallbackMatches_080bae40(state, request);
        }
        if (matches != 0) {
            StoreS32_080bae40(
                candidate_ids,
                selected_count,
                LoadS32_080bae40(candidate_ids, index));
            StoreS32_080bae40(
                selected_tokens,
                selected_count,
                LoadU16_080bae40(candidate_tokens, index));
            selected_count++;
        }
    }

    if (selected_count == 0) {
        return -1;
    }

    if (request->roster_mode == 1 && request->ranked_selection == 1) {
        struct CombatantState_080bae40 *actor_state =
            Func_08077008((s32)actor);
        s8 selection_class =
            Func_08077198(actor_state->class_id)->selection_class;
        s32 action_group = request->action_flags & 0x0f;

        if (selection_class != 2 && action_group >= 3 && action_group <= 5) {
            s32 ranked_token;

            SortRankedCandidates_080bae40(
                candidate_ids, selected_tokens, selected_count, actor);
            ranked_token = ChooseRankedToken_080bae40(
                selected_tokens, selected_count);
            if (ranked_token >= 0) {
                return ranked_token;
            }
        }
    }

    index = (u32)(selected_count * Func_08004458()) >> 16;
    switch (index) {
    case 0: return selected_tokens[0];
    case 1: return selected_tokens[1];
    case 2: return selected_tokens[2];
    case 3: return selected_tokens[3];
    case 4: return selected_tokens[4];
    default: return selected_tokens[5];
    }
}
