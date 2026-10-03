#include "RESMENU.H"
#include "TYPES.H"
#include "SOUND_IDS.H"
#include "RESOURCE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"

extern u8 Menu_SelectionStepDelays[];
extern u8 MsgCommandName;
void RenderOutput_PrepareForRedraw(void *work);
void UiText_DrawCharacterAtOffset(s32 resource_id, void *work, s32 x, s32 y);
void Audio_PlayCue(s32 sound_id);

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
    if (diff >= 0)
        return diff;
    return rhs - lhs;
}

u32 Runtime_BumpAllocate(s32 size);
u32 Resource_DecodeByteLz(const void *, void *);
void VramBlock_LoadCached(s32, s32, void *);
void Runtime_BumpFree(void *);


s32 Menu_SelectResource(s32 start, s32 goal)
{
    s16 resource_base;
    s16 cur;
    s32 diff;
    s32 resource_id;
    s32 dist;
    s32 pos;
    s32 step;
    s32 delay;
    const u8 *tbl;
    struct ResourceMenuWork *state;

    state = gMenuSelectWork;
    step = 1;
    delay = 12;
    state->selection = (s16)start;
    if (goal < start)
        step = -1;
    pos = start;

    for (;;) {
        RenderOutput_PrepareForRedraw(state->window);
        resource_base = state->resource_base;
        if (resource_base != 0) {
            resource_id = resource_base + state->selection;
        } else {
            resource_id = state->resource_ids[state->selection] + (s32)&MsgCommandName;
        }
        UiText_DrawCharacterAtOffset(resource_id, state->window, 0, 0);

        cur = state->selection;
        tbl = Menu_SelectionStepDelays;
        diff = cur - goal;
        dist = AbsoluteDifference(diff, cur, goal);
        WaitFrames(tbl[dist] + delay);

        if (pos == goal)
            break;

        state->selection = (s16)((u16)state->selection + step);
        Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
        delay = 0;
        pos += step;
    }

    WaitFrames(48);
    Audio_PlayCue(SOUND_MENU_CONFIRM);
    return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
    s32 size = 1024;
    void *buffer = (void *)Runtime_BumpAllocate(size);
    u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

    /* 表内の相対位置から転送元を求める。 */
    Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
    VramBlock_LoadCached(slot, size, buffer);
    Runtime_BumpFree(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *entry;
    s16 index;
    s32 slot;
    s32 flags;

    work = gMenuSelectWork;
    index = work->count;
    if (index <= 5) {
        work->count = (u16)work->count + 1;
        entry = &work->entries[index];
        slot = Resource_FindFreeEntry();
        Menu_LoadResourceSlot(slot, no);
        entry->x = index * 24 + 32;
        flags = 136;
        entry->y = flags;
        entry->slot = slot;
        work->resource_ids[index] = no;
    }
}

/* Lays the menu's entries out three tiles apart on the given tile row,
   centred with the window for their text, and opens that window. */
void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *menu = ((struct ResourceMenuWork *)gMenuSelectWork);
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->resource_base = resource_base;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++) {
        struct ResourceMenuEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}
