#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_JA)
#define SLOT1_ID_OFS 0x117E
#define SLOT0_ID_OFS 0x117C
#define SLOT_VALUE_OFS 0x1180
#else
#define SLOT1_ID_OFS 0x12EE
#define SLOT0_ID_OFS 0x12EC
#define SLOT_VALUE_OFS 0x12F0
#endif

s32 GameFlag_IsSet(s32);
s32 Localization_LookupEntryId(s32);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
    s32 result;
    s32 current;
    u32 character_id;
    u8 *state;
    s32 offset;

    state = *(u8 **)ADDR_03001E8C;

    if (GameFlag_IsSet(0x20) != 0) {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    character_id = Localization_LookupEntryId(character);
    if (character_id != -1U) {
        if (slot > 1U) {
            if (*(u16 *)(state + SLOT1_ID_OFS) == character_id) {
                slot = 1;
            } else if (*(u16 *)(state + SLOT0_ID_OFS) == character_id) {
                slot = 0;
            } else {
                return;
            }
        }
        offset = SLOT_VALUE_OFS + slot * 2;
        current = *(u16 *)(state + offset);
        Func_0801a4fc(character_id, value, &current, &result, slot + 0xe, 1);
    }
}
