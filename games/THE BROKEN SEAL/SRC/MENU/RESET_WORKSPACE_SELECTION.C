#include "TYPES.H"

struct MenuWorkspaceEntry {
    u8 unknown_00[10];
    u16 state;
    u8 unknown_0c[40];
};

struct MenuWorkspace {
    u8 unknown_000[0x400];
    struct MenuWorkspaceEntry entries[7];
    u8 unknown_56c[8];
    u16 selection[4];
    u16 cursor;
};

/* Clears the state of the seven workspace entries, then the four selected
   entries and the cursor that RUN_WORKSPACE_SELECTION_LOOP.C reads. */
void Menu_ResetWorkspaceSelection(struct MenuWorkspace *work)
{
    struct MenuWorkspaceEntry *entry = work->entries;
    u16 zero = 0;

    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    entry++;
    entry->state = zero;
    work->selection[0] = zero;
    work->selection[1] = zero;
    work->selection[2] = zero;
    work->selection[3] = zero;
    work->cursor = zero;
}
