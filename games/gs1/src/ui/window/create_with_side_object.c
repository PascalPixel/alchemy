#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWindow_CreateWithSideObject Func_08019da8

#define FIELD_AT_OFFSET(base, type, ofs)     (*(type *)((u8 *)(base) + (ofs)))

extern s32 Localization_LookupEntryId();
extern s32 UiWindow_Create();
extern s32 CreateSideObject();

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 win;
    s32 minus_four;
    s32 ofs;
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    ofs = minus_four;
    if (FIELD_AT_OFFSET(work, u8, RENDER_MODE_OFS) != 0) {
        win = UiWindow_Create(x, y, 6, 5, 2);
        ofs = 0;
    } else {
        win = UiWindow_Create(x, y, 5, 5, 2);
    }
    if (win != 0) {
        CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
    }
    return win;
}
