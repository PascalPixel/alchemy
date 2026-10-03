#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "UI.H"
#include "EVENT_RUNTIME.H"
#include "WINDOW.H"
#include "FIELD_SCENE.H"

void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32);
void Object_SetModeById(u32, s32);
void Object_WaitUntilChildValueDiffers(s32, s32);

/* The field event allocation also holds the confirmation menu's parameters. */
struct EventPromptWork {
    struct EventRuntime event;
    u8 unknown_200[0xac2];
    s16 resource_base;
    s16 width;
};

LAYOUT_SIZE_GUARD(EventPromptBase_Size, struct EventRuntime, 0x200);
LAYOUT_OFFSET_GUARD(EventPromptWork_ResourceBase, struct EventPromptWork, resource_base, 0xcc2);
LAYOUT_OFFSET_GUARD(EventPromptWork_Width, struct EventPromptWork, width, 0xcc4);

extern volatile s32 gKeyState;
struct ActionDescriptor;
struct ActionDescriptor *BattleAction_FindDescriptor(s32);
s32 Inventory_RequestMode(s32, s32, s32, s32);
void UiWork_FinalizePendingCoreFar(void);

s32 Menu_RunConfirmSelectionFar(s32 value, s32 x, s32 y, s32 z);

/* An empty routine after the inventory give-and-report code; nothing in
   the image calls it. */
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

/* Places the answer menu below the dialogue when its windows leave room. */
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom)
{
    struct EventPromptWork *work = (struct EventPromptWork *)gWork;
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

    answer = Inventory_RequestMode(use_bottom, work->resource_base, work->width, 0);
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
