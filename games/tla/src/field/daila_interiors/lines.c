#include "types.h"
#include "field_event.h"
#include "party_state.h"
#include "scene_call.h"
#include "text/msg_ids.h"
#include "daila.h"
#include "field_scene.h"
#include "scene_ids.h"

void Func_020000ac(s32 unused, s32 actor)
{
    Deri_Jump(actor, 0x100, 50);
    Deri_ObjLaunch(actor, 6, 15);
    Deri_ObjLaunch(actor, 6, 15);
    Deri_Wait(10);
    if (!Deri_FlagChk(0x200)) {
        Deri_MesSet((s32)MsgDeriHeyaGah);
        Deri_MesPut(actor, 0);
        Deri_ObjWalk(actor, 360, 72);
        Deri_ObjTurn(actor, 0x4000, 0);
        Deri_FlagSet(0x200);
    } else {
        Deri_MesSet((s32)MsgDeriHeyaYarg);
        Deri_MesPut(actor, 0);
        Deri_ObjWalk(actor, 392, 104);
        Deri_ObjTurn(actor, 0x8000, 0);
        Deri_FlagClr(0x200);
    }
    Deri_Wait(20);
}

void Func_02000150(s32 unused, s32 actor)
{
    ObjectMotion_SetAngleToward(actor, 4, 0);
    Battle_WaitMode0(10);
    Func_02000d94(actor, 0x103, 50);
    ObjectMotion_Launch(actor, 6, 15);
    ObjectMotion_Launch(actor, 6, 15);
    Battle_WaitMode0(10);
    Engine_EventSetMessage((s32)MsgDeriHeyaWhatAreYouDoing);
    Engine_EventShowMessage(actor, 0);
    Battle_WaitMode0(10);
}

s32 DeriHeya_TalkScamps(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDeriHeyaScamps);
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
    return 0;
}

s32 DeriHeya_TalkMazeTreasure(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDeriHeyaMazeTreasure);
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
    return 0;
}

void DeriHeya_BeginLeaderEvent(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetSpritePriority(gPartyState.current_owner, 1);
}

extern const u8 Data_020016e4[];
extern const u8 Data_02001924[];
extern const u8 Data_02001990[];
extern const u8 Data_02001c18[];

const struct SceneEvent *Scene_GetEvents(void)
{
    s32 flag = GameFlag_Test(0x873);
    struct PartyState *party = &gPartyState;
    s32 id = (s32)&SceneId_DeriHeya2;

    if (flag) {
        if (party->scene == id)
            return (const struct SceneEvent *)Data_02001c18;
        return (const struct SceneEvent *)Data_02001990;
    }
    if (party->scene == id)
        return (const struct SceneEvent *)Data_02001924;
    return (const struct SceneEvent *)Data_020016e4;
}
