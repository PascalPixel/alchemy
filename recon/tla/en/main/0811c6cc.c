#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_MSG.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_TARGET.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
void UiWork_ClearValueNameTablesFar(void);
extern u8 gCameraWork[];
extern u8 Data_03001ae8[];
s32 BattlePres_ShowMessageWhenField38Positive(s16 *);
s32 BattlePres_RunUnitAction(s16 *);
s32 BattlePresentation_RunPairedUnitTransition(s16 *);
void BattleMotion_SetupEscapeObject(s32);

s32 BattlePres_ShowMessageWhenField38Positive(s16 *script)
{
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Owner_GetStateFar(object_id);
    if (BattleObject_IsValidId(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    UiWork_ClearValueNameTablesFar();
    UiWork_PushValueSlotFar(object_id, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
    return 0;
}
