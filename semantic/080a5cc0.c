#include "layout_guard.h"
#include "types.h"

typedef enum SelectionPhase_080a5cc0 {
    SELECTION_OPEN_080a5cc0,
    SELECTION_CHOOSE_ACTION_080a5cc0,
    SELECTION_CONFIRM_ACTION_080a5cc0,
    SELECTION_CHOOSE_TARGET_080a5cc0,
    SELECTION_APPLY_ACTION_080a5cc0
} SelectionPhase_080a5cc0;

typedef struct ItemDefinition_080a5cc0 {
    u8 padding00[9];
    u8 cost;
} ItemDefinition_080a5cc0;

typedef struct SelectionState_080a5cc0 {
    u8 padding000[0x24];
    void *context;
    u8 padding028[4];
    void *panel;
    u8 padding030[0x144];
    s16 selection_scratch;
    u8 padding176[2];
    u16 selected_item;
    u8 padding17a[0x9e];
    u8 action_available;
    u8 padding219;
    u8 current_actor;
    u8 target_actor;
    void *prompt_object;
    u16 flags;
    s16 retry_state;
    u8 padding224[0x36];
    s16 result_message;
    u8 padding25c[0x0c];
    u8 action_category;
} SelectionState_080a5cc0;

LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_Context,
    SelectionState_080a5cc0,
    context,
    0x24);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_Panel,
    SelectionState_080a5cc0,
    panel,
    0x2c);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_SelectionScratch,
    SelectionState_080a5cc0,
    selection_scratch,
    0x174);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_SelectedItem,
    SelectionState_080a5cc0,
    selected_item,
    0x178);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_ActionAvailable,
    SelectionState_080a5cc0,
    action_available,
    0x218);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_CurrentActor,
    SelectionState_080a5cc0,
    current_actor,
    0x21a);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_Flags,
    SelectionState_080a5cc0,
    flags,
    0x220);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_ResultMessage,
    SelectionState_080a5cc0,
    result_message,
    0x25a);
LAYOUT_OFFSET_GUARD(
    SelectionState080a5cc0_ActionCategory,
    SelectionState_080a5cc0,
    action_category,
    0x268);

extern SelectionState_080a5cc0 *Data_03001f2c;

void Func_080030f8(s32);
void Func_08015270(void *);
void Func_08015278(void *);
void Func_08077008(u8);
ItemDefinition_080a5cc0 *Func_08077080(u16);
s32 Func_080770c0(s32);
void Func_08077010(u8);
void Func_08077120(u8, s32);
void Func_080a112c(void *, u8, s32, s32);
void Func_080a1d08(s32, s32, s32);
void Func_080a3cf8(s32, s32);
s32 Func_080a5fe0(void);
s32 Func_080a602c(s32);
s32 Func_080a63e4(s32);
void Func_080a65e4(u8, s32, s32);
s32 Func_080a6ccc(s32);
void Func_080a9cbc(void);
s32 Func_080a9f10(u16, u8, u8, s32);
void Func_080aa460(u16);
void Func_080f9010(s32);

/* Run the multi-stage item/effect selection and confirmation workflow. */
s32 Func_080a5cc0(
    s32 *selected_actor,
    s32 *unused_output,
    s32 *selected_item)
{
    SelectionState_080a5cc0 *state = Data_03001f2c;
    SelectionPhase_080a5cc0 phase = SELECTION_OPEN_080a5cc0;
    s32 result = 0;
    s32 finished = 0;

    (void)unused_output;
    while (finished == 0 && Func_080770c0(0x150) == 0) {
        switch (phase) {
        case SELECTION_OPEN_080a5cc0:
            state->selection_scratch = 0;
            Func_080a3cf8(0, 0x0ae9);
            if (Func_080a602c(0) == -1) {
                finished = 1;
                result = -1;
            }
            Func_08015270(state->panel);
            phase = SELECTION_CHOOSE_ACTION_080a5cc0;
            break;

        case SELECTION_CHOOSE_ACTION_080a5cc0: {
            s32 choice;

            Func_080030f8(1);
            Func_08077008(state->current_actor);
            phase = SELECTION_OPEN_080a5cc0;
            if (state->action_available == 0)
                break;

            switch (state->action_category) {
            case 0:
                Func_080a3cf8(0, 0x0aea);
                break;
            case 1:
                Func_080a3cf8(0, 0x0af1);
                break;
            case 2:
                Func_080a3cf8(0, 0x0af0);
                break;
            }

            Func_080a9cbc();
            Func_080a112c(
                state->context, state->current_actor, 0, 0);
            choice = Func_080a6ccc(0);
            if (choice == -1)
                break;

            phase = SELECTION_CONFIRM_ACTION_080a5cc0;
            if (state->action_category != 0) {
                s32 alternate = state->action_category != 1;

                Func_080a65e4(state->current_actor, choice, alternate);
                Func_08015278(state->panel);
                Func_080a1d08(alternate ? 0x0ae3 : 0x0ae2, -1, -1);
                phase = SELECTION_OPEN_080a5cc0;
            }
            break;
        }

        case SELECTION_CONFIRM_ACTION_080a5cc0: {
            s32 confirmation = Func_080a5fe0();

            if (confirmation == 1) {
                phase = SELECTION_CHOOSE_TARGET_080a5cc0;
            } else if (confirmation == 2) {
                state->target_actor = 9;
                phase = SELECTION_APPLY_ACTION_080a5cc0;
            } else {
                finished = 1;
                result = 1;
                *selected_actor = state->current_actor;
                *selected_item = state->selected_item & 0x3fff;
            }
            break;
        }

        case SELECTION_CHOOSE_TARGET_080a5cc0:
            Func_080a3cf8(0, 0x0aeb);
            phase = SELECTION_APPLY_ACTION_080a5cc0;
            if (Func_080a63e4(0) == -1) {
                state->flags |= 1;
                phase = SELECTION_CHOOSE_ACTION_080a5cc0;
            }
            break;

        case SELECTION_APPLY_ACTION_080a5cc0: {
            u16 item_id = state->selected_item & 0x3fff;
            s32 used_self_target = 0;

            result = Func_080a9f10(
                state->selected_item,
                state->current_actor,
                state->target_actor,
                0);
            if (state->target_actor == 9) {
                state->target_actor = state->current_actor;
                used_self_target = 1;
            }

            if (result != -1) {
                ItemDefinition_080a5cc0 *item =
                    Func_08077080(item_id);

                Func_08077120(state->current_actor, -(s32)item->cost);
            }
            Func_08077010(state->current_actor);

            if (result != -1) {
                Func_080a112c(
                    state->context, state->target_actor, 0, 0);
                Func_080aa460(item_id);
                Func_08015278(state->panel);
                Func_080a1d08(
                    state->result_message + 0x0bef, 0, -1);
                result = 1;
                state->flags |= 1;
                phase = SELECTION_CHOOSE_ACTION_080a5cc0;
            } else {
                Func_080f9010(0x72);
                Func_08015278(state->panel);
                Func_080a1d08(
                    state->result_message + 0x0bef, -1, -1);
                state->retry_state = 1;
                if (used_self_target != 0) {
                    state->flags |= 1;
                    phase = SELECTION_CHOOSE_ACTION_080a5cc0;
                } else {
                    phase = SELECTION_CHOOSE_TARGET_080a5cc0;
                }
            }
            break;
        }

        default:
            finished = 1;
            break;
        }
    }

    if (Func_080770c0(0x150) != 0)
        result = -1;
    return result;
}
