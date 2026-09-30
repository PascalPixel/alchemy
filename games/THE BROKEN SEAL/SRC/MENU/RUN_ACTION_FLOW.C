#include "TYPES.H"

/* menu/run_action_flow.c */
struct MenuActionObject {
    u8 padding0[5];
    u8 mode;
};

struct MenuActionState {
    u8 padding0[0x14];
    struct MenuActionObject *object;
    u8 padding18[0x15C];
    u16 selection;
};

extern struct MenuActionState *gMenuWork;

s32 GameFlag_TestFar(s32 flag);
s32 CharacterMenu_SelectOwner(s32 index);
s32 CharacterMenu_SelectCommand(void);
s32 PsynergyMenu_SelectAction(void);
s32 ItemMenu_SelectItem(void);

s32 Menu_RunActionFlow(void)
{
    struct MenuActionState *state = gMenuWork;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !GameFlag_TestFar(0x150)) {
        switch (step) {
        case 0:
            state->selection = finished;
            if (CharacterMenu_SelectOwner(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->object->mode = 13;
            result = CharacterMenu_SelectCommand();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->object->mode = 13;
            result = PsynergyMenu_SelectAction();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->object->mode = 13;
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

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct OwnerCursor {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[6];
    u16 frame;
};

struct OwnerSelectMenu {
    u8 unknown_000[0x10];
    s32 window;
    struct OwnerCursor *cursors[2];
    s8 owner_index[2];
    u8 unknown_01e[0x202];
    u16 mode;
};


void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_IsSet(s32 flag);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_RunRearrange(void);
void UiIcon_PrepareObject(struct OwnerCursor *cursor);
void WaitFrames(s32 frames);

/* Opens the owner selector for party SLOT: shows its cursor, slides to the
   member it last chose (or clears the choice), runs the Psynergy or the
   rearrange selector and returns its result. */
s32 CharacterMenu_SelectOwner(s32 slot)
{
    struct OwnerSelectMenu *menu;
    struct OwnerCursor *cursor;
    s32 result;
    s32 index;

    menu = (struct OwnerSelectMenu *)gMenuWork;
    index = menu->owner_index[slot];
    result = 0;
    cursor = menu->cursors[slot];
    cursor->state = 1;
    cursor->frame = 0;
    RenderOutput_RedrawSavedRectFar(menu->window);
    if (GameFlag_IsSet(0x172))
        UiWindow_DrawDividerLineFar(menu->window, 9, 1, 9, 3);
    if (index == -1)
        menu->owner_index[slot] = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->mode == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = CharacterSelector_RunRearrange();
    cursor = menu->cursors[slot];
    UiIcon_PrepareObject(cursor);
    WaitFrames(1);
    return result;
}
#endif
