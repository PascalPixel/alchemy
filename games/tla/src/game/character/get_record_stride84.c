#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"

s32 Inventory_GetEquippedItem(struct BtlUnit *owner, s32 type);

struct ClassDefinition *Owner_GetRecordStride84(s32 class_id)
{
    return &Class_DefinitionTable[class_id];
}

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct BtlUnit *owner = UnitGet(owner_no);

    owner->class_index = Owner_DetermineClass(owner->enemy_id,
        owner->djinn_available, Inventory_GetEquippedItem(owner, 10));
    Owner_UpdateClassActions(owner_no);
    Owner_CalculateBaseElements(owner_no, owner->base_elm);
}

void Owner_RefreshClassActions(s32 owner_no)
{
    Owner_RefreshDerivedData(owner_no);
}

extern const s16 Data_080c6b04[5][2];

s32 Curve_LookupScaledValue(s32 input, s32 halve)
{
    s32 min;
    s32 max;
    s32 n;
    s32 i;
    s32 value;
    s32 dx;
    s32 dy;

    max = Data_080c6b04[0][0];
    min = Data_080c6b04[4][0];
    n = 5;
    if (input > max)
        input = max;
    else if (input < min)
        input = min;
    for (i = 0; i < n; i++) {
        if (input > Data_080c6b04[i][0])
            break;
    }
    if (i == n) {
        value = Data_080c6b04[i - 1][1];
    } else {
        dx = Data_080c6b04[i - 1][0] - Data_080c6b04[i][0];
        dy = Data_080c6b04[i - 1][1] - Data_080c6b04[i][1];
        value = (input - Data_080c6b04[i][0]) * dy / dx
            + Data_080c6b04[i][1];
    }
    switch (halve) {
    case 0:
        break;
    case 1:
        value /= 2;
        break;
    }
    return value + 0x100;
}
