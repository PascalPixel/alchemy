/* DRAFT of Unnamed_080aa768, the Djinn screen's main loop: a state machine
   over selecting a Djinni, giving, trading, setting and putting one on
   standby, with the help and status screens, until a state finishes.

   2026-10-05 deep finisher 14: rewritten from the listing (the previous body
   was permuter output at 468): alchemy diff 4 (559 instructions both).
   - the work cell is the Djinn screen's view (DJINN_MENU.H's DjinnMenuWork
     layout, with the owner at 0x08, the pair count at 0x218 and the pending
     command bits at 0x220 named here); the calls are the listing's own
     (AudioCommand_PlayFar, DjinnMenu_ShowHelp).
   - the cursor restore after a transfer is written out in both cases: as an
     inline function, integrate folds menu + 0x184 into a constant address
     that the search loop then keeps in r4, which pushes result off r4 and
     moves every allocation (386 -> 24; the column as an s32 holding the
     u16 remainder had taken it from 426 to 386).
   - done = 0 comes first, and the cursor is a local: 24 -> 4.
   Tried: column as u16/s16 or computed without the u16 cast, found as
   u16/s16/u8 (475..511), the loop as for/while/goto (same), the selection
   store as found * 10 + column (8), through a pointer (12), with a variable
   index (110).
   Remaining (both restores): sched2 issues lsls r3, r3, #1 (found * 10)
   before lsls r1, r1, #1 (the selection address); the ROM builds the
   address first.  The HImode field store takes the bit-field path, which
   computes its address after the value, so the address's reload insns sit
   after the multiply and win the tie on LUID. */
#include "TYPES.H"
#include "DJINN_MENU.H"

struct DjinnMenuState {
    u8 unknown_000[8];
    s32 owner;
    u8 unknown_00c[4];
    s32 status_window;
    struct RenderOutput *cursor;
    u8 unknown_018[4];
    s8 selected_column[2];
    u8 unknown_01e[6];
    s32 window;
    u8 unknown_028[0x08];
    s32 list_window;
    s32 second_window;
    u8 unknown_038[0xd4];
    s32 help_window;
    u8 unknown_110[0x34];
    u16 frames[8];
    u8 unknown_154[0x20];
    u16 selection[2];
    u16 pair_entries[2];
    struct RenderOutput *second_cursor;
    u8 unknown_180[0x04];
    struct DjinnMenuBackdrop *backdrop;
    u8 unknown_188[0x80];
    u16 owners[DJINN_LIST_OWNERS];
    u8 pair_count;
    u8 owner_count;
    u8 shown_owner[2];
    u8 unknown_21c[4];
    u16 pending;
    u8 unknown_222[0x32];
    u8 pair_index[2];
    u8 pair_element[2];
    u8 pair_owner[2];
};

extern struct DjinnMenuState *gMenuWork;

void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void RenderOutput_ClearListFar(s32 window);
void Owner_RecalculateStatsFar(s32 owner);
s32 Djinn_ActivateFar(s32 owner, s32 element, s32 index);
s32 Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
s32 Trade_RemoveOfferFar(s32 owner, s32 element, s32 index);
s32 Trade_AddOfferFar(s32 owner, s32 element, s32 index);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 target);
void Menu_SetFirstObjectRowCoordinates(s32 y);
s32 DjinnMenu_ShowHelp(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 Menu_OpenBackdropScreen(void);
s32 FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 z);
s32 OwnerAction_RunCompareLoop(s32 mode);
s32 Unnamed_080ae2f4(void);
void DjinnMenu_DrawElementList(struct DjinnListTable *tbl);
s32 Menu_ComputeEntryValues(void *tbl);

s32 Unnamed_080aa768(void)
{
    struct DjinnMenuState *menu;
    struct RenderOutput *cursor;
    s32 result;
    s32 state;
    s32 ret;
    s32 done;
    s32 i;

    done = 0;
    result = 0;
    ret = 0;
    menu = gMenuWork;
    cursor = menu->cursor;
    cursor->active = 13;
    cursor->unknown_0c = 0;
    Menu_OpenBackdropScreen();
    WaitFrames(1);
    state = 2;
    do {
        switch (state) {
        case 0:
            if (result < 0) {
                ret = -1;
                done = 1;
            }
            state = 2;
            break;
        case 1:
            break;
        case 2:
            Menu_SetFirstObjectRowCoordinates(0);
            FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
            result = DjinnMenu_SelectDjinn(0);
            state = 15;
            if (result == 10)
                break;
            state = 0;
            if (result < 0)
                break;
            menu->selection[1] = menu->selected_column[0];
            state = 10;
            if (result == 7)
                break;
            state = 3;
            break;
        case 10:
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            result = Unnamed_080ae2f4();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 15:
            result = DjinnMenu_ShowHelp();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 8:
            state = 0;
            if (menu->pair_count == 0)
                break;
            result = DjinnMenu_SelectDjinn(1);
            if (result == -2)
                done = 1;
            state = 4;
            if (result < 0)
                break;
            state = 9;
            break;
        case 3:
            DjinnMenu_DrawElementList(&menu->backdrop->list);
            Menu_SetFirstObjectRowCoordinates(-8);
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            FourObjectMotion_SetSlotPosition(0, menu->selected_column[0] * 56 + 48, 54, 0);
            result = DjinnMenu_SelectDjinn(1);
            for (i = 0; i < menu->owner_count; i++)
                menu->frames[i] += 8;
            if (result == -2)
                done = 1;
            if (result < 0) {
                state = 2;
                break;
            }
            if (result == 3 || result == 4 || result == 8 || result == 9)
                menu->shown_owner[1] = menu->owners[menu->selected_column[1]];
            if (result < 0)
                state = 2;
            else if (result == 1)
                state = 5;
            else if (result == 2)
                state = 6;
            else if (result == 3) {
                menu->pending = 2;
                state = 7;
            } else if (result == 4) {
                menu->pending = 2;
                state = 9;
            } else if (result == 5)
                state = 11;
            else if (result == 6)
                state = 12;
            else if (result == 8) {
                menu->pending = 2;
                state = 13;
            } else if (result == 9) {
                menu->pending = 2;
                state = 14;
            }
            break;
        case 7:
            result = OwnerAction_RunCompareLoop(1);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 13:
            AudioCommand_PlayFar(126);
            result = Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            Owner_RecalculateStatsFar(menu->shown_owner[0]);
            Owner_RecalculateStatsFar(menu->shown_owner[1]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;
            
                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                menu->selection[0] = column + found * DJINN_LIST_ROWS;
                menu->cursor->active = 1;
            }
            state = 0;
            break;
        case 6:
            result = OwnerAction_RunCompareLoop(2);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 12:
            AudioCommand_PlayFar(175);
            Djinn_DeactivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_AddOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            Owner_RecalculateStatsFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        case 9:
            result = OwnerAction_RunCompareLoop(0);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 14:
            AudioCommand_PlayFar(126);
            Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            result = Djinn_TransferFar(menu->shown_owner[1], menu->pair_element[1], menu->pair_index[1], menu->shown_owner[0]);
            Owner_RecalculateStatsFar(menu->shown_owner[0]);
            Owner_RecalculateStatsFar(menu->shown_owner[1]);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;
            
                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                menu->selection[0] = column + found * DJINN_LIST_ROWS;
                menu->cursor->active = 1;
            }
            state = 2;
            break;
        case 4:
            if (result == -1) {
                ret = result;
                state = 2;
            } else if (menu->pending & 1)
                state = 8;
            else if (menu->pending & 2)
                state = 7;
            break;
        case 5:
            result = OwnerAction_RunCompareLoop(3);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 11:
            AudioCommand_PlayFar(139);
            Djinn_ActivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_RemoveOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            Owner_RecalculateStatsFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        default:
            done = 1;
            break;
        }
    } while (done == 0);
    return ret;
}
