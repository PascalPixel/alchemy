typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct BattleDecision_080bd424 {
    s16 actor;
    u8 unknown_02[4];
    s16 state;
    u16 action;
    s16 target;
    s16 effect;
};

struct CombatantState_080bd424 {
    u8 unknown_000[0x3A];
    s16 action_threshold;
    u8 unknown_03C[0x9C];
    u16 linked_action;
    u8 unknown_0DA[0x46];
    u32 selection_state;
    u8 unknown_124[4];
    u8 character;
    u8 unavailable;
    u8 unknown_12A[0x13];
    u8 flag_13D;
    u8 unknown_13E[0xA];
    u8 flag_148;
};

struct CharacterActions_080bd424 {
    u8 unknown_00[0x35];
    s8 battle_class;
    s8 selection_mode;
    u8 availability;
    u16 actions[18];
};

struct Action_080bd424 {
    u8 target_mode;
    u8 category;
    u8 unknown_02;
    u8 behavior;
    u8 unknown_04[4];
    u8 effect;
    u8 threshold;
};

struct LinkedAction_080bd424 {
    u8 unknown_00[0xC];
    u8 active;
    u8 unknown_0D[0x1B];
    u16 action;
};

u32 Func_080771a0(void);
struct CombatantState_080bd424 *Func_08077008(s32);
struct LinkedAction_080bd424 *Func_08077018(s32);
struct Action_080bd424 *Func_08077080(s32);
struct CharacterActions_080bd424 *Func_08077198(u8);
s32 Func_080b9a70(u32);
u16 Func_080bad7c(s32);
s32 Func_080bae40(u32, void *);
u32 Func_080bd3c8(s32);
s32 Func_080bd3e4(u8 *);

/*
 * Build one battle decision for a combatant.
 *
 * The character record provides a selection policy, an availability bitset,
 * and a table of action IDs.  In advancing mode the policy can cycle through
 * candidates until it finds a usable action and target; in immediate mode the
 * routine accepts only the two explicitly preselected states.
 */
void Func_080bd424(void *arg0, s32 advance) {
    struct BattleDecision_080bd424 *decision =
        (struct BattleDecision_080bd424 *)arg0;
    struct CombatantState_080bd424 *combatant =
        Func_08077008(decision->actor);
    struct CharacterActions_080bd424 *character;
    s32 attempts = 0;
    s32 allow_linked_action = 1;
    s32 searching = 1;
    s32 selection = -1;

    if (combatant->unavailable != 0) {
        return;
    }
    if (advance != 0 && decision->state != 4) {
        return;
    }

    character = Func_08077198(combatant->character);

    while (searching != 0) {
        struct Action_080bd424 *action;
        u16 action_id;
        s32 available;
        s32 state;
        s32 skip_action = 0;

        switch (character->selection_mode) {
        case 0:
            selection = Func_080bd3e4((u8 *)0x080C2B80);
            break;

        case 1:
            selection = Func_080bd3e4((u8 *)0x080C2B88);
            break;

        case 2:
            selection = Func_080bd3e4((u8 *)0x080C2B90);
            break;

        case 3:
            if ((combatant->selection_state & 1) == 0) {
                u8 random_selection = (Func_080771a0() & 7) << 1;
                u8 old_state = (u8)combatant->selection_state;

                *(u8 *)&combatant->selection_state =
                    (old_state & 0xF1) | random_selection | 1;
            }
            /* Fall through: modes 3 and 4 share the stored selection. */
        case 4:
            selection = (combatant->selection_state >> 1) & 7;
            if (advance != 0) {
                u8 old_state = (u8)combatant->selection_state;
                u8 next_selection = ((selection + 1) & 7) << 1;

                *(u8 *)&combatant->selection_state =
                    (old_state & 0xF1) | next_selection;
            }
            break;

        case 5:
            selection++;
            break;

        case 6:
            break;
        }

        /*
         * Thumb register shifts yield zero for the mode-6 sentinel (-1) and
         * for any index beyond the bitset.  Spell that behavior without an
         * undefined negative or oversized C shift.
         */
        if (selection >= 0 && selection < 32) {
            available =
                ((character->availability >> selection) & 1) &
                allow_linked_action;
        } else {
            available = 0;
        }
        action_id =
            *(u16 *)((u8 *)character + 0x38 + selection * (s32)sizeof(u16));
        decision->state = 4;

        if (available != 0 && advance != 0) {
            if ((combatant->linked_action & 0x1FF) == 0) {
                available = 0;
                if (character->battle_class == 0) {
                    decision->state = 2;
                    decision->action = 0x1FD;
                    return;
                }
            }

            if (available != 0) {
                struct LinkedAction_080bd424 *linked =
                    Func_08077018(combatant->linked_action);

                if (linked->active == 1) {
                    struct Action_080bd424 *linked_action =
                        Func_08077080(linked->action);

                    action_id = linked->action;
                    decision->state = 2;
                    decision->action = 0;
                    if (linked_action->category < 1 ||
                        linked_action->category > 2) {
                        available = 0;
                    }
                } else {
                    available = 0;
                }
            }

            if (available == 0) {
                allow_linked_action = 0;
            }
        }

        action = Func_08077080(action_id);

        switch (action->behavior) {
        case 46:
            decision->state = 3;
            decision->target = Func_080b9a70(decision->actor);
            break;

        case 47:
            decision->state = 7;
            decision->target = Func_080b9a70(decision->actor);
            break;

        case 49:
            decision->state = 99;
            decision->target = Func_080b9a70(decision->actor);
            break;
        }

        if (advance == 0 && decision->state != 3 && decision->state != 7) {
            return;
        }

        state = (u16)decision->state;
        if (state != 2) {
            if (Func_080bd3c8(action_id) != 0) {
                decision->state = 1;
                decision->action = action_id;
                state = 1;

                if (action->threshold > combatant->action_threshold &&
                    character->battle_class != 0) {
                    skip_action = 1;
                } else if (combatant->flag_13D != 0 &&
                           character->battle_class == 2) {
                    skip_action = 1;
                }
            } else {
                state = (u16)decision->state;
            }
        }

        if (skip_action != 0) {
            goto next_candidate;
        }
        if (state == 99 && combatant->flag_148 != 0) {
            goto next_candidate;
        }
        if (advance != 0 && (state == 3 || state == 7)) {
            goto next_candidate;
        }

        if (state == 4) {
            decision->action = action_id;
            if (action_id == 1) {
                decision->state = 0;
            }
        }

        decision->effect = action->effect;
        switch (action->target_mode) {
        case 1:
        case 2:
        case 4: {
            s32 target = Func_080bae40(
                decision->actor,
                action);

            if (target == -2) {
                target = Func_080bad7c((u16)decision->actor <= 7U);
            }
            if (target != -1) {
                decision->target = target;
                searching = 0;
            }
            break;
        }

        case 3:
            decision->target = Func_080b9a70(decision->actor);
            break;

        default:
            decision->target = Func_080b9a70(decision->actor);
            searching = 0;
            break;
        }

        if (advance == 0) {
            searching = 0;
        }

next_candidate:
        if (searching != 0 && attempts > 16) {
            decision->state = 3;
            searching = 0;
        }
        attempts++;
    }
}
