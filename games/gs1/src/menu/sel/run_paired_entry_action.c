#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_paired_entry_action.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

#if defined(GS1_EDITION_JA)
#define ALT_PARAM 4
#else
#define ALT_PARAM 2
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
    s32 sp14;
    void *state;

    state = *(void **)ADDR_03001F2C;
    if (mode == 0) {
        sp14 = mode;
        Menu_SetRect(FIELD_AT_OFFSET(state, s32 *, 0x34), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x259), 1, mode, 2, param, 1);
        Menu_SetRect(FIELD_AT_OFFSET(state, s32 *, 0x24), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x258), mode, 1, 2, param, mode);
    } else {
        Menu_SetRect(FIELD_AT_OFFSET(state, s32 *, 0x34), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x21B), 1, 0, ALT_PARAM, param, 1);
        Menu_SetRect(FIELD_AT_OFFSET(state, s32 *, 0x24), 0, 0, FIELD_AT_OFFSET(state, u8 *, 0x21A), 0, 0, 1, param, 0);
    }
    return 1;
}
