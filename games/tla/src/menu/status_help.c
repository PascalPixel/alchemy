#include "types.h"
#include "ram_buffer.h"

struct StatusOwner {
    u8 unknown_000[15];
    u8 level;
    u8 unknown_010[276];
    s32 experience;
};

#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

extern u8 MsgYourNameAndLevelExperience[];
struct StatusOwner *UnitGet(s32 owner);
u32 Owner_GetLevelThresholdFar(s32 owner_id, s32 level);
void UiText_DrawQuantity(s32 value, s32 digits);
void *MemAlloc(s32 size);
void MemFree(void *buffer);
void UiText_CopyMessageStringFar(s32 message_id, void *buffer, s32 length);
void UiText_RenderWideStringAtOffsetFar(void *buffer, void *destination, s32 offset, s32 terminator);

void Func_080ff2e8(void *window, s32 help, s32 full)
{
    u8 *menu = Ram_HeapSlots->menu_runtime;
    u8 *buffer;

    if (full == 0 && help > 3)
        help++;
    if (help == 1) {
        u8 *owner = menu + 0x216;
        struct StatusOwner *state = UnitGet(*owner);

        if (state->level == 99)
            help = 8;
        else
            UiText_DrawQuantity(Owner_GetLevelThresholdFar(*owner, state->level + 1) - state->experience, 5);
    }
    buffer = MemAlloc(HELP_LENGTH * 2);
    UiText_CopyMessageStringFar(help + (s32)MsgYourNameAndLevelExperience, buffer, HELP_LENGTH);
    UiText_RenderWideStringAtOffsetFar(buffer, window, 0, -1);
    MemFree(buffer);
}
