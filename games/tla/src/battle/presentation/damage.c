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
#include "motobj.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "field_event.h"
#include "system.h"
#include "animspr.h"
#include "fixed_math.h"
#include "battle_party.h"
#include "battle_system.h"
#include "battle_target.h"
#include "text/msg_ids.h"

void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage, u8 *context)
{
    u8                 localContext[4];
    struct BattleUnit *character;
    struct BattleObj  *slot;

    if (context == 0)
    {
        context = localContext;
        context[0] = 0;
        context[1] = 0;
        context[2] = 0;
        context[3] = 0;
    }

    character = UnitGet(unitId);
    character->hp -= damage;
    if (character->hp < 0)
        character->hp = 0;

    slot = BattleObjGet(unitId);
    Object_SetMode(slot->object, 5);
    PartyStatusDrawFar(0);
    UiWork_ClearValueNameTablesFar();

    if (unitId <= 7)
    {
        if (showMessage != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgBitterBlow);
        UiText_DrawQuantity(damage, 5);
        UiText_DrawQuantity(unitId, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgP);
    }
    else
    {
        if (showMessage != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgCritical);
        UiText_DrawQuantity(damage, 5);
        UiText_DrawQuantity(unitId, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgE);
        UiText_DrawQuantity(unitId, 1);
    }

    BattleMotion_SetMode5AndActivateSlot(unitId);
    if (unitId <= 7)
    {
        if (character->hp <= 0)
        {
            UiText_DrawQuantity(unitId, 1);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDown);
        }
    }
    else if (character->hp <= 0)
    {
        UiText_DrawQuantity(unitId, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDowned);
    }

    slot = BattleObjGet(unitId);
    Object_SetMode(slot->object, 1);
}

struct BattleObjectAnimation
{
    u8                     unk00[0x28];
    struct AnimationEntry *entry;
};

s32 Animation_ApplyChildArgumentFar(void *group, s32 flags);

void BattleMotion_SetMode5AndActivateSlot(s32 unitId)
{
    struct BattleObjectAnimation *animation;
    struct AnimationEntry        *entry;

    if (((struct BattleUnit *)UnitGet(unitId))->hp <= 0)
    {
        animation = BattleObjGet(unitId)->object->records;
        Animation_ApplyChildArgumentFar(animation, 5);
        entry = animation->entry;
        entry->param = 6;
        entry->frame = 0xff;
        WaitFrames(4);
        Map_RenderAnimatedTileFramesForObjectFar(animation);
        ActivateBattleObjectSlot(unitId);
    }
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action)
{
    s16 livingUnits[14];
    s32 targetId;
    s32 livingCount;

    targetId = action->target;
    if (((struct BattleUnit *)UnitGet(targetId))->hp != 0)
    {
        return targetId;
    }

    if (targetId > 0x7F)
    {
        livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
    }
    else
    {
        livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    }

    if (livingCount == 0)
    {
        return 0x100;
    }

    return livingUnits[(u32)(Random16() * livingCount) >> 0x10];
}
