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

s32 Inventory_GetEquippedItem(struct BattleUnit *owner, s32 type);

struct ClassDefinition *Owner_GetRecordStride84(s32 classId)
{
    return &Class_DefinitionTable[classId];
}

void Owner_RefreshDerivedData(s32 owner_id)
{
    struct BattleUnit *owner = UnitGet(owner_id);

    owner->classIndex = Owner_DetermineClass(owner->enemyId,
        owner->djinnAvailable, Inventory_GetEquippedItem(owner, 10));
    Owner_UpdateClassActions(owner_id);
    Owner_CalculateBaseElements(owner_id, owner->baseElement);
}

void Owner_RefreshClassActions(s32 owner_id)
{
    Owner_RefreshDerivedData(owner_id);
}

extern const s16 Element_PowerResistCurve[5][2];

s32 Curve_LookupScaledValue(s32 input, s32 halve)
{
    s32 min;
    s32 max;
    s32 n;
    s32 i;
    s32 value;
    s32 dx;
    s32 dy;

    max = Element_PowerResistCurve[0][0];
    min = Element_PowerResistCurve[4][0];
    n = 5;
    if (input > max)
        input = max;
    else if (input < min)
        input = min;
    for (i = 0; i < n; i++) {
        if (input > Element_PowerResistCurve[i][0])
            break;
    }
    if (i == n) {
        value = Element_PowerResistCurve[i - 1][1];
    } else {
        dx = Element_PowerResistCurve[i - 1][0] - Element_PowerResistCurve[i][0];
        dy = Element_PowerResistCurve[i - 1][1] - Element_PowerResistCurve[i][1];
        value = (input - Element_PowerResistCurve[i][0]) * dy / dx
            + Element_PowerResistCurve[i][1];
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
