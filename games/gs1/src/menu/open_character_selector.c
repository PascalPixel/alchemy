#include "layout_guard.h"
#include "types.h"
#include "gs1_edition.h"
#include "inventory_menu.h"
#include "character_menu.h"

#if defined(GS1_EDITION_JA)
#define ROW_CNT 4
#else
#define ROW_CNT 8
#endif

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

struct CharacterSelectorState {
    u8 padding000[0x24];
    s32 screen_handle;
    u8 padding028[0x0e4];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x0b4];
    u16 character_ids[8];
    u8 padding218;
    u8 character_count;
    u8 padding21a[6];
    u16 selector_flags;
};

LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_ScreenHandle,
    struct CharacterSelectorState,
    screen_handle,
    0x24);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_SelectorWindow,
    struct CharacterSelectorState,
    selector_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_RowPositions,
    struct CharacterSelectorState,
    row_positions,
    0x144);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_CharacterIds,
    struct CharacterSelectorState,
    character_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    CharacterMenu080a7380_SelectorFlags,
    struct CharacterSelectorState,
    selector_flags,
    0x220);

extern struct MenuObjectControl *Data_03001e68;

struct CharacterSelectorState *Runtime_AllocateHeapBlock(s32, s32);
void Func_08002dd8(s32);
void WaitFrames(s32);
void Func_08015278(s32);
void Func_08015408(s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
s32 Func_08077158(const u16 *);
void Func_080a1090(s32);
void Func_080a8034(s32, s32, s32, s32);
s32 Func_080a7440(void);

/*
 * Open the compact character selector, run its blocking interaction body,
 * and tear the screen down.  The shared object list is suspended while this
 * modal owns the display.  Its eight row anchors all begin at x=30.
 */
s32 OpenCharacterSelector(void)
{
    struct CharacterSelectorState *state =
        Runtime_AllocateHeapBlock(55, 0x0a70);
    s32 result;
    s32 index;

    Data_03001e68->suspended = 1;
    Func_08015408(0, 0, 30, 20);
    WaitFrames(1);
    Func_080a1090(0);

    state->character_count =
        (u8)Func_08077158(state->character_ids);
    Func_080a8034(0, 3, 0, 7);
    state->selector_window = Func_08015010(13, 0, 17, 5, 2);
    for (index = 0; index < ROW_CNT; index++)
        state->row_positions[index] = 30;
    state->selector_flags = 3;

    result = Func_080a7440();

    Func_08015278(state->screen_handle);
    InventoryMenu_CloseWindows();
    Data_03001e68->suspended = 0;
    WaitFrames(1);
    Func_08002dd8(55);
    return result;
}
