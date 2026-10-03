#include "TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "MOTION_OBJECT.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TARGET.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_MSG.H"
#include "SYSTEM.H"
#include "ANIMSPR.H"

void Object_SetMode(void *actor, s32 mode);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(s32 value, s32 mode);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattleMotion_SetMode5AndActivateSlot(s32 unit_id);

s32 AnimationObjects_SelectAnimationFar(void *, s32);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *);
s32 ActivateBattleObjectSlot(s32);

/* Takes damage from a unit's HP, stopping at zero, and reports it with the
 * unit posed in mode 5: an optional opening line (a bitter blow for units 0
 * to 7, the party, and a critical hit for the others), the damage message,
 * and a downed message once HP reaches zero. The unit returns to mode 1 at
 * the end. */
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

    character = Owner_GetStateFar(unit_id);
    character->hp -= damage;
    if (character->hp < 0)
        character->hp = 0;

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->object, 5);
    UiWindow_DrawPartyStatusContentsFar(0);
    UiWork_ClearValueNameTablesFar();

    if (unit_id <= 7) {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgBitterBlow);
        UiWork_PushValueSlotFar(damage, 5);
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgDmgP);
    } else {
        if (show_message != 0)
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgCritical);
        UiWork_PushValueSlotFar(damage, 5);
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgDmgE);
        UiWork_PushValueSlotFar(unit_id, 1);
    }

    BattleMotion_SetMode5AndActivateSlot(unit_id);
    if (unit_id <= 7) {
        if (character->hp <= 0) {
            UiWork_PushValueSlotFar(unit_id, 1);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgGoesDown);
        }
    } else if (character->hp <= 0) {
        UiWork_PushValueSlotFar(unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgDowned);
    }

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->object, 1);
}

void BattleMotion_SetMode5AndActivateSlot(s32 unit_id)
{
  struct AnimationEntry *actor_data;
  struct AnimationObject *object;
  if (Owner_GetStateFar(unit_id)->hp <= 0)
  {
    object = GetBattleObjectSlot(unit_id)->object->records;
    AnimationObjects_SelectAnimationFar(object, 5);
    actor_data = object->entries[0];
    actor_data->param = 6;
    actor_data->frame = 0xFF;
    WaitFrames(4);
    Map_RenderAnimatedTileFramesForObjectFar(object);
    ActivateBattleObjectSlot(unit_id);
  }
}

s32 BattleTarget_ReplaceDefeated(const u8 *action)
{
    s16 living_units[14];
    s32 target_id;
    s32 living_count;

    target_id = ((const struct BattleCommandRequest *)action)->target;
    if (Owner_GetStateFar(target_id)->hp != 0) {
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
