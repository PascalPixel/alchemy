#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"

void SceneDialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x96d) == 0) {
        GameFlag_Set(0x96d);
        Event_SetMessage(MSG_ROBIN_ID_REALLY_LIKE_THANK);
        Event_ShowMessage(9, 0);
    } else {
        Event_SetMessage(MSG_BABI_WAITING_FOR_AT_COLOSSEUM);
        Event_ShowMessage(9, 0);
    }
}
