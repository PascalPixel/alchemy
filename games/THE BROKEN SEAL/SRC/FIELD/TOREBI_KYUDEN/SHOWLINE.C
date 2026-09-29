#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"
extern u8 MsgTorebiBabiWaitingForAtColosseum[];
extern u8 MsgTorebiRobinIdReallyLikeThank[];
extern u8 MsgTorebiWarriorsWhoStayed[];

void SceneDialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x96d) == 0) {
        GameFlag_Set(0x96d);
        Event_SetMessage((s32)MsgTorebiRobinIdReallyLikeThank);
        Event_ShowMessage(9, 0);
    } else {
        Event_SetMessage((s32)MsgTorebiBabiWaitingForAtColosseum);
        Event_ShowMessage(9, 0);
    }
}

void SceneDialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)MsgTorebiWarriorsWhoStayed;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(k + 1);
        Event_ShowMessage(a, 0);
    } else {
        Event_SetMessage(k + 2);
        Event_ShowMessage(a, 0);
    }
}
