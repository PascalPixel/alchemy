#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BTLSYS.H"
#include "MOTOBJ.H"
#include "FIXED_MATH.H"
#include "BTLPTY.H"
#include "BATTLE_TARGET.H"
#include "BATTLE_COMMAND.H"
#include "BTLMES.H"
#include "SYSTEM.H"
#include "ANIMSPR.H"
#include "FIELD_EVENT.H"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

void UiWork_PushValueSlotFar(s32 value, s32 mode);

void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context)
{
    u8 local_context[4];
    struct BtlUnit *character;
    struct BtlObj *slot;

    if (context == 0) {
        context = local_context;
        context[0] = 0;
        context[1] = 0;
        context[2] = 0;
        context[3] = 0;
    }

    character = UnitGetFar(unit_id);
    character->hp -= damage;
    if (character->hp < 0)
        character->hp = 0;

    slot = BtlObjGet(unit_id);
    ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 5);
    PtyStDrawFar(0);
    UiWork_ClearValueNameTablesFar();

    if (unit_id <= 7) {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgBitterBlow);
        UiWork_PushValueSlotFar(damage, 5);
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgP);
    } else {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)MsgCritical);
        UiWork_PushValueSlotFar(damage, 5);
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgE);
        UiWork_PushValueSlotFar(unit_id, 1);
    }

    BattleMotion_SetMode5AndActivateSlot(unit_id);
    if (unit_id <= 7) {
        if (character->hp <= 0) {
            UiWork_PushValueSlotFar(unit_id, 1);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDown);
        }
    } else if (character->hp <= 0) {
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgDowned);
    }

    slot = BtlObjGet(unit_id);
    ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 1);
}

void BattleMotion_SetMode5AndActivateSlot(s32 unit_id)
{
  struct AnimationEntry *actor_data;
  struct AnimationObject *object;
  if (UnitGetFar(unit_id)->hp <= 0)
  {
    object = BtlObjGet(unit_id)->object->records;
    AnimationObjects_SelectAnimationFar(object, 5);
    actor_data = object->entries[0];
    actor_data->param = 6;
    actor_data->frame = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar(object);
    ActivateBattleObjectSlot(unit_id);
  }
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action)
{
    u16 living_units[14];
    s32 target_id;
    s32 living_count;

    target_id = action->target;
    if (UnitGetFar(target_id)->hp != 0) {
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

    return (s16)living_units[(Random16() * living_count) >> 0x10];
}
