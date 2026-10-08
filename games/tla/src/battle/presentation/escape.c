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
#include "system.h"
#include "btlpty.h"
#include "btldisp.h"
#include "text/msg_ids.h"


s32 BattleEscape_PlayRun(void *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Random16() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, (u16 *)party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                BattleMotion_SetupEscapeObject(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    UiText_ShowMessageAndWaitCoreFar((s32)MsgCannotEscape);
    return 0;
}

s32 BattlePres_ShowDefendMessage(s16 *script)
{
    s32 object_id;
    s32 result;
    struct BtlUnit *unit;

    object_id = *script;
    unit = UnitGet(object_id);
    if (BattleObject_IsValidId(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (unit->hp <= 0) {
        return result;
    }
    UiWork_ClearValueNameTablesFar();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
    return 0;
}
