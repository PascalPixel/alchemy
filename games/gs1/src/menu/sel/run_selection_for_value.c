#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_selection_for_value.h"

struct State_0801c304 {
    u8 filler0[0x39e];
    u16 value;
    u8 filler3a0[0x18];
    u16 active;
};

extern struct State_0801c304 *gIw;

void Resource_ScheduleOwnerResetDelayed(void);

void Resource_ResetOwnerEntries(void);

u32 Menu_RunSelectionForValue(u32 value)
{
    struct State_0801c304 *state = gIw;
    u32 result;

    /* 値、使用中フラグの順に設定する。 */
    state->value = value;
    state->active = 1;
    Menu_Do(value);
    Menu_Run();
    Menu_Apply(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_Do2(1);
    Resource_ResetOwnerEntries();
    return result;
}
