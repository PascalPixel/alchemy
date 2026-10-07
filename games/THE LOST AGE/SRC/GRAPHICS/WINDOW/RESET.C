#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "SELECT.H"
#include "RAM_BUFFER.H"

s32 Func_0803dc1c();

void Ui_ClearWorkStateAndWaitFrame(void)
{
    Func_0803dc1c();
    Resource_ClearOwnerListAndCounters();
    WaitFrames(1);
}

void Func_0803dd98(u32);
void Func_0803e7ac(void);
void Menu_OpenSelectionWindow(u32, u32);
void Func_0803df00(void);
u32 Func_0803e998(u32);
void Resource_ResetOwnerEntries(void);

u32 Menu_RunSelectionForValue(u32 value)
{
    struct SelectionScreen *state = Ram_HeapSlots->menu_work;
    u32 result;

    /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
    asm volatile("" : "+l"(state));
    state->cursor_index = value;
    state->locked = 1;
    Func_0803dd98(value);
    Func_0803e7ac();
    Menu_OpenSelectionWindow(0, 5);
    Func_0803df00();
    result = Func_0803e998(1);
    Resource_ResetOwnerEntries();
    return result;
}
