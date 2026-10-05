#include "PROTO.H"
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "FIELD_EVENT.H"
#include "SYSTEM.H"
#include "ANIMSPR.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TARGET.H"
#include "text/MSG_IDS.H"


void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context)
{
    u8 local_context[4];
    struct BattleUnit *character;
    struct BattleObjectSlot *slot;

    if (context == 0) {
        context = local_context;
        context[0] = 0;
        context[1] = 0;
        context[2] = 0;
        context[3] = 0;
    }

    character = Owner_GetState(unit_id);
    character->hp -= damage;
    if (character->hp < 0)
        character->hp = 0;

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->object, 5);
    UiWindow_DrawPartyStatusContentsFar(0);
    UiWork_ClearValueNameTablesFar();

    if (unit_id <= 7) {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgBitterBlow);
        UiText_DrawQuantity(damage, 5);
        UiText_DrawQuantity(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgP);
    } else {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgCritical);
        UiText_DrawQuantity(damage, 5);
        UiText_DrawQuantity(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgE);
        UiText_DrawQuantity(unit_id, 1);
    }

    BattleMotion_SetMode5AndActivateSlot(unit_id);
    if (unit_id <= 7) {
        if (character->hp <= 0) {
            UiText_DrawQuantity(unit_id, 1);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDown);
        }
    } else if (character->hp <= 0) {
        UiText_DrawQuantity(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDowned);
    }

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->object, 1);
}

struct BattleObjectAnimation {
    u8 unknown_00[0x28];
    struct AnimationEntry *entry;
};

s32 Animation_ApplyChildArgumentFar(void *group, s32 flags);

void BattleMotion_SetMode5AndActivateSlot(s32 unit_id)
{
    struct BattleObjectAnimation *animation;
    struct AnimationEntry *entry;

    if (((struct BattleUnit *)Owner_GetState(unit_id))->hp <= 0) {
        animation = GetBattleObjectSlot(unit_id)->object->records;
        Animation_ApplyChildArgumentFar(animation, 5);
        entry = animation->entry;
        entry->param = 6;
        entry->frame = 0xff;
        WaitFrames(4);
        Map_RenderAnimatedTileFramesForObjectFar(animation);
        ActivateBattleObjectSlot(unit_id);
    }
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action)
{
    s16 living_units[14];
    s32 target_id;
    s32 living_count;

    target_id = action->target;
    if (((struct BattleUnit *)Owner_GetState(target_id))->hp != 0) {
        return target_id;
    }

    if (target_id > 0x7F) {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
    } else {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
    }

    if (living_count == 0) {
        return 0x100;
    }

    return living_units[(u32)(Random16() * living_count) >> 0x10];
}
