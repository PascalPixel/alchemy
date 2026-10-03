#include "CHARACTER_MENU.H"
#include "BATTLE_UNIT.H"
#include "EDITION.H"
/* NONMATCHING: 3 halfwords. Reload loads the 0x0be6 message base into r3; the
 * reference reloads it into r0, the call's own argument register. Spellings
 * of the sum (temporary, nested call, split add, base variable) move nothing.
 */
#include "TYPES.H"

extern struct CharacterMenuState *gMenuWork;
struct BattleUnit *Owner_GetStateFar(s32 owner_id);
u32 Owner_GetLevelThresholdFar(s32 owner_id, s32 level);
void UiWork_PushValueSlotFar(u32 quantity, s32 style);
void *Runtime_BumpAllocate(s32 size);
void UiText_CopyMessageStringFar(s32 message_id, void *buffer, s32 length);
void UiText_RenderWideStringAtOffsetFar(void *buffer, struct UiWindow *destination, s32 offset, s32 terminator);
void Runtime_BumpFree(void *buffer);

/* The message id base is a link-time symbol, loaded from the literal pool. */
extern u8 MsgProgressHelp;

/* The characters of the help line: the Japanese line is shorter. */
#if EDITION_INTERNATIONAL
#define PROGRESS_TEXT_MAX 0x80
#else
#define PROGRESS_TEXT_MAX 0x40
#endif


void StatusMenu_ShowOwnerProgressMessage(
    void *destination,
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
