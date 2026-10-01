#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
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

/* menu/sel/run_selection.c */
struct MenuDefaults {
    u8 padding_000[0x1C8];
    s16 primary;
    s16 secondary;
};

struct TextObject {
    u8 storage[12];
};

void UiTextResource_Initialize(struct TextObject *object, s32 *resource);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);
extern struct MenuDefaults gGameState;
extern volatile u32 gKeysHeld;
struct Work *UiWindow_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Menu_DrawSelectionRow(
    struct Work *work, s16 primary, const s16 *secondary);
void UiWork_Finalize(struct Work *work, s32 release);
extern volatile u32 gKeysRepeat;
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second);
s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg);

s16 Menu_RunSelection(void);

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((s32)&Menu_RunSelection, 0xC80);
}

void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second)
{
    s16 selected = first;
    s32 label = BattleFx_FindConditionResourceFar(selected, *second) + (s32)MsgDebugEntryName;
    RenderOutput_PrepareForRedraw(work);
    UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
    UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 0);
    UiText_DrawResource(selected + (s32)MsgDebugCategoryName, (s32)work, 0, 0);
    UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 14);
    UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

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
        result = (s16)Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
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

/* Menu rows: step the value or its sub-value with the direction and shoulder keys, returning -1 on A and -2 on B. */
s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg)
{
    register s16 *mode asm("r0") = mode_arg; /* FAKEMATCH: keeps the mode pointer in r0 */
    if (gKeysRepeat & 1)
        return -1;
    if (gKeysRepeat & 2)
        return -2;
    if ((gKeysRepeat & 0x80) || (gKeysRepeat & 0x40)) {
        *mode ^= 1;
    } else if (gKeysRepeat & 0x10) {
        if (*mode == 0) {
            value++;
        } else {
            (*sub)++;
            if (*sub > 99)
                *sub = 0;
        }
        if (value > 200)
            value = 0;
        ((void (*)(s32, s32, s16 *))Menu_DrawSelectionRow)(arg, value, sub);
    } else if (gKeysRepeat & 0x20) {
        if (*mode == 0) {
            value--;
        } else {
            (*sub)--;
            if (*sub < 0)
                *sub = 99;
        }
        if (value < 0)
            value = 200;
        ((void (*)(s32, s32, s16 *))Menu_DrawSelectionRow)(arg, value, sub);
    } else if (gKeysRepeat & 0x100) {
        if (*mode == 0) {
            *sub = 0;
            value += 10;
        } else {
            *sub += 10;
            if (*sub > 99)
                *sub -= 99;
        }
        if (value > 200)
            value = 0;
        ((void (*)(s32, s32, s16 *))Menu_DrawSelectionRow)(arg, value, sub);
    } else if (gKeysRepeat & 0x200) {
        if (*mode == 0) {
            *sub = 0;
            value -= 10;
        } else {
            *sub -= 10;
            if (*sub < 0)
                *sub += 99;
        }
        if (value < 0)
            value = 200;
        ((void (*)(s32, s32, s16 *))Menu_DrawSelectionRow)(arg, value, sub);
    }
    return value;
}
