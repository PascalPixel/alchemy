#include "TYPES.H"
#include "IO_REG.H"

extern u8 gEventWork[];

extern volatile u32 gKeysHeld;

void BattleEv_RunWait(s32 action)
{
    u8 *runtime = *(u8 **)gEventWork;
    s32 wait_token = UiText_OpenMessageAtObject();
    s32 resolved_action;
    u32 frames = 0;

    WaitFrames(1);
    resolved_action = ObjectTable_ReadActiveValue(action);
    if (action <= 7) {
        s32 masked_action = action & 0x0fff;

        if (BattleAction_FindDescriptor(masked_action) == 0) {
            resolved_action = masked_action;
        }
    }
    UiWork_FinalizeEntityMatchingLocalizedIdFar(resolved_action);

    if (*(s32 *)(runtime + 0x1cc) == 0) {
        while (UiWork_IsIdleFar(wait_token) == 0) {
            WaitFrames(1);
            frames++;
            if (frames > 600 ||
                ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                 (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
                UiWork_FinalizePendingCoreFar();
            }
        }
    }

    WaitFrames(1);
}
