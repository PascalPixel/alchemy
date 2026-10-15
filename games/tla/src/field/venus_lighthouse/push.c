#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"

void SceneState_RunActor13AtColumn42Setup(void)
{
    struct FieldActor *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Engine_ActorGet(10);
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    if (obj->x >> 20 == 42) {
        Engine_EventWait(30);
        Engine_AudioPlayCue(188);
        obj->motion_flags = 0;
        val = 0xfffe0000;
        obj->terrain_height = val;
        obj->y = val;
        Engine_GameFlagSet(0x200);
        a = 3;
        b = 5;
        Engine_MapCopyCellsTo(44, 117, 41, 117, a, b);
    }
    Engine_EventEnd();
}


void VenusLighthouse_ReadRelief(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetAnimation(Engine_PartyGetLeaderActor(), 1);
    Engine_MessageShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
    Engine_EventEnd();
}

void VenusLighthouse_ReadTrueHeart(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetAnimation(Engine_PartyGetLeaderActor(), 1);
    Engine_MessageShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
    Engine_EventEnd();
}

