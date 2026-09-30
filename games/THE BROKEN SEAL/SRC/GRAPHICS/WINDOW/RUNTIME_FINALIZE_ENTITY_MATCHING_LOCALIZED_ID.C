#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

extern s32 Localization_LookupEntryId();
extern void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeEntityMatchingLocalizedId(void)
{
    u8 *base;
    u8 *work;
    s32 id;
    s32 index;
    s32 i;
    u8 *entity;
    s32 offset;

    base = *(u8 **)((u32)&Data_03001e8c);
    work = base + 0x500;
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

    for (i = 0; i != WINDOW_COUNT; i++, work += 36) {
        entity = *(u8 **)work;
        if (*(u8 *)(entity + 4) == 2 && *(u8 *)(entity + 14) == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}
