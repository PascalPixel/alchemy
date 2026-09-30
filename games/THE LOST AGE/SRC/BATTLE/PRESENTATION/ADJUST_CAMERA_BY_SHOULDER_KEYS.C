#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"

s32 BattleObject_IsValidId(u32 object_id)
{
    if (object_id <= 7) {
        return 0;
    }
    if (object_id - 128 <= 5) {
        return 0;
    }
    return -1;
}
