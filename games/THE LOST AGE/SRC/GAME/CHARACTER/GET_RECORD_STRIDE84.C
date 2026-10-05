#include "TYPES.H"
#include "OWNER_STATE.H"

s32 Inventory_GetEquippedItem(struct BattleUnit *owner, s32 type);
s32 Owner_DetermineClass(s32 class_id, u32 *djinn, s32 item);
void Owner_UpdateClassActions(s32 owner_no);
void Owner_CalculateBaseElements(s32 owner_no, struct BattleElementStats *elements);

/* A class's row of the class table. */
struct ClassDefinition *Owner_GetRecordStride84(s32 class_id)
{
    return &Class_DefinitionTable[class_id];
}

/* Recomputes what an owner's class, Djinn and equipment decide: the class
   (⚓️ keeps its halfword where enemies keep their id), the class actions
   and the elemental levels. */
void Owner_RefreshDerivedData(s32 owner_no)
{
    struct BattleUnit *owner = Owner_GetState(owner_no);

    owner->class_index = Owner_DetermineClass(owner->enemy_id,
        owner->djinn_available, Inventory_GetEquippedItem(owner, 10));
    Owner_UpdateClassActions(owner_no);
    Owner_CalculateBaseElements(owner_no, owner->base_elements);
}
