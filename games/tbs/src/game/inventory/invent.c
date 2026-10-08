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
#include "object_runtime.h"
#include "animspr.h"
#include "scene.h"
#include "system.h"
#include "ui.h"
#include "event_runtime.h"
#include "window.h"
#include "field_scene.h"
#include "field_event.h"
#include "btlpty.h"
#include "b5_context.h"
s32 Menu_RunConfirmSelectionFar(s32, s32, s32, s32);

void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32);

struct EventPromptWork {
    struct EventRuntime event;
    u8 unknown_200[0xac2];
    s16 resource_base;
    s16 width;
};

extern volatile s32 gKeyState;
struct ActionDescriptor;
void UiWork_FinalizePendingCoreFar(void);

void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct ObjectRuntime *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->animation_kind == 1) {
        s32 cnt = 0;
        u8 *p = &((struct AnimationObject *)obj->animation)->last_no;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != *p) {
                break;
            }
            cnt++;
        }
    }
}

s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom)
{
    struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
    s32 sprite = ((struct ScenePlacement *)BattleAction_FindDescriptor(work->event.speaker))->sprite;
    struct UiWindow *window = (struct UiWindow *)work->event.message_window;
    struct UiWindow *side = (struct UiWindow *)work->event.side_window;
    s32 use_bottom = 1;
    s32 answer;

    while (gKeyState != 0)
        WaitFrames(1);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(3);

    if (force_bottom == 0) {
        s32 bottom = window->y + window->height;

        if (side != 0) {
            s32 side_bottom = side->y + side->height;
            if (bottom < side_bottom)
                bottom = side_bottom;
        }

        if (bottom > 15)
            use_bottom = 0;
    }

    answer = Menu_RunConfirmSelectionFar(use_bottom, work->resource_base, work->width, 0);
    if (answer != 0) {
        Object_SetModeById(actor, 4);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(actor, 4);
    } else {
        Object_SetModeById(actor, 3);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(actor, 3);
    }

    return answer;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}
