#include "types.h"
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

extern struct MenuDefaults Data_02000240;
extern volatile u32 Data_03001ae8;

struct Work *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Menu_DrawSelectionRow(
    struct Work *work, s16 primary, const s16 *secondary);
void Func_0801c0dc(struct TextObject *object, s32 *resource);
void WaitFrames(s32 frames);
s16 Func_08029094(
    struct Work *work,
    s16 primary,
    s16 *secondary,
    s16 *mode);
void Func_0801c17c(s32 resource);
void UiWork_Finalize(struct Work *work, s32 release);
void Func_0808a238(s16 primary, s16 secondary);
void Func_0801c154(struct TextObject *object, s32 x, s32 y);

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
    primary = Data_02000240.primary;
    secondary = Data_02000240.secondary;
    work = UiWindow_Create(0, 7, 30, 5, 2);
    Menu_DrawSelectionRow(work, primary, &secondary);
    Func_0801c0dc(&object, &resource);

    while (Data_03001ae8 != 0)
        WaitFrames(1);

    for (;;) {
        result = Func_08029094(work, primary, &secondary, &mode);
        if (result == -1) {
            Func_0801c17c(resource);
            UiWork_Finalize(work, 2);
            Func_0808a238(primary, secondary);
            return result;
        }
        if (result == -2) {
            Func_0801c17c(resource);
            UiWork_Finalize(work, 2);
            return result;
        }

        Func_0801c154(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
        primary = result;
        WaitFrames(1);
    }
}
