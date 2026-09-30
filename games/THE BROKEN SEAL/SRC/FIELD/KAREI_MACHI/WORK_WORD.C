#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 *gActorEffectWork;

/* Flag 0x200 records that the leader is linked into the actor effect
   work's word at +24. */
void SceneState_ClearWord24WhenFlag200(void)
{
    if (GameFlag_IsSet(0x200) != 0) {
        *(s32 *)(gActorEffectWork + 24) = 0;
        GameFlag_Clear(0x200);
    }
}

void SceneState_LinkRecordZeroWhenFlag200Clear(void)
{
    u8 *work;

    if (GameFlag_IsSet(0x200) == 0) {
        work = gActorEffectWork;
        *(s32 *)(work + 24) = ((s32 (*)())Object_GetById)(ACTOR_PARTY_LEADER);
        GameFlag_Set(0x200);
    }
}
