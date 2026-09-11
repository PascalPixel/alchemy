#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_selection.h"
#include "gs1_edition.h"

struct MenuDefaults {
    u8 padding_000[0x1C8];
    s16 primary;
    s16 secondary;
};

struct TextObject {
    u8 storage[12];
};

struct Work;

extern struct MenuDefaults gCell;
extern volatile u32 gIw;

struct Work *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Menu_DrawSelectionRow(
    struct Work *work, s16 primary, const s16 *secondary);

void WaitFrames(s32 frames);

void UiWork_Finalize(struct Work *work, s32 release);

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
    primary = gCell.primary;
    secondary = gCell.secondary;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    Menu_Apply(&object, &resource);

    while (gIw != 0)
        WaitFrames(1);

    for (;;) {
        result = Menu_SetMode(work, primary, &secondary, &mode);
        if (result == -1) {
            Menu_Do(resource);
            UiWork_Finalize(work, 2);
            Menu_Apply2(primary, secondary);
            return result;
        }
        if (result == -2) {
            Menu_Do(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        Menu_Place(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}
