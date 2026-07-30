typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

u32 Func_08004458(void);
u8 *Func_08077008(s32);
u8 *Func_08077198(u8);
s32 Func_080772b8(s32);

/*
 * Select an encoded battle-roster entry for an action.
 *
 * The two source rosters hold at most six combatants.  Each accepted ID is
 * paired with a token that records both its source roster and original slot:
 * 0x100 | slot for the first roster and 0x180 | slot for the second.
 */
s32 Func_080bae40(u32 actor, void *arg1) {
    u8 *request = (u8 *)arg1;
    u8 *battle = *(u8 **)0x03001E74;
    s32 candidate_ids[6];
    u16 candidate_tokens[6];
    s32 selected_tokens[6];
    s32 candidate_count = 0;
    s32 selected_count = 0;
    s32 source_index;
    s32 index;

    if (request[0] != 0) {
        s32 special_roster = request[0] == 2 || request[0] == 4;
        s32 use_first_roster;
        s16 *roster;
        u16 token_base;

        if (actor <= 7U) {
            use_first_roster = special_roster;
        } else {
            use_first_roster = !special_roster;
        }

        if (use_first_roster) {
            roster = (s16 *)(battle + 0x58);
            token_base = 0x100;
        } else {
            roster = (s16 *)(battle + 0x66);
            token_base = 0x180;
        }

        for (source_index = 0; roster[source_index] != 0xFF; source_index++) {
            s16 candidate = roster[source_index];

            if (candidate == 0xFE) {
                continue;
            }
            if (request[0] == 4 && candidate != (s32)actor) {
                continue;
            }

            candidate_ids[candidate_count] = candidate;
            candidate_tokens[candidate_count] = token_base | source_index;
            candidate_count++;
        }
    }

    if (candidate_count == 0) {
        return -2;
    }

    for (index = 0; index < candidate_count; index++) {
        u8 *state = Func_08077008(candidate_ids[index]);
        s32 mode = request[3];
        s32 matches = 0;

        switch (mode) {
        case 0:
        case 1:
        case 2:
            break;

        case 3:
            matches = FIELD(state, s8, 0x131) != 0;
            break;

        case 4:
            if (FIELD(state, u8, 0x138) != 0) matches++;
            if (FIELD(state, u8, 0x139) != 0) matches++;
            if (FIELD(state, u8, 0x13A) != 0) matches++;
            if (FIELD(state, u8, 0x13C) != 0) matches++;
            if (FIELD(state, u8, 0x13D) != 0) matches++;
            if (FIELD(state, u8, 0x141) != 0) matches++;
            break;

        case 5:
        case 0x38:
        case 0x39:
            if (FIELD(state, s16, 0x38) == 0) {
                matches = 100;
            }
            break;

        case 6:
        case 7:
            if (FIELD(state, s8, 0x133) <= 3) matches = 1;
            if (FIELD(state, u8, 0x132) == 1) matches++;
            break;

        case 8:
        case 9:
            if (FIELD(state, s8, 0x133) >= -3) matches = 1;
            if (FIELD(state, u8, 0x132) == 1) matches++;
            break;

        case 10:
        case 11:
            if (FIELD(state, s8, 0x135) <= 3) matches = 1;
            if (FIELD(state, u8, 0x134) == 1) matches++;
            break;

        case 12:
        case 13:
            if (FIELD(state, s8, 0x135) >= -3) matches = 1;
            if (FIELD(state, u8, 0x134) == 1) matches++;
            break;

        case 14:
        case 15:
            if (FIELD(state, s8, 0x137) <= 3) matches = 1;
            if (FIELD(state, u8, 0x136) == 1) matches++;
            break;

        case 16:
        case 17:
            if (FIELD(state, s8, 0x137) >= -3) matches = 1;
            if (FIELD(state, u8, 0x136) == 1) matches++;
            break;

        case 18:
            matches = FIELD(state, s8, 0x131) == 0;
            break;

        case 19:
            matches = FIELD(state, s8, 0x131) <= 1;
            break;

        case 23:
            matches = FIELD(state, u8, 0x13B) == 0;
            break;

        case 24:
            matches = FIELD(state, u8, 0x13C) == 0;
            break;

        case 26:
            matches = FIELD(state, u8, 0x140) == 0;
            break;

        case 28:
            matches = FIELD(state, u8, 0x141) == 0;
            break;

        case 0x21:
            if (FIELD(state, s8, 0x133) > 0) matches++;
            if (FIELD(state, s8, 0x135) > 0) matches++;
            if (FIELD(state, s8, 0x137) > 0) matches++;
            if (FIELD(state, s8, 0x12C) > 0) matches++;
            if (FIELD(state, s8, 0x12D) > 0) matches++;
            if (FIELD(state, s8, 0x12E) > 0) matches++;
            if (FIELD(state, s8, 0x12F) > 0) matches++;
            break;

        case 0x3D:
        case 0x3E:
            matches =
                FIELD(state, s16, 0x38) < FIELD(state, s16, 0x34);
            break;

        case 0x40:
            if (FIELD(state, u8, 0x138) != 0) matches++;
            if (FIELD(state, u8, 0x139) != 0) matches++;
            if (FIELD(state, u8, 0x13A) != 0) matches++;
            if (FIELD(state, u8, 0x13C) != 0) matches++;
            if (FIELD(state, u8, 0x13D) != 0) matches++;
            if (FIELD(state, u8, 0x141) != 0) matches++;
            if (FIELD(state, u8, 0x140) != 0) matches++;
            if (FIELD(state, s8, 0x131) != 0) matches++;
            break;

        default:
            matches = 1;
            break;
        }

        /*
         * Most modes cannot select an unavailable combatant.  Modes for which
         * Func_080772b8 reports a special dead-target rule retain their match.
         */
        if (FIELD(state, u16, 0x38) == 0 && Func_080772b8(mode) == 0) {
            matches = 0;
        }

        if (matches == 0) {
            switch ((request[1] & 0xF) - 1) {
            case 0:
                if (FIELD(state, s16, 0x38) != 0 &&
                    FIELD(state, s16, 0x38) < FIELD(state, s16, 0x34)) {
                    matches = 1;
                }
                break;

            case 1:
            case 4:
            case 5:
            case 7:
            case 8:
                if (FIELD(request, u16, 0xA) != 0 &&
                    FIELD(state, s16, 0x38) != 0) {
                    matches = 1;
                }
                break;

            case 2:
            case 3:
                if (FIELD(state, s16, 0x38) != 0) {
                    matches = 1;
                }
                break;

            case 9:
                if (FIELD(state, s16, 0x3A) != 0) {
                    matches = 1;
                }
                break;
            }
        }

        if (matches != 0) {
            candidate_ids[selected_count] = candidate_ids[index];
            selected_tokens[selected_count] = candidate_tokens[index];
            selected_count++;
        }
    }

    if (selected_count == 0) {
        return -1;
    }

    /*
     * A narrow group of actions ranks candidates by their live battle stat.
     * The encoded roster tokens must move in parallel with the combatant IDs.
     */
    if (request[0] == 1 && request[8] == 1) {
        u8 *actor_state = Func_08077008((s32)actor);
        s8 actor_class =
            FIELD(Func_08077198(FIELD(actor_state, u8, 0x128)), s8, 0x35);
        s32 action_group = request[1] & 0xF;

        if (actor_class != 2 && action_group >= 3 && action_group <= 5) {
            s32 outer;

            for (outer = 0; outer < selected_count; outer++) {
                s32 current;

                for (current = outer; current < selected_count - 1; current++) {
                    u8 *first = Func_08077008(candidate_ids[current]);
                    u8 *second = Func_08077008(candidate_ids[current + 1]);
                    u8 *current_actor = Func_08077008((s32)actor);
                    s8 current_class = FIELD(
                        Func_08077198(FIELD(current_actor, u8, 0x128)),
                        s8,
                        0x35);
                    s16 first_value;
                    s16 second_value;

                    if (current_class == 0) {
                        first_value = FIELD(first, s16, 0x38);
                        second_value = FIELD(second, s16, 0x38);
                    } else {
                        first_value = FIELD(first, s16, 0x34);
                        second_value = FIELD(second, s16, 0x34);
                    }

                    if (first_value < second_value) {
                        s32 id = candidate_ids[current];
                        s32 token = selected_tokens[current];

                        candidate_ids[current] = candidate_ids[current + 1];
                        candidate_ids[current + 1] = id;
                        selected_tokens[current] = selected_tokens[current + 1];
                        selected_tokens[current + 1] = token;
                    }
                }
            }

            switch (selected_count) {
            case 1:
                return selected_tokens[0];

            case 2:
                if ((11U * Func_08004458() >> 16) <= 5U) {
                    return selected_tokens[0];
                }
                return selected_tokens[1];

            case 3: {
                u32 roll = 15U * Func_08004458() >> 16;

                if (roll <= 5U) return selected_tokens[0];
                if (roll <= 10U) return selected_tokens[1];
                return selected_tokens[2];
            }

            case 4: {
                u32 roll = 18U * Func_08004458() >> 16;

                if (roll <= 5U) return selected_tokens[0];
                if (roll <= 10U) return selected_tokens[1];
                if (roll <= 14U) return selected_tokens[2];
                return selected_tokens[3];
            }
            }
        }
    }

    index = (u32)(selected_count * Func_08004458()) >> 16;
    return selected_tokens[index];
}
