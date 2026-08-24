#include "types.h"
#include "sound_ids.h"

struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};

extern struct MenuSelectionState *Data_03001f38;
extern u8 Data_080373ef[];
extern u8 Data_0000001f;

void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacter(s32 resource_id, void *work, s32 arg2, s32 arg3);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 sound_id);

static inline s32 AbsoluteDifference(s32 difference, s32 lhs, s32 rhs)
{
    if (difference >= 0)
        return difference;
    return rhs - lhs;
}

s32 Menu_SelectResource(s32 initial_selection, s32 target_selection)
{
    s16 resource_base;
    s16 current_selection;
    s32 difference;
    s32 resource_id;
    s32 distance;
    s32 current_selection_index;
    s32 selection_step;
    s32 frame_delay;
    const u8 *selection_delay_table;
    struct MenuSelectionState *state;

    state = Data_03001f38;
    selection_step = 1;
    frame_delay = 12;
    state->selection = (s16)initial_selection;
    if (target_selection < initial_selection)
        selection_step = -1;
    current_selection_index = initial_selection;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->work);
        resource_base = state->resource_base;
        if (resource_base != 0) {
            resource_id = resource_base + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)&Data_0000001f;
        }
        UiText_DrawCharacter(resource_id, state->work, 0, 0);

        current_selection = state->selection;
        selection_delay_table = Data_080373ef;
        difference = current_selection - target_selection;
        distance = AbsoluteDifference(difference, current_selection, target_selection);
        WaitFrames(selection_delay_table[distance] + frame_delay);

        if (current_selection_index == target_selection)
            break;

        state->selection = (s16)((u16)state->selection + selection_step);
        Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
        frame_delay = 0;
        current_selection_index += selection_step;
    }

    WaitFrames(48);
    Audio_PlayCue(SOUND_MENU_CONFIRM);
    return target_selection;
}
