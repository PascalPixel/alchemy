#include "EDITION.H"
#include "TYPES.H"
#include "MENU_LIST.H"
#include "RENDER_INPUT.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "RUNTIME_INTERFACES.H"

struct SideObjectRegistry {
#if EDITION_INTERNATIONAL
    u8 pad_0000[0x12ec];
    u16 state_ids[2];
    u16 state_values[2];
#else
    u8 pad_0000[0x117c];
    u16 state_ids[2];
    u16 state_values[2];
#endif
};

extern s32 GameFlag_IsSet(s32);
extern s32 Localization_LookupEntryId(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
s32 GameFlag_TestFar(s32);

struct RenderOutput *CreateSideObject(
    s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct SideObjectRegistry *state = (struct SideObjectRegistry *)gWindowWork[0];
    struct RenderOutput *object = 0;
    s32 first;
    s32 second;
    s32 id;
    s32 slot;

    if (GameFlag_IsSet(32) != 0) {
        if (object_kind == 0)
            object_kind = 18;
        if (object_kind == 1)
            object_kind = 19;
    }

    id = Localization_LookupEntryId(object_kind);
    if (id == -1)
        return object;

    if ((u32)side > 1) {
        side = 1;
        if (state->state_ids[1] != 999) {
            side = 0;
            if (state->state_ids[0] != 999)
                return object;
        }
    }

    slot = 14 + side;
    UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
    object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
    if (object != 0) {
        s32 slotBits = slot << 4;

        /* The top nibble of the packed table's second byte selects the slot. */
        ((u8 *)&object->table)[1] = (((u8 *)&object->table)[1] & 15) | slotBits;
        object->kind = 2;
    }

    state->state_ids[side] = id;
    state->state_values[side] = first;
    return object;
}

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
    s32 result;
    s32 current;
    u32 character_id;
    struct SideObjectRegistry *state;

    state = (struct SideObjectRegistry *)gWindowWork[0];

    if (GameFlag_TestFar(0x20) != 0) {
        if (character == 0)
            character = 0x12;
        if (character == 1)
            character = 0x13;
    }

    character_id = Localization_LookupEntryId(character);
    if (character_id != -1U) {
        if (slot > 1U) {
            if (state->state_ids[1] == character_id) {
                slot = 1;
            } else if (state->state_ids[0] == character_id) {
                slot = 0;
            } else {
                return;
            }
        }
        current = state->state_values[slot];
        UiGlyph_LoadEntryWithPalette(character_id, value, &current, &result, slot + 0xe, 1);
    }
}
