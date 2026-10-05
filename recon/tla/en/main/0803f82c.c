#include "TYPES.H"
#include "SELECT.H"
#include "RAM_BUFFER.H"

void MenuSelection_BuildEntries(u32);
void Menu_SetupSelectionBothSides(void);
void Menu_OpenSelectionWindow(u32, u32);
void Resource_ScheduleOwnerResetDelayed(void);
u32 Menu_WaitForSelectionInput(u32);
void Resource_ResetOwnerEntries(void);

u32 Menu_RunSelectionForValue(u32 value)
{
    struct SelectionScreen *state = Ram_HeapSlots->menu_work;
    u32 result;

    state->cursor_index = value;
    state->locked = 1;
    MenuSelection_BuildEntries(value);
    Menu_SetupSelectionBothSides();
    Menu_OpenSelectionWindow(0, 5);
    Resource_ScheduleOwnerResetDelayed();
    result = Menu_WaitForSelectionInput(1);
    Resource_ResetOwnerEntries();
    return result;
}
