#include "CHARACTER_MENU.H"
#include "TYPES.H"

extern struct CharacterMenuState *gMenuWork;

s32 GameFlag_TestFar(s32 flag);
s32 CharacterMenu_SelectOwner(s32 index);
s32 CharacterMenu_SelectCommand(void);
s32 PsynergyMenu_SelectAction(void);
s32 ItemMenu_SelectItem(void);

s32 Menu_RunActionFlow(void)
{
    struct CharacterMenuState *state = gMenuWork;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !GameFlag_TestFar(0x150)) {
        switch (step) {
        case 0:
            state->selected_slots[0] = finished;
            if (CharacterMenu_SelectOwner(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->owner_cursors[0]->active = 13;
            result = CharacterMenu_SelectCommand();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->owner_cursors[0]->active = 13;
            result = PsynergyMenu_SelectAction();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->owner_cursors[0]->active = 13;
            result = ItemMenu_SelectItem();
            /* Collapse every non-cancellation result to one. */
            changed = (u32)~result;
            step = (-changed | changed) >> 31;
            break;
        default:
            finished = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150))
        result = -1;

    return result;
}

void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_IsSet(s32 flag);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_RunRearrange(void);
void UiIcon_PrepareObject(struct RenderOutput *cursor);
void WaitFrames(s32 frames);

/* Opens the owner selector for party SLOT: shows its cursor, slides to the
   member it last chose (or clears the choice), runs the Psynergy or the
   rearrange selector and returns its result. */
s32 CharacterMenu_SelectOwner(s32 slot)
{
    struct CharacterMenuState *menu;
    struct RenderOutput *cursor;
    s32 result;
    s32 index;

    menu = (struct CharacterMenuState *)gMenuWork;
    index = menu->owner_index[slot];
    result = 0;
    cursor = menu->owner_cursors[slot];
    cursor->active = 1;
    cursor->unknown_0c = 0;
    RenderOutput_RedrawSavedRectFar((s32)menu->owner_window);
    if (GameFlag_IsSet(0x172))
        UiWindow_DrawDividerLineFar((s32)menu->owner_window, 9, 1, 9, 3);
    if (index == -1)
        menu->owner_index[slot] = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->page == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = CharacterSelector_RunRearrange();
    cursor = menu->owner_cursors[slot];
    UiIcon_PrepareObject(cursor);
    WaitFrames(1);
    return result;
}
