#include "types.h"
#include "gs1_edition.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Work;
struct Slot;

struct UiTextMessageWorkGlobals {
    void *state;
    u8 padding4[0x54];
    void *control;
};

extern volatile struct UiTextMessageWorkGlobals Data_03001e8c;

s32 UiText_BuildRenderEntries(s32, s32);
struct Work *UiWindow_Create(s32, s32, s32, s32, s32);
void Func_08017248(s32, s32, s32, s32, s32);
struct Slot *Func_08016670(struct Work *, s32, s32);
void UiWork_Finalize(struct Work *, s32);

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    s32 active_offset;
    struct Work *existing;
    struct Work *work;
    void *state;
    void *control;

    state = Data_03001e8c.state;
    control = Data_03001e8c.control;
    result = 0;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    one = 1;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = one;
    active_offset = RENDER_ENTRY_TBL_OFS + index * 2;

    if (FIELD(state, u16, active_offset) != 0) {
        existing = FIELD(control, struct Work *, 0);
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            existing = work;
            FIELD(control, struct Work *, 0) = existing;
            Func_08017248(0, 15, 30, 6, one);
            FIELD(control, s32, 8) = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)Func_08016670(work, index, FIELD(control, s32, 8));
            FIELD(control, s32, 4) = result;
            FIELD(control, s32, 8) = 0;
            if (result == 0) {
                UiWork_Finalize(work, one);
            }
        }
    }
}
