#include "battle_runtime.h"
#include "item.h"

u8 Item_GetTargetMode(s32 item_id) {
    return BattleAction_GetDirect(
        Item_GetDirect(item_id)->action_id)->target_mode;
}
