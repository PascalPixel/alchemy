#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "edition.h"
#include "btlsys.h"

#include "types.h"
#include "text/msg_ids.h"

void UiWork_PushValueSlotFar(u32 quantity, s32 style);


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
        struct BtlUnit *owner = UnitGetFar(menu->selected_ids[0]);

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

    buffer = MemAlloc(PROGRESS_TEXT_MAX * 2);
    UiText_CopyMessageStringFar(message_variant + (s32)MsgProgressHelp, buffer, PROGRESS_TEXT_MAX);
    UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
    MemFree(buffer);
}
