#include "types.h"
#include "scene.h"
#include "btlmes.h"
#include "battle_escape.h"
#include "btldisp.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "btlpty.h"
#include "system.h"

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
