#include "TYPES.H"
#include "OWNER_STATE.H"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern char MsgActorDefends;
s32 BattleObject_IsValidId(s32);
void UiWork_ClearValueNameTablesFar(void);
void UiText_DrawQuantity(s32 value, s32 mode);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);

s32 BattlePres_ShowMessageWhenField38Positive(s16 *script)
{
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Owner_GetState(object_id);
    if (BattleObject_IsValidId(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    UiWork_ClearValueNameTablesFar();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
    return 0;
}
