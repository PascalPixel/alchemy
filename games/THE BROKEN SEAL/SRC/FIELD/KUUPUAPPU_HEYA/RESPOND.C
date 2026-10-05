#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "text/MSG_IDS.H"


void Object_RefreshSelectorById(s32 actor);

struct Responder {
    u8 unknown_00[6];
    u16 facing;
    u8 unknown_08[0x5c];
    s16 stop;
};

static inline void KuupuappuHeya_WalkOn(struct Responder *actor, s32 row, s32 half, u16 turn)
{
    Engine_ActorEnableActionCallback(25, KuupuappuHeya_ResponseScripts[row][actor->stop]);
    actor->stop = actor->stop - half * 2 + turn;
}

void KuupuappuHeya_RunActor25Response(void)
{
    struct Responder *actor;
    u32 facing;
    s32 half;

    actor = (struct Responder *)Object_GetById(25);
    facing = actor->facing & 0xf000;
    half = actor->stop >> 1;
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(25, 2);
    Engine_EventSetMessage((s32)MsgKuupuappuNoLeaveAlone);
    Engine_EventShowMessage(25, 0);
    Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
    switch (actor->stop) {
    case 4:
        if (facing > 0x2000 && facing < 0xa000) {
            Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptR);
            actor->stop = 2;
        } else {
            Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptP);
            actor->stop = 3;
        }
        break;
    case 0:
    case 2:
        if (facing > 0x2000 && facing < 0xa000) {
            KuupuappuHeya_WalkOn(actor, half, half, 1);
        } else {
            KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
        }
        break;
    case 1:
    case 3:
        if (facing > 0x6000 && facing < 0xe000) {
            KuupuappuHeya_WalkOn(actor, half, half, 1);
        } else {
            KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
        }
        break;
    }
    actor->stop &= 3;
    Object_RefreshSelectorById(25);
    Engine_EventEnd();
}
