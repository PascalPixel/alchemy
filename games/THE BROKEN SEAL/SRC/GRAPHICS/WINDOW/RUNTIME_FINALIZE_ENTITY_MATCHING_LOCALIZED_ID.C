#include "TYPES.H"
#include "WINDOW.H"
#include "RENDER_INPUT.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern s32 Localization_LookupEntryId();

void UiWork_FinalizeEntityMatchingLocalizedId(void)
{
    struct UiRenderWork *state;
    struct UiWindow *work;
    s32 id;
    s32 index;
    s32 i;
    struct RenderOutput *entity;

    state = (struct UiRenderWork *)gWindowWork[0];
    work = state->windows;
    id = Localization_LookupEntryId();
    if (id == -1)
        return;

    if (state->resource_ids[1] == id) {
        index = 1;
    } else if (state->resource_ids[0] == id) {
        index = 0;
    } else {
        return;
    }

    id = state->output_ids[index];

    for (i = 0; i != WINDOW_COUNT; i++, work++) {
        entity = (struct RenderOutput *)(u32)work->unknown_00;
        if ((u8)entity->kind == 2 && (u8)entity->index == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}
