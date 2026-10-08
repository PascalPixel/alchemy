#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "tbs_edition.h"
#include "system.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "game_state.h"

#define UiWindow_Create ((struct Work * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Menu_ColonString ((u8 (*))&Menu_ColonString)

void Event_SetDestinationFar(s16 primary, s16 secondary);

struct Work;
void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber(s32 n, s32 digits, s32 work, s32 x, s32 y);
void UiText_DrawString(u8 *str, s32 work, s32 x, s32 y);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);

struct TextObject {
    u8 storage[12];
};

void UiTextResource_Initialize(struct TextObject *object, s32 *resource);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);
extern volatile u32 gKeysHeld;


void Scheduler_ScheduleCallbackCAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
}

void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second)
{
    s16 selected = first;
    s32 label = Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugEntryName;
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
    primary = gGameState.default_scene;
    secondary = gGameState.default_entrance;
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
            Event_SetDestinationFar(primary, secondary);
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

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode)
{
    if (gKeysRepeat & KEY_A)
        return -1;
    if (gKeysRepeat & KEY_B)
        return -2;
    if (gKeysRepeat & KEY_DOWN) {
        *mode ^= 1;
    } else if (gKeysRepeat & KEY_UP) {
        *mode ^= 1;
    } else if (gKeysRepeat & KEY_RIGHT) {
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
    } else if (gKeysRepeat & KEY_LEFT) {
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
    } else if (gKeysRepeat & KEY_R) {
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
    } else if (gKeysRepeat & KEY_L) {
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
