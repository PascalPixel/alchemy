#include "TYPES.H"
#include "SYSTEM.H"

#define KEYS_NEW (*(volatile u32 *)0x03001c94)


struct PsynergyOwnerMenu {
    u8 padding000[8];
    s32 selected_owner;
    u8 padding00c[0x10];
    s8 selection;
    u8 padding01d;
    s8 count;
    u8 padding01f[0xed];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x74];
    u8 entries[0x40];
    u16 character_ids[8];
    u8 entry_count;
    u8 padding219;
    u8 item_owner;
    u8 padding21b[5];
    u16 frame;
};

extern u8 Value_00000c05;

void *Owner_GetStateFar(s32 owner);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 Math_Mod(s32 numerator, s32 denominator);
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 frame);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyOwnerMenu *menu, s32 owner);
s32 PsynergyMenu_CollectActions(void *owner, void *entries, s32 mode);
void PsynergyMenu_DrawPsynergyIcons(void *entries);
void Menu_PlaceEntryObjectsInGrid(s32 x, s32 y, s32 columns);
void Menu_HideEmptyEntryIcons(void *entries);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);

/* Choose whose Psynergy to list: left and right step through the party,
   redrawing that member's Psynergy page; A returns 1 and B -1, and the
   chosen member is stored as the selected owner. */
s32 PsynergyMenu_SelectOwner(void)
{
    struct PsynergyOwnerMenu *menu = *(struct PsynergyOwnerMenu **)0x03001f2c;
    s32 selection = menu->selection;
    s32 count = menu->count;
    s32 pending = 1;
    s32 frame = menu->frame;
    s32 result;
    s32 i;

    Owner_GetStateFar(menu->character_ids[selection]);
    RenderOutput_RedrawSavedRectFar(menu->selector_window);
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000c05, menu->selector_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000c05 + 1, menu->selector_window, 0, 16);
    for (;;) {
        if (pending) {
            pending = 0;
            selection = Math_Mod(selection + count, count);
            Owner_GetStateFar(menu->character_ids[selection]);
            frame = Math_Mod(frame + 3, 3);
            Menu_CreateWindowAndEntryObjects(menu->character_ids[selection], frame);
            PsynergyMenu_CallIconRoutineWithValue(menu, menu->character_ids[selection]);
            for (i = 0; i < 8; i++)
                menu->row_positions[i] = 30;
            menu->row_positions[selection] = 26;
            menu->entry_count = PsynergyMenu_CollectActions(Owner_GetStateFar(menu->character_ids[selection]), menu->entries, 0);
            PsynergyMenu_DrawPsynergyIcons(menu->entries);
            Menu_PlaceEntryObjectsInGrid(96, 96, 8);
            Menu_HideEmptyEntryIcons(menu->entries);
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (KEYS_NEW & 1) {
            Audio_PlayCue(112);
            result = 1;
            break;
        }
        if (KEYS_NEW & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        {
            volatile u32 *repeat = (volatile u32 *)0x03001b04;

            if (*repeat & 32) {
                Audio_PlayCue(111);
                if (count > 1) {
                    selection--;
                    pending = 1;
                }
            }
            if (*repeat & 16) {
                Audio_PlayCue(111);
                if (count > 1) {
                    selection++;
                    pending = 1;
                }
            }
        }
    }
    menu->selection = selection;
    menu->selected_owner = menu->character_ids[selection];
    menu->item_owner = menu->character_ids[selection];
    return result;
}
