#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s32 Localization_LookupEntryId();
extern s32 UiWindow_Create();
extern s32 CreateSideObject();

s32 Func_08019da8(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 object;
    s32 minus_four;
    s32 offset;
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    offset = minus_four;
    if (M2C_FIELD(work, u8, RENDER_MODE_OFS) != 0) {
        object = UiWindow_Create(x, y, 6, 5, 2);
        offset = 0;
    } else {
        object = UiWindow_Create(x, y, 5, 5, 2);
    }
    if (object != 0) {
        CreateSideObject(arg0, arg1, -1, object, offset, minus_four);
    }
    return object;
}
