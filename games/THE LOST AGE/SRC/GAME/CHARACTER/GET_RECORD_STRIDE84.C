#include "PROTO.H"
#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Inventory_GetEquippedItem(struct BattleUnit *owner, s32 type);

struct ClassDefinition *Owner_GetRecordStride84(s32 class_id)
{
    return &Class_DefinitionTable[class_id];
}

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct BattleUnit *owner = Owner_GetState(owner_no);

    owner->class_index = Owner_DetermineClass(owner->enemy_id,
        owner->djinn_available, Inventory_GetEquippedItem(owner, 10));
    Owner_UpdateClassActions(owner_no);
    Owner_CalculateBaseElements(owner_no, owner->base_elements);
}
