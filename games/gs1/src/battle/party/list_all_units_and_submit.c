#include "types.h"
#include "scene.h"
#include "abi/battle/party/list_all_units_and_submit.h"

void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    Battle_Apply(3, local);
    Battle_Apply2(local, 0);
}
