#include "TYPES.H"

struct State_0801c304 {
    u8 filler0[0x39e];
    u16 value;
    u8 filler3a0[0x18];
    u16 active;
};

extern struct State_0801c304 *gResQueueWork;
void Func_0801a7f4(u32);
void Menu_SetupSelectionBothSides(void);
void Menu_OpenSelectionWindow(u32, u32);
void Resource_ScheduleOwnerResetDelayed(void);
u32 Menu_WaitForSelectionInput(u32);
void Resource_ResetOwnerEntries(void);

u32 Menu_RunSelectionForValue(u32 value)
{
    struct State_0801c304 *state = gResQueueWork;
    u32 result;

    /* 値、使用中フラグの順に設定する。 */
    state->value = value;
    state->active = 1;
    Func_0801a7f4(value);
    Menu_SetupSelectionBothSides();
    Menu_OpenSelectionWindow(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_WaitForSelectionInput(1);
    Resource_ResetOwnerEntries();
    return result;
}
