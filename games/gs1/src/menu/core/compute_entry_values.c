#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* menu/core/compute_entry_values.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_ComputeEntryValues(void *tbl)
{
    void *state;
    s32 i;
    void *p;
    u16 *src;
    s8 *dst;
    s16 v;
    s32 cnt;

    state = *(void **)ADDR_03001F2C;
    i = 0;
    if (i < FIELD_AT_OFFSET(state, u8, 0x219)) {
        dst = (s8 *)tbl + 0xA0;
        src = (u16 *)((u8 *)state + 0x208);
        p = tbl;
        do {
            v = Menu_Place(p, *src, -1);
            cnt = FIELD_AT_OFFSET(state, u8, 0x219);
            i += 1;
            *dst = v;
            src += 1;
            dst += 1;
            p = (u8 *)p + 0x14;
        } while (i < cnt);
    }
}

/* menu/draw/draw_at_window_offset.c */
s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    Menu_SetRange(FIELD_AT_OFFSET(win, u16 *, 0xC) + arg1 + 1, FIELD_AT_OFFSET(win, u16 *, 0xE) + arg2 + 1, arg3, arg4, arg5);
}

/* ui/window/apply_rect_at_object_origin.c */
void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
  int ofs;
  ofs = 0xC;
  Ui_SetRange(((*((u16 *)(((u8 *)obj) + ofs))) + x) + 1, ((*((u16 *)(((u8 *)obj) + 0xE))) + y) + 1, width, height, palette);
}

/* menu/sel/run_paired_entry_action.c */
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
