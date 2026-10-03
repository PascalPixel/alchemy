#include "TYPES.H"
#include "WINDOW.H"
#include "RENDER_INPUT.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern s32 Localization_LookupEntryId();

void UiWork_FinalizeEntityMatchingLocalizedId(void)
{
    u8 *base;
    struct UiWindow *work;
    s32 id;
    s32 index;
    s32 i;
    struct RenderOutput *entity;
    s32 offset;

    base = gWindowWork[0];
    work = (struct UiWindow *)(base + 0x500);
    id = Localization_LookupEntryId();
    if (id == -1)
        return;

    if (*(u16 *)(base + RENDER_SLOT_ID_OFS + 2) == id) {
        index = 1;
    } else if (*(u16 *)(base + RENDER_SLOT_ID_OFS) == id) {
        index = 0;
    } else {
        return;
    }

    offset = RENDER_SLOT_VALUE_OFS + index * 2;
    id = *(u16 *)(base + offset);

    for (i = 0; i != WINDOW_COUNT; i++, work++) {
        entity = (struct RenderOutput *)(u32)work->unknown_00;
        if ((u8)entity->kind == 2 && (u8)entity->index == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}
