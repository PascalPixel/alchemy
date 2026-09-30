#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
s32 BattleFx_FindConditionResourceFar(s16, s16);
void Event_SetPairWork1c0Far(s16 primary, s16 secondary);
void UiTextResource_Release(s32 resource);

/* menu/draw/draw_selection_row.c */
struct Work;

void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber(s32 n, s32 digits, s32 work, s32 x, s32 y);
void UiText_DrawString(u8 *str, s32 work, s32 x, s32 y);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);
extern u8 MsgDebugEntryName[];
extern u8 MsgDebugCategoryName[];
extern u8 Menu_ColonString[];

s16 Menu_RunSelection(void)
{
    s32 resource;
    s16 mode;
    s16 secondary;
    struct TextObject object;
    s16 primary;
    struct Work *work;
    s16 result;

    work = 0;
    mode = 0;
    primary = gGameState.primary;
    secondary = gGameState.secondary;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    UiTextResource_Initialize(&object, &resource);

    while (gKeysHeld != 0)
        WaitFrames(1);

    for (;;) {
        result = Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
        if (result == -1) {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            Event_SetPairWork1c0Far(primary, secondary);
            return result;
        }
        if (result == -2) {
            UiTextResource_Release(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}
