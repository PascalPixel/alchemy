#include "TYPES.H"
#include "SCENE.H"
#include "TBS_EDITION.H"

struct SideObjectRegistry {
#if defined(TBS_EDITION_JA)
    u8 pad_0000[0x117c];
    u16 state_ids[2];
    u16 state_values[2];
#else
    u8 pad_0000[0x12ec];
    u16 state_ids[2];
    u16 state_values[2];
#endif
};

struct SideObject {
    u8 pad_00[4];
    u8 mode_04;
    u8 pad_05[20];
    u8 slot_19;
};

extern struct SideObjectRegistry *gWindowWork;

extern s32 GameFlag_IsSet(s32);
extern s32 Localization_LookupEntryId(s32);

extern struct SideObject *RenderOutput_Create(
    s32, s32, s32, s32, s32);

struct SideObject *CreateSideObject(
    s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct SideObjectRegistry *state = gWindowWork;
    struct SideObject *object = 0;
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
    object = RenderOutput_Create(first, 0x80000000, arg3, arg4, arg5);
    if (object != 0) {
        s32 slotBits = slot << 4;

        object->slot_19 = (object->slot_19 & 15) | slotBits;
        object->mode_04 = 2;
    }

    state->state_ids[side] = id;
    state->state_values[side] = first;
    return object;
}
