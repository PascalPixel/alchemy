#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};
u8 *Owner_GetStateFar(s32 group);
s32 Inventory_AddItemFar(s32 group, s32 value);
void Inventory_EquipFar(s32 group, s32 index);

/* Apply a value to every matching member of a fifteen-slot group. */
void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Owner_GetStateFar(group);
    s32 i;
    Inventory_AddItemFar(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Inventory_EquipFar(group, i);
    }
}
