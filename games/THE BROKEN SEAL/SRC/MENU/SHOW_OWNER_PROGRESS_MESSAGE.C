#include "RUNTIME_MEM.H"
#include "CHARACTER_MENU.H"
#include "BATTLE_UNIT.H"
#include "EDITION.H"
#include "BATTLE_RUNTIME.H"

#include "TYPES.H"

extern struct CharacterMenuState *gMenuWork;
u32 Owner_GetLevelThresholdFar(s32 owner_id, s32 level);
void UiWork_PushValueSlotFar(u32 quantity, s32 style);
void UiText_CopyMessageStringFar(s32 message_id, void *buffer, s32 length);
void UiText_RenderWideStringAtOffsetFar(void *buffer, struct UiWindow *destination, s32 offset, s32 terminator);

extern u8 MsgProgressHelp;

#if EDITION_INTERNATIONAL
#define PROGRESS_TEXT_MAX 0x80
#else
#define PROGRESS_TEXT_MAX 0x40
#endif

void StatusMenu_ShowOwnerProgressMessage(
    struct UiWindow *destination,
    s32 message_variant,
    s32 preserve_variant)
{
    struct CharacterMenuState *menu = gMenuWork;
    void *buffer;

    if (preserve_variant == 0 && message_variant > 3) {
        message_variant++;
    }
    if (message_variant == 1) {
        struct BattleUnit *owner = Owner_GetStateFar(menu->selected_ids[0]);

        if (owner->level == 99) {
            message_variant = 8;
        } else {
#if EDITION_INTERNATIONAL
            u64 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1) -
                owner->experience;
#else
            u32 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1) -
                owner->experience;
#endif
            UiWork_PushValueSlotFar(remaining, 5);
        }
    }

    buffer = Runtime_BumpAllocate(PROGRESS_TEXT_MAX * 2);
    UiText_CopyMessageStringFar(message_variant + (s32)&MsgProgressHelp, buffer, PROGRESS_TEXT_MAX);
    UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
    Runtime_BumpFree(buffer);
}
