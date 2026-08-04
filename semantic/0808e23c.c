/*
 * Owner behavior words below are tagged: values below 0x10000 are script
 * identifiers, while larger values are direct three-argument handlers.
 * The latter call is an audited GCC call-via-r3 veneer represented as an
 * ordinary C function pointer.
 */
#include "layout_guard.h"
#include "types.h"

typedef void (*OwnerHandler_0808e23c)(u16 item, s32 actor, s32 slot);

typedef struct Character_0808e23c {
    u8 padding000[0xd8];
    u16 inventory[15];
} Character_0808e23c;

typedef struct ItemOwner_0808e23c {
    u32 match_flags;
    u16 behavior_flags;
    s16 parameter;
    u32 behavior;
} ItemOwner_0808e23c;

typedef struct ItemDefinition_0808e23c {
    u8 padding00[0x0c];
    u8 type;
    u8 padding0d[0x1b];
    u16 field_action;
} ItemDefinition_0808e23c;

typedef struct ActionRuntime_0808e23c {
    u8 padding000[0x170];
    s16 special_anchor;
    u8 padding172[0xb54];
    s8 action_running;
} ActionRuntime_0808e23c;

typedef struct FieldState_0808e23c {
    u16 current_map;
    u16 current_variant;
    u8 padding004[0x1f4];
    u8 party_ids[0x48];
    u16 saved_map;
    u16 saved_variant;
} FieldState_0808e23c;

LAYOUT_OFFSET_GUARD(
    Character0808e23c_Inventory,
    Character_0808e23c,
    inventory,
    0xd8);
LAYOUT_OFFSET_GUARD(
    ItemOwner0808e23c_BehaviorFlags,
    ItemOwner_0808e23c,
    behavior_flags,
    4);
LAYOUT_OFFSET_GUARD(
    ItemOwner0808e23c_Behavior,
    ItemOwner_0808e23c,
    behavior,
    8);
LAYOUT_OFFSET_GUARD(
    ItemDefinition0808e23c_Type,
    ItemDefinition_0808e23c,
    type,
    0x0c);
LAYOUT_OFFSET_GUARD(
    ItemDefinition0808e23c_FieldAction,
    ItemDefinition_0808e23c,
    field_action,
    0x28);
LAYOUT_OFFSET_GUARD(
    ActionRuntime0808e23c_SpecialAnchor,
    ActionRuntime_0808e23c,
    special_anchor,
    0x170);
LAYOUT_OFFSET_GUARD(
    ActionRuntime0808e23c_ActionRunning,
    ActionRuntime_0808e23c,
    action_running,
    0xcc6);
LAYOUT_OFFSET_GUARD(
    FieldState0808e23c_PartyIds,
    FieldState_0808e23c,
    party_ids,
    0x1f8);
LAYOUT_OFFSET_GUARD(
    FieldState0808e23c_SavedMap,
    FieldState_0808e23c,
    saved_map,
    0x240);

extern FieldState_0808e23c Data_02000240;
extern u32 Data_02000434;
extern ActionRuntime_0808e23c *Data_03001ebc;

void Func_08015040(s32, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
Character_0808e23c *Func_08077008(s32);
ItemDefinition_0808e23c *Func_08077018(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);
void Func_080770d0(s32);
void Func_08077058(s32, s32);
s32 Func_08077148(s32);
s32 Func_0808ddec(u32);
ItemOwner_0808e23c *Func_0808e14c(u16);
void Func_080916b0(void);
void Func_08091750(void);
s32 Func_08091d84(s32);
void Func_08092b94(s16);
void Func_08092f84(s32, s32);
void Func_08096810(void);
void Func_08096fb0(u16, s32);
void Func_08097194(void);

static s32 CountOwnedItems_0808e23c(s32 actor, u16 item)
{
    Character_0808e23c *character = Func_08077008(actor);
    s32 count = 0;
    s32 slot;

    for (slot = 0; slot < 15; slot++) {
        if ((character->inventory[slot] & 0x01ff) == item)
            count++;
    }
    return count;
}

static s32 FinalizeItemUse_0808e23c(s32 actor, s32 slot, s32 result)
{
    if (Func_080770c0(0x142) != 0)
        Func_08015040(0x927, 1);
    if (Func_080770c0(0x143) != 0)
        Func_08077058(actor, slot);
    return result;
}

/* Resolve an item owner and dispatch its field-use or scripted behavior. */
s32 Func_0808e23c(u32 packed_item, s32 slot)
{
    u16 item = packed_item & 0x03ff;
    s32 actor = (packed_item >> 10) & 0x0f;
    s32 party_count = Func_08077148(actor);
    s32 owned = 0;
    s32 result = -1;

    if (actor == 0x0f) {
        s32 party_index;

        actor = 0;
        for (party_index = 0; party_index < party_count; party_index++) {
            s32 candidate = Data_02000240.party_ids[party_index];
            s32 candidate_owned =
                CountOwnedItems_0808e23c(candidate, item);

            if (owned < candidate_owned) {
                owned = candidate_owned;
                actor = candidate;
            }
        }
    } else {
        owned = CountOwnedItems_0808e23c(actor, item);
    }

    if (owned == 0) {
        Func_08015040(0x927, 1);
        return -1;
    }

    {
        ItemOwner_0808e23c *owner = Func_0808e14c(item);

        if (owner != 0 && owner->behavior != 0) {
            Func_080770d0(0x143);
            Func_080770d0(0x142);
            if ((owner->behavior_flags & 0x0400) == 0) {
                Func_08015120(actor, 1);
                Func_08015120(item, 2);
                Func_08015040(0x91c, 1);
            }

            if (owner->behavior < 0x10000) {
                s32 map_object = Func_0808ddec(Data_02000434);

                Func_080916b0();
                Func_08092b94((s16)owner->behavior);
                Func_08092f84(map_object, 0);
                Func_08091750();
            } else {
                ((OwnerHandler_0808e23c)owner->behavior)(
                    item, actor, slot);
            }
            return FinalizeItemUse_0808e23c(actor, slot, 0);
        }
    }

    Func_080770d0(0x143);
    Func_080770c8(0x142);

    {
        ItemDefinition_0808e23c *definition = Func_08077018(item);
        u16 action = definition->field_action;

        if (action != 0) {
            Func_080770c8(0x145);
            Func_080770d0(0x142);

            if (action == 0x95 && Func_080770c0(0x144) == 0) {
                s32 selection;

                Func_08015120(item, 2);
                Func_08015040(0x924, 0x0d);
                selection = Func_08091d84(1);
                Func_08015140();
                if (selection != 0)
                    return 0;

                Data_02000240.current_map = Data_02000240.saved_map;
                Data_02000240.current_variant =
                    Data_02000240.saved_variant;
                Data_03001ebc->special_anchor = 0x03e7;
            }

            Func_08015120(actor, 1);
            Func_08015120(item, 2);
            Func_08015040(0x91c, 1);
            Func_08096fb0(action, 0);
            Data_03001ebc->action_running = 1;
            Func_08096810();
            Data_03001ebc->action_running = 0;
            Func_08097194();
            if ((Func_08077018(item)->type & 1) != 0)
                Func_080770c8(0x143);
        }
    }

    return FinalizeItemUse_0808e23c(actor, slot, result);
}
