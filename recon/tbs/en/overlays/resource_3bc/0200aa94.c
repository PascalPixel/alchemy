/* NONMATCHING: resource_3bc at 0x0200aa94..0x0200abac (280 bytes with their
 * pools), ColossoLogRollingStage_RunStateInteraction and
 * InitializeStateInteraction, between FIELD/KOROSSEO_MARUTA/SAVED_POSITIONS.C
 * and MIDDLE.C, stay listing.
 *
 * Remaining difference: they load the round numbers 0x8f and 0x90 and the
 * finals messages from their pools, as link-time symbols do.
 */
#include "SITES.H"

s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = gGameState.scene;
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&LinkedMessage_StageFirstFinalsMatch;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&LinkedMessage_StageSecondFinalsMatch;
    } else {
        script_id = (s32)&LinkedMessage_StageThirdFinalsMatch;
    }
    Event_SetMessage(script_id);
    Event_ShowMessage(actor_handle, 0);
    if (GameFlag_IsSet(interaction_base + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    GameFlag_Set(interaction_base + 520);
    Event_SetMessage((s32)&LinkedMessage_WouldYouLikeHearDescription);
    Event_OpenMessage(actor_handle, 0);
    return Event_ChooseYesNo(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = gGameState.scene;
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&LinkedMessage_StageFirstFinalsMatch;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&LinkedMessage_StageSecondFinalsMatch;
    } else {
        script_id = (s32)&LinkedMessage_StageThirdFinalsMatch;
    }
    Event_SetMessage(script_id + 1);
    Event_ShowMessage(actor_handle, 0);
}
