#include "TYPES.H"

extern u8 MsgBitterBlow[];
extern u8 MsgDmgP[];
extern u8 MsgCritical[];
extern u8 MsgDmgE[];
extern u8 MsgGoesDown[];
extern u8 MsgDowned[];


struct CharacterRuntimeRecord {
    u8 reserved_00[56];
    s16 hp;
};

struct BattleMotionSlot {
    void *actor;
};

struct CharacterRuntimeRecord *Owner_GetStateFar(s32 unit_id);
struct BattleMotionSlot *GetBattleObjectSlot(s32 unit_id);
void Object_SetMode(void *actor, s32 mode);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(s32 value, s32 mode);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattleMotion_SetMode5AndActivateSlot(s32 unit_id);

/* Takes damage from a unit's HP, stopping at zero, and reports it with the
 * unit posed in mode 5: an optional opening line (a bitter blow for units 0
 * to 7, the party, and a critical hit for the others), the damage message,
 * and a downed message once HP reaches zero. The unit returns to mode 1 at
 * the end. */

void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context)
{
    u8 local_context[4];
    struct CharacterRuntimeRecord *character;
    struct BattleMotionSlot *slot;

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
    Object_SetMode(slot->actor, 5);
    UiWindow_DrawPartyStatusContentsFar(0);
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

    slot = GetBattleObjectSlot(unit_id);
    Object_SetMode(slot->actor, 1);
}
